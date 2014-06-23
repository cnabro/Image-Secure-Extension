#include "isecore.h"

jpeg_decompress_container read_jpeg_container(char *filename);
jpgx_compress_container write_jpgx(char *filename, jpeg_decompress_container container, secure_container **sc_array, int sc_arr_count, char *key);
jpgx_decompress_container read_jpgx_container(char* filename, char* key);