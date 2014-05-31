#include "stdafx.h"

#include "IseWrapper.h"
extern "C"
{
	jpgx_decompress_container IseWrapper::IseWrapperClass::getSecureJpegContainer(char *filename, char* key)
	{
		return get_secure_jpeg_container(filename, key);
	}

	pngx_decompress_container IseWrapper::IseWrapperClass::getSecurePngContainer(char *filename, char* key)
	{
		return get_secure_png_container(filename, key);
	}

	jpgx_compress_container IseWrapper::IseWrapperClass::makeJPGX(char *filename, secure_container sc_array[], int sc_arr_count)
	{
		return makeJPGX(filename,sc_array,sc_arr_count);
	}

	pngx_compress_container IseWrapper::IseWrapperClass::makePNGX(char *filename, secure_container sc_array[], int sc_arr_count)
	{
		return makePNGX(filename, sc_array, sc_arr_count);
	}
}
