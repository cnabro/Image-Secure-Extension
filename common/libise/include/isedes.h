#include <stdlib.h>
#include <stdio.h>
#include "des.h"
#include "config.h"

void des_encode_file(char *in_file_path, char *out_file_path, char * key);
void des_decode_file(char *in_file_path, char *out_file_path, char * key);
