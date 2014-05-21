#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#ifdef WIN32
#include <io.h>
#endif

#ifndef WIN32
#include <unistd.h>
#endif


#include "jpeglib.h"
#include "zlib.h"
#include "zip.h"
//#include "unzip.h"


typedef enum SECURE_TYPE{
	ST_NORMAL,
	ST_FACE
} SECURE_TYPE;

typedef struct jpeg_container{
	struct jpeg_decompress_struct dcinfo;
	unsigned char *image;
} jpeg_container;

typedef struct secure_container{
	int width;
	int height;
	int pos_x;
	int pos_y;
	SECURE_TYPE type;
} secure_container;

jpeg_container readjpeg(char *filename);
int writejpeg(char *filename, jpeg_container container, secure_container scarr[]);
//void compressJPGX(char *frompath, char *topath);
void compressFiles(char *infiles[], int file_count, char *out_zip);
char * getFileName(char* path);