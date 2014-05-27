#include "isecore.h"

jpeg_container read_jpeg_container(char *filename);
int write_jpeg_with_secure_container(char *filename, jpeg_container container, secure_container sc_array[], int sc_arr_count);