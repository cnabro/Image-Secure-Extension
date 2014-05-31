#include "isecore.h"

jpgx_decompress_container get_secure_jpeg_container(char *filename, char* key)
{
	printf("c# wrapper test : get_secure_jpeg_buffer : %s", filename);

	return ;
}

pngx_decompress_container get_secure_png_container(char *filename, char* key)
{
	printf("c# wrapper test : get_secure_png_buffer : %s", filename);

	return ;
}

jpgx_compress_container make_jpgx(char *filename, secure_container sc_array[], int sc_arr_count)
{
	printf("c# wrapper test : make_secure_jpeg");

	char *in_file_name = filename;
	return;
}

pngx_compress_container make_pngx(char *filename, secure_container sc_array[], int sc_arr_count)
{
	printf("c# wrapper test : make_secure_png");

	return;
}