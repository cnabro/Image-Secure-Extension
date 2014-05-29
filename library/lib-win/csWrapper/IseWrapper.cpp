#include "stdafx.h"

#include "IseWrapper.h"
extern "C"
{
	unsigned char* IseWrapper::IseWrapperClass::getSecureJpeg(char *filename, char* key)
	{
		return get_secure_jpeg_buffer(filename, key);
	}

	unsigned char* IseWrapper::IseWrapperClass::getSecurePng(char *filename, char* key)
	{
		return get_secure_png_buffer(filename, key);
	}

	int IseWrapper::IseWrapperClass::makeSecureJpeg(char *filename, jpeg_container container, secure_container sc_array[], int sc_arr_count)
	{
		return 11333;
	}

	int IseWrapper::IseWrapperClass::makeSecurePng(char *filename, png_container container, secure_container sc_array[], int sc_arr_count)
	{
		return 0;
	}
}
