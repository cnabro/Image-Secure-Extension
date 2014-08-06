#include "isecore.h"

png_decompress_container read_png_container(char *filename);
pngx_compress_container write_pngx(char *filename, png_decompress_container container, secure_container **sc_array, int sc_arr_count, char *user_key);
pngx_decompress_container read_pngx_container(char* filename, char* user_key);