#include "stdafx.h"

#include "IseWrapper.h"
extern "C"
{
	unsigned char* IseWrapper::IseWrapperClass::getSecureJpegBuffer(char *filename)
	{
		return get_secure_jpeg_buffer("E:/opensource/ise/source/library/lib-win/test/test.jpg");
	}

	unsigned char* IseWrapper::IseWrapperClass::getSecurePngBuffer(char *filename)
	{
		return get_secure_png_buffer("test.jpg");
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
