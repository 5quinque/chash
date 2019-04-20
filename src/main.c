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
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <png.h>
#include "chash_png.h"

#define _POSIX_SOURCE 1
#define USAGE "Usage: chash [image path] [-vth]"

void handle_args(int argc, char **argv, char **image_path);

int main(int argc, char **argv) {
  char *image_path = NULL;
  FILE *fp;
  png_structp png_ptr;
  png_infop info_ptr;
  png_infop end_info;
  png_uint_32 width;
  png_uint_32 height;
  png_bytep *row_pointers = NULL;
  int color_type;
  struct rgb colours;

  handle_args(argc, argv, &image_path);

  if (!image_path) {
    printf("%s\n", USAGE);
    return 1;
  }

  fp = fopen(image_path, "rb");

  
  // [todo] if mimetype == png
  if (!png_prechecks(image_path, &fp)) {
    return 1;
  }

  if (!png_setup(&png_ptr, &info_ptr, &end_info))
    fclose(fp);
  

  /* let libpng know that there are
   * some bytes missing from the start of the file.
   */
  /*
  png_set_sig_bytes(png_ptr, PNG_BYTES_TO_CHECK);
  png_init_io(png_ptr, fp);

  png_get_image_and_info(png_ptr, info_ptr, &row_pointers, &width, &height, &color_type);
  
  png_get_colours(row_pointers, width, height, color_type, &colours);
  */


  /* clean up */
  /*
  png_read_end(png_ptr, end_info);
  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

  for (png_uint_32 row = 0; row < height; row++) {
    free(row_pointers[row]);
  }

  free(row_pointers);
  */
  fclose(fp);

  return 0;
}

void handle_args(int argc, char **argv, char **image_path) {
  int c;

  while ((c = getopt(argc, argv, "vth")) != -1) {
    switch (c) {
    case 'v':
      printf("version 0.0.0\n");
      return;
      break;
    case 'h':
    case '?':
      printf("%s\n", USAGE);
      return;
    default:
      break;
    }
  }
  for (int index = optind; index < argc; index++) {
    *image_path = argv[index];
  }
}

