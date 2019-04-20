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

int png_prechecks(char *file_name, FILE **fp);
int check_if_png(FILE **fp);
int png_get_image_and_info(png_structp png_ptr, png_infop info_ptr,
    png_bytep **row_pointers, png_uint_32 *width, png_uint_32 *height, int *color_type);
int png_setup(png_structp *png_ptr, png_infop *info_ptr, png_infop *end_info);

struct rgb {
  int red;
  int green;
  int blue;
  int alpha;
};

void png_get_colours(png_bytep *row_pointers, png_uint_32 width,
    png_uint_32 height, int color_type, struct rgb *fg);

#define PNG_BYTES_TO_CHECK 8
