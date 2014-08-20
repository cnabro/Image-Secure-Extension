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
#include "md5.h"
#include "jpeglib.h"
#include "zlib.h"
#include "zip.h"
#include "unzip.h"
#include "mxml.h"
#include "png.h"

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#define ISE_STATUS_ERROR_IVALID_PWD  -1;
#define ISE_STATUS_ERROR_IVALID_FILE -2;
#define ISE_STATUS_ERROR_UNPACKING -3;
#define ISE_STATUS_OK 1;

static const unsigned char des3_test_keys[24] =
{
	0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
	0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01,
	0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23
};

static const unsigned char padding_bit[24] =
{
	0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
	0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01,
	0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23
};

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
	int status;
} jpeg_decompress_container;

typedef struct png_decompress_container
{
	png_structp png_ptr;
	png_infop info_ptr;
	png_bytep *image;
	int status;
} png_decompress_container;

/*
	jpgx container
*/ 
typedef struct jpgx_compress_container
{
	int sc_cnt;
	secure_container ** sc_arr;
	char *file_path;
} jpgx_compress_container;

typedef struct jpgx_decompress_container
{
	int status;
	int sc_cnt;
	secure_container ** sc_arr;
	jpeg_decompress_container jdcinfo;
} jpgx_decompress_container;

/*
	pngx container
*/
typedef struct pngx_decompress_container
{
	int status;
	int sc_cnt;
	secure_container ** sc_arr;
	png_decompress_container pdcinfo;
} pngx_decompress_container;

typedef struct pngx_compress_container
{
	int status;
	int sc_cnt;
	secure_container ** sc_arr;
	char *file_path;
} pngx_compress_container;

/*
	prop info
*/
typedef struct prop_info_container
{
	int status;
	char * key_hash;
	int sc_count;
	secure_container ** sc_arr;
	char ** file_name;
} prop_info_container;


/*
	main function
*/
jpgx_decompress_container get_jpgx_container(char *filename, char* key);
pngx_decompress_container get_pngx_container(char *filename, char* key);

jpgx_compress_container make_jpgx(char *filename, secure_container **sc_array, int sc_arr_count, char* key);
pngx_compress_container make_pngx(char *filename, secure_container **sc_array, int sc_arr_count, char* key);
#ifdef __cplusplus
}
#endif