#include "isecore.h"

jpgx_decompress_container get_secure_jpeg(char *filename, char* key)
{
	printf("c# wrapper test : get_secure_jpeg_buffer : %s", filename);

	return ;
}

pngx_decompress_container get_secure_png(char *filename, char* key)
{
	printf("c# wrapper test : get_secure_png_buffer : %s", filename);

	return ;
}

jpgx_compress_container make_secure_jpeg(char *filename, secure_container sc_array[], int sc_arr_count)
{
	printf("c# wrapper test : make_secure_jpeg");

	char *in_file_name = filename;
	return;
}

extern pngx_compress_container make_secure_png(char *filename, secure_container sc_array[], int sc_arr_count)
{
	printf("c# wrapper test : make_secure_png");

	return;
}