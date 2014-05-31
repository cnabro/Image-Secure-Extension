#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#ifdef WIN32
#include <io.h>
#include <direct.h>
#include <stdlib.h> 
#include <stdarg.h>
#endif

#ifndef WIN32
#include <unistd.h>
#endif

#include "des.h"
#include "jpeglib.h"
#include "zlib.h"
#include "zip.h"
#include "mxml.h"

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum SECURE_TYPE
{
	ST_NORMAL,
	ST_FACE
} SECURE_TYPE;

typedef struct secure_container
{
	int width;
	int height;
	int pos_x;
	int pos_y;
	SECURE_TYPE type;
} secure_container;

/*
image container
*/
typedef struct jpeg_decompress_container
{
	struct jpeg_decompress_struct dcinfo;
	unsigned char *image;
} jpeg_decompress_container;

typedef struct png_decompress_container
{
	//struct jpeg_decompress_struct dcinfo; TODO : set png struct
	unsigned char *image;
} png_decompress_container;



/*
	jpgx container
*/ 
typedef struct jpgx_compress_container
{
	int sc_cnt;
	secure_container *sc_arr;
	char *file_path;
} jpgx_compress_container;

typedef struct jpgx_decompress_container
{
	int sc_cnt;
	secure_container *sc_arr;
	jpeg_decompress_container jdcinfo;
} jpgx_decompress_container;

/*
	pngx container
*/
typedef struct pngx_decompress_container
{
	int sc_cnt;
	secure_container *sc_arr;
} pngx_decompress_container;

typedef struct pngx_compress_container
{
	int sc_cnt;
	secure_container *sc_arr;
	char *file_path;
} pngx_compress_container;

/*
	main function
*/

jpgx_decompress_container get_secure_jpeg_container(char *filename, char* key);
pngx_decompress_container get_secure_png_container(char *filename, char* key);

jpgx_compress_container make_jpgx(char *filename, secure_container sc_array[], int sc_arr_count);
pngx_compress_container make_pngx(char *filename, secure_container sc_array[], int sc_arr_count);
#ifdef __cplusplus
}
#endif