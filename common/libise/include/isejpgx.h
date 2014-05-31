#include "isecore.h"

jpeg_decompress_container read_jpeg_container(char *filename);
int write_jpgx(char *filename, jpeg_decompress_container container, secure_container sc_array[], int sc_arr_count, char *key);