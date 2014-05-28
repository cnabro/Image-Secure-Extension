#include "isecore.h"

unsigned char* get_secure_jpeg_buffer(char *filename)
{
	printf("c# wrapper test : get_secure_jpeg_buffer : %s", filename);

	return "test get_secure_jpeg_buffer";
}

unsigned char* get_secure_png_buffer(char *filename)
{
	printf("c# wrapper test : get_secure_png_buffer : %s", filename);

	return "test get_secure_png_buffer";
}


int make_secure_jpeg(char *filename, jpeg_container container, secure_container sc_array[], int sc_arr_count)
{
	printf("c# wrapper test : make_secure_jpeg");
}

int make_secure_png(char *filename, png_container container, secure_container sc_array[], int sc_arr_count)
{
	printf("c# wrapper test : make_secure_png");
}