#include "isecore.h"

int encode_file_des(char *in_file_path, char *out_file_path, char * key);
int decode_file_des(char *in_file_path, char *out_file_path, char * key);
char * make_des_key(char * key);
