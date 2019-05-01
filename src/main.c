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
//#include <math.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <png.h>
#include <jpeglib.h>
#include "chash_png.h"

#define _POSIX_SOURCE 1
#define USAGE "Usage: chash [image path] [-vth]"

int handle_args(int argc, char **argv, char **image_path);
int get_mimetype(FILE *fp);

int main(int argc, char **argv) {
  char *image_path = NULL;
  FILE *fp;
  int mimetype = 0;

  if (!handle_args(argc, argv, &image_path)) {
    return 1;
  }

  fp = fopen(image_path, "rb");
  if (!fp) {
    printf("Error opening file: %s\n", image_path);
    return 0;
  }

  mimetype = get_mimetype(fp);

  switch (mimetype) {  
  case 0:
    read_png(fp, image_path);
    break;
  case 1:
    printf("Reading jpeg..\n");
    //read_jpeg(fp, image_path);
    break;
  default:
    printf("%s\n", USAGE);
    return 1;
    break;
  }

  fclose(fp);

  return 0;
}

// [todo] check for jpeg
int get_mimetype(FILE *fp) {
  int is_png;
  is_png = check_if_png(&fp);

  if (is_png) {
    return 0;
  }

  return 1;
}

int grayscale(png_bytep *row_pointers) {
  //row_pointers[0][0] = 255;
  //row_pointers[0][1] = 255;
  //row_pointers[0][2] = 255;
  //png_set_rows(png_ptr, info_ptr, row_pointers);
  //png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
  
  return 1;
}

int handle_args(int argc, char **argv, char **image_path) {
  int c;

  while ((c = getopt(argc, argv, "vth")) != -1) {
    switch (c) {
    case 'v':
      printf("version 0.0.0\n");
      return 0;
      break;
    case 'h':
    case '?':
      printf("%s\n", USAGE);
      return 0;
      break;
    default:
      break;
    }
  }

  for (int index = optind; index < argc; index++) {
    *image_path = argv[index];
  }

  if (!*image_path) {
    printf("%s\n", USAGE);
    return 0;
  }

  return 1;
}

