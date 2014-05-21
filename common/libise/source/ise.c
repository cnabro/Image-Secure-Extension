#include "ise.h"

int main()
{
	char *infilename = "./test/abcd.jpg";
	char *outfilename = "./test/.abcd/out.jpg";
	char *compress = "./test/out.jpg";
	char *test = "./test/test.jpg";
	secure_container scarr[2];

	jpeg_container container = readjpeg(infilename);


	//test

	char *infiles[] = { "./test/abcd.jpg", "C:/Users/gyu-il/Desktop/study.jpg" };
	//

	scarr[0].height = 400;
	scarr[0].width = 400;
	scarr[0].type = ST_NORMAL;
	scarr[0].pos_x = 500;
	scarr[0].pos_y = 500;

	scarr[1].height = 400;
	scarr[1].width = 700;
	scarr[1].type = ST_NORMAL;
	scarr[1].pos_x = 1500;
	scarr[1].pos_y = 1400;

	//mkdir("./test/.abcd/");
	//writejpeg(outfilename, container, scarr);
	//compressJPGX(outfilename, "./test/abcd.jpgx");

	//test code
	compressFiles(infiles, 2, "./test/test1.zip");

	return 0;
}

void compressFiles(char *infiles[], int file_count, char *out_zip)
{
	zip_fileinfo zfi;
	int size_read, i;
	int err = ZIP_OK;
	char buf[4086];

	const char* filenameinzip = out_zip;
	char* filepath = NULL;
	char* filename = NULL;

	zipFile zf = zipOpen(filenameinzip, APPEND_STATUS_CREATE);
	//char *test = "./test/test.jpg";

	for (i = 0; i < file_count; i++)
	{
		filename = getFileName(infiles[i]);
		printf("%s\n", filename);
		filepath = infiles[i];

		int ret = zipOpenNewFileInZip(zf,
			filename,
			&zfi,
			NULL, 0,
			NULL, 0,
			"my comment for this interior file",
			Z_DEFLATED,
			Z_DEFAULT_COMPRESSION
			);

		FILE * fin = fopen(filepath, "rb");
		if (fin == NULL)
		{
			err = ZIP_ERRNO;
			printf("error in opening %s for reading\n", filenameinzip);
			return 1;
		}

		do
		{

			err = ZIP_OK;
			size_read = (int)fread(buf, 1, sizeof(buf), fin);
			//printf("gogo : %d\n", size_read);

			if (size_read < sizeof(buf))
			{
				if (feof(fin) == 0)
				{
					printf("error in reading %s\n", filenameinzip);
					err = ZIP_ERRNO;
				}
			}

			if (size_read > 0)
			{
				err = zipWriteInFileInZip(zf, buf, size_read);
				if (err < 0)
				{
					printf("error in writing %s in the zipfile\n",
						filenameinzip);
				}

			}
		} while ((err == ZIP_OK) && (size_read > 0));

		fclose(fin);
		zipCloseFileInZip(zf);
	}


	zipClose(zf, "my comment for exterior file");

}

char * getFileName(char* path)
{
	char *filepath;
	char *pToken = NULL;
	char *pFilename = NULL;
	char *pSeparator = "/";
	
	filepath = malloc(strlen(path));
	strcpy(filepath, path);

	pToken = strtok(filepath, pSeparator);
	pFilename = pToken;

	while ((pToken = strtok(NULL, pSeparator)) != NULL)
	{
		pFilename = pToken;
	}
	
	return pFilename;
}

jpeg_container readjpeg(char *filename)
{
	jpeg_container info;
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;

	JSAMPROW row_pointer[1];

	FILE *infile = fopen(filename, "rb");
	unsigned long location = 0;
	int i = 0;

	if (!infile)
	{
		printf("Error opening jpeg file %s\n!", filename);
		return;
	}

	cinfo.err = jpeg_std_error(&jerr);

	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);

	jpeg_start_decompress(&cinfo);

	info.image = (unsigned char*)malloc(cinfo.output_width*cinfo.output_height*cinfo.num_components);
	info.dcinfo = cinfo;

	row_pointer[0] = (unsigned char *)malloc(cinfo.output_width*cinfo.num_components);

	while (cinfo.output_scanline < cinfo.image_height)
	{
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
		for (i = 0; i < cinfo.image_width*cinfo.num_components; i++)
		{
			info.image[location++] = row_pointer[0][i];
		}
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(row_pointer[0]);
	fclose(infile);

	return info;
}

int writejpeg(char *filename, jpeg_container container, secure_container scarr[])
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	int i, j = 0;

	JSAMPROW row_pointer[1];
	FILE *outfile = fopen(filename, "wb");
	FILE *secureFile = fopen("./secure.ise", "wb");

	if (!outfile)
	{
		printf("Error opening output jpeg file %s\n!", filename);
		return -1;
	}

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = container.dcinfo.image_width;
	cinfo.image_height = container.dcinfo.image_height;
	cinfo.input_components = container.dcinfo.num_components;
	cinfo.in_color_space = container.dcinfo.out_color_space;

	//1. set secure container


	jpeg_set_defaults(&cinfo);

	jpeg_start_compress(&cinfo, TRUE);

	//2. sampling image from secure container


	//3. encode sampling images


	//4. write sampling info on xml file


	//5. compress images with jpgx

	while (cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer[0] = &container.image[cinfo.next_scanline * cinfo.image_width * cinfo.input_components];

		//여기서 pos_x, pos_y겹치는곳 찾음
		for (j = 0; j < 2; j++)
		{
			secure_container sc = scarr[j];
			if (sc.pos_y < cinfo.next_scanline && sc.pos_y + sc.height > cinfo.next_scanline)
			{
				for (i = sc.pos_y * cinfo.input_components; i < (sc.pos_y + sc.width) * cinfo.input_components; i++)
				{
					row_pointer[0][i] = 0;
					//write 
				}
			}
		}

		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose(outfile);

	return 1;
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

//
//void compressJPGX(char *frompath, char *topath)
//{
//	char    *filename = NULL;
//	char    *gzfilename = NULL;
//
//	gzFile  zfp;
//	FILE    *fd;
//	int     n;
//	void*   buf;
//	int     lerrno;
//
//	filename = frompath;
//	gzfilename = (char *)malloc(strlen(topath)*sizeof(char));
//	buf = (void *)malloc(255);
//
//	//sprintf(gzfilename, "%s.jpgx", filename);
//	gzfilename = topath;
//
//	if ((fd = fopen(filename, "rb")) < 0)
//	{
//		printf("file open error\n");
//		return;
//	}
//
//	if ((zfp = gzopen(gzfilename, "wb")) == NULL)
//	{
//		printf("jpgx open error\n");
//		return;
//	}
//
//	while (0 < (n = fread((void*)buf, 1, 255, fd)))
//	{
//		printf("read cnt : %d\n", n);
//
//		printf("%s\n", gzerror(zfp, &lerrno));
//		if (gzwrite(zfp, buf, n) < 0)
//		{
//			printf("%s\n", gzerror(zfp, &lerrno));
//		}
//	}
//	gzclose(zfp);
//	fclose(fd);
//	printf("success");
//}

