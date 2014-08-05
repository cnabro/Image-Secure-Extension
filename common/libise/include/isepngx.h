#include "isecore.h"
#include <png.h>

png_decompress_container read_png_container(char *filename);
pngx_decompress_container write_jpgx(char *filename, png_decompress_container container, secure_container **sc_array, int sc_arr_count);