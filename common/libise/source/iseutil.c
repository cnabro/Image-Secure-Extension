#include "ise.h"

char * getFileName(char* path)
{
	char *filepath;
	char *pToken = NULL;
	char *pFilename = NULL;

	char *pSeparator = "/";
	char *pExtenSeparator = ".";

	filepath = (char*)malloc(strlen(path));
	strcpy(filepath, path);

	pToken = strtok(filepath, pSeparator);
	pFilename = pToken;

	while ((pToken = strtok(NULL, pSeparator)) != NULL)
	{
		pFilename = pToken;
	}

	return pFilename;
}



//void compressJPGX(char *folder, char *topath)
//{
//	zip_fileinfo zfi;
//
//	zipFile zf = zipOpen("myarch.zip", APPEND_STATUS_ADDINZIP);
//	int ret = zipOpenNewFileInZip(zf,
//		"myfile.txt",
//		&zfi,
//		NULL, 0,
//		NULL, 0,
//		"my comment for this interior file",
//		Z_DEFLATED,
//		Z_NO_COMPRESSION
//		);
//	zipCloseFileInZip(zf);
//	zipClose(zf, "my comment for exterior file");
//}



