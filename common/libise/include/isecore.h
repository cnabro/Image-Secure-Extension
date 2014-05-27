#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#ifdef WIN32
#include <io.h>
#include <direct.h>
#endif

#ifndef WIN32
#include <unistd.h>
#endif

#include "des.h"
#include "jpeglib.h"
#include "zlib.h"
#include "zip.h"
#include "mxml.h"

typedef enum SECURE_TYPE{
	ST_NORMAL,
	ST_FACE
} SECURE_TYPE;

typedef struct secure_container{
	int width;
	int height;
	int pos_x;
	int pos_y;
	SECURE_TYPE type;
} secure_container;


typedef struct jpeg_container{
	struct jpeg_decompress_struct dcinfo;
	unsigned char *image;
} jpeg_container;


typedef struct png_container{
	//struct jpeg_decompress_struct dcinfo; TODO : set png struct
	unsigned char *image;
} png_container;

unsigned char* get_secure_jpeg_buffer(char *filename);
unsigned char* get_secure_png_buffer(char *filename);

int make_secure_jpeg(char *filename, jpeg_container container, secure_container sc_array[], int sc_arr_count);
int make_secure_png(char *filename, png_container container, secure_container sc_array[], int sc_arr_count);
