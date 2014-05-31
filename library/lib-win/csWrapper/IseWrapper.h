// libise_wrapper.h

#pragma once
#include "isecore.h"
using namespace System;

namespace IseWrapper
{
	extern "C"
	{
		public ref class IseWrapperClass
		{
		public:
			jpgx_decompress_container getSecureJpegContainer(char *filename, char* key);
			pngx_decompress_container getSecurePngContainer(char *filename, char* key);

			jpgx_compress_container makeJPGX(char *filename, secure_container sc_array[], int sc_arr_count);
			pngx_compress_container makePNGX(char *filename, secure_container sc_array[], int sc_arr_count);

		};

		jpgx_decompress_container get_secure_jpeg_container(char *filename, char* key);
		pngx_decompress_container get_secure_png_container(char *filename, char* key);

		jpgx_compress_container make_jpgx(char *filename, secure_container sc_array[], int sc_arr_count);
		pngx_compress_container make_pngx(char *filename, secure_container sc_array[], int sc_arr_count);
	}
}
