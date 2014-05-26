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