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
				static jpgx_decompress_container getSecureJpegContainer(char *filename, char* key);
				static pngx_decompress_container getSecurePngContainer(char *filename, char* key);

				static jpgx_compress_container makeJPGX(char *filename, secure_container *sc_array, int sc_arr_count, char* key);
				static pngx_compress_container makePNGX(char *filename, secure_container *sc_array, int sc_arr_count, char* key);

		};
	}
}
