#include "isecore.h"

png_container read_png_container(char *filename);
int write_png_with_secure_container(char *filename, png_container container, secure_container sc_array[], int sc_arr_count);