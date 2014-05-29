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
			unsigned char* getSecureJpeg(char *filename, char* key);
			unsigned char* getSecurePng(char *filename, char* key);

			int makeSecureJpeg(char *filename, jpeg_container container, secure_container sc_array[], int sc_arr_count);
			int makeSecurePng(char *filename, png_container container, secure_container sc_array[], int sc_arr_count);

		};

		unsigned char* get_secure_jpeg_buffer(char *filename, char* key);
		unsigned char* get_secure_png_buffer(char *filename, char* key);

		int make_secure_jpeg(char *filename, jpeg_container container, secure_container sc_array[], int sc_arr_count);
		int make_secure_png(char *filename, png_container container, secure_container sc_array[], int sc_arr_count);
	}
}
