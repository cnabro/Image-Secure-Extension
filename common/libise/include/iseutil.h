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



#include "zlib.h"
#include "zip.h"
