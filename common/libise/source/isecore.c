#include "isecore.h"
#include "isejpgx.h"

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

jpgx_compress_container make_jpgx(char *filename, secure_container **sc_array, int sc_arr_count, char* key)
{
	
	jpeg_decompress_container container = read_jpeg_container(filename);
	char *out_temp_folder = str_concat(3, get_current_path(filename), ".", get_file_name(filename));

	_mkdir(out_temp_folder);
	jpgx_compress_container jpgx_container = write_jpgx(filename, container, sc_array, sc_arr_count, key);
	
	printf("c# wrapper test : make_secure_jpeg");
	return jpgx_container;
}

pngx_compress_container make_pngx(char *filename, secure_container **sc_array, int sc_arr_count, char* key)
{
	printf("c# wrapper test : make_secure_png");

	return;
}