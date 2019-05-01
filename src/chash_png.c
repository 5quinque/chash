/*

Copyright (C) 2019 Linnit <ryan@ryanl.co.uk>

This file is part of chash

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "chash_png.h"

int png_get_image_and_info(png_structp png_ptr, png_infop info_ptr,
    png_bytep **row_pointers, png_uint_32 *width, png_uint_32 *height, int *color_type) {
  int rowbytes;

  png_read_info(png_ptr, info_ptr);

  *width = png_get_image_width(png_ptr, info_ptr);
  *height = png_get_image_height(png_ptr, info_ptr);
  *color_type = png_get_color_type(png_ptr, info_ptr);
  
  *row_pointers = malloc(*height * sizeof(png_bytep));
  if (!*row_pointers) {
    printf("Error allocating memory for `row_pointers`\n");
    return 0;
  }

  rowbytes = png_get_rowbytes(png_ptr, info_ptr);

  for (png_uint_32 row = 0; row < *height; row++) {
    (*row_pointers)[row] = png_malloc(png_ptr, rowbytes);
    if (!(*row_pointers)[row]) {
      printf("Error allocating memory for `row_pointers[%d]`\n", row);
      return 0;
    }
  }

  png_read_image(png_ptr, *row_pointers);

  return 1;
}

int png_prechecks(char *file_name, FILE **fp) {
  int is_png;

  /* Can we open the file */
  if (!*fp) {
    printf("Error opening file: %s\n", file_name);
    return 0;
  }

  /* Is the file a PNG? */
  is_png = check_if_png(&*fp);
  if (!is_png) {
    printf("Error: %s, is not a PNG file\n", file_name);
    return 0;
  }

  return 1;
}

/* Most of this function is from libpng-1.5.13/example.c */
int check_if_png(FILE **fp) {
  unsigned char header[PNG_BYTES_TO_CHECK];

  if (!*fp)
    return 0;

  /* Read in some of the signature bytes */
  if(fread(header, 1, PNG_BYTES_TO_CHECK, *fp) != PNG_BYTES_TO_CHECK)
    return 0;

  /* Compare the first PNG_BYTES_TO_CHECK bytes of the signature.
  Return nonzero(true) if they match */

  return(!png_sig_cmp(header, (png_size_t)0, PNG_BYTES_TO_CHECK));
}

void png_get_colours(png_bytep *row_pointers, png_uint_32 width,
    png_uint_32 height, int color_type, struct rgb *fg) {
  int value_length = 4;

  if (color_type == 2) {
    value_length = 3;
  }

  for (png_uint_32 row = 0; row < height; row++) {
    for (png_uint_32 col = 0; col < (width * value_length); col += value_length) {
      fg->red = row_pointers[row][col+0];
      fg->green = row_pointers[row][col+1];
      fg->blue = row_pointers[row][col+2];
      fg->alpha = row_pointers[row][col+3];

      printf("%3d %3d %3d\t", fg->red, fg->green, fg->blue);
      //printf("%d %d %d %d  ", fg->red, fg->green, fg->blue, fg->alpha);
    }
    printf("\n");
  }
}

int png_setup(png_structp *png_ptr, png_infop *info_ptr, png_infop *end_info, FILE **fp) {
  //printf("png_setup: address of png_ptr %p \n", &*png_ptr);
  
  /* Setup png_struct and png_info structs */
  *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!*png_ptr) {
    printf("Error creating png_ptr\n");
    return 0;
  }

  *info_ptr = png_create_info_struct(*png_ptr);
  if (!*info_ptr) {
    png_destroy_read_struct(&*png_ptr,
      (png_infopp)NULL, (png_infopp)NULL);
    printf("Error creating info_ptr\n");
    return 0;
  }

  *end_info = png_create_info_struct(*png_ptr);
  if (!*end_info) {
    png_destroy_read_struct(&*png_ptr, &*info_ptr,
      (png_infopp)NULL);
    return 0;
  }

  /* instead of `setjmp` we could compile with PNG_SETJMP_NOT_SUPPORTED */
  /* pg. 8 http://www.libpng.org/pub/png/libpng-1.4.0-manual.pdf */
  if (setjmp(png_jmpbuf(*png_ptr))) {
    png_destroy_read_struct(&*png_ptr, &*info_ptr,
        &*end_info);
    printf("libpng encountered an error\n");
    return 0;
  }


  /* let libpng know that there are
   * some bytes missing from the start of the file
   * from signature check in `check_if_png`
   */
  png_set_sig_bytes(*png_ptr, PNG_BYTES_TO_CHECK);
  png_init_io(*png_ptr, *fp);

  return 1;
}

int read_png(FILE *fp, char *image_path) {
  png_structp png_ptr;
  png_infop info_ptr;
  png_infop end_info;
  png_uint_32 width;
  png_uint_32 height;
  png_bytep *row_pointers = NULL;
  int color_type;
  struct rgb colours;

  /* no longer need to do this
  if (!png_prechecks(image_path, &fp)) {
    return 0;
  }
  */

  if (!png_setup(&png_ptr, &info_ptr, &end_info, &fp)) {
    printf("error setting up png\n");
    fclose(fp);

    return 0;
  }  

  /* read the image */
  png_get_image_and_info(png_ptr, info_ptr, &row_pointers, &width, &height, &color_type);

  /* print pixel values */
  png_get_colours(row_pointers, width, height, color_type, &colours);

  /* clean up */
  png_read_end(png_ptr, end_info);
  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

  for (png_uint_32 row = 0; row < height; row++) {
    free(row_pointers[row]);
  }
  free(row_pointers);

  return 0;
}

