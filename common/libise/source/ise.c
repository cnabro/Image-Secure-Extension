#include "ise.h"
static const unsigned char des3_test_buf[8] = "abcdefg\0";

static const unsigned char des3_test_keys[24] =
{
	0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
	0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01,
	0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23
};

static const unsigned char des3_test_ecb_enc[3][8] =
{
	{ 0x6A, 0x2A, 0x19, 0xF4, 0x1E, 0xCA, 0x85, 0x4B },
	{ 0x03, 0xE6, 0x9F, 0x5B, 0xFA, 0x58, 0xEB, 0x42 },
	{ 0xDD, 0x17, 0xE8, 0xB8, 0xB4, 0x37, 0xD2, 0x32 }
};

int main()
{
	int i, j = 0;

	char *infilename = "./test/test.jpg";
	char *outfilename = "./test/.abcd/out.jpg";
	char *compress = "./test/out.jpg";
	char *test = "./test/test.jpg";
	secure_container scarr[2];
	jpeg_container container = readjpeg(infilename);

	/*des*/
	des3_context ctx3;
	unsigned char key[24];
	unsigned char buf[8];

	//test

	/*jpeg croping test*/
	char *infiles[] = { "./test/newsy.jpg", "./test/out.jpg" };

	scarr[0].height = 500;
	scarr[0].width = 500;
	scarr[0].type = ST_NORMAL;
	scarr[0].pos_x = 500;
	scarr[0].pos_y = 500;

	scarr[1].height = 400;
	scarr[1].width = 700;
	scarr[1].type = ST_NORMAL;
	scarr[1].pos_x = 10;
	scarr[1].pos_y = 50;

	_mkdir("./test/.abcd/");
	writejpeg(outfilename, container, scarr, 2);

	/*des test*/
	memcpy(buf, des3_test_buf, 8);

	des3_set3key_enc(&ctx3, des3_test_keys);

	printf("ori : %s\n\n\n", buf);

	des3_crypt_ecb(&ctx3, buf, buf);

	printf("enc : %s\n\n\n",buf);

	des3_set3key_dec(&ctx3, des3_test_keys);
	
	des3_crypt_ecb(&ctx3, buf, buf);

	printf("dec : %s\n\n\n", buf);
	/*for (j = 0; j < 10000; j++)
	{
		
	}*/

	//memcmp(buf, des3_test_ecb_enc[u], 8) != 0 );

	return 0;
}

int compressFiles(char *infiles[], int file_count, char *out_zip)
{
	zip_fileinfo zfi;
	int size_read, i, err, ret = ZIP_OK;
	char buf[4086];

	const char* filenameinzip = out_zip;
	char* filepath = NULL;
	char* filename = NULL;
	FILE * fin;

	zipFile zf = zipOpen(filenameinzip, APPEND_STATUS_CREATE);
	//char *test = "./test/test.jpg";

	for (i = 0; i < file_count; i++)
	{
		filename = getFileName(infiles[i]);
		printf("%s\n", filename);
		filepath = infiles[i];

		ret = zipOpenNewFileInZip(zf, filename, &zfi, NULL, 0, NULL, 0, "my comment for this interior file", Z_DEFLATED, Z_DEFAULT_COMPRESSION);

		fin = fopen(filepath, "rb");
		if (fin == NULL)
		{
			err = ZIP_ERRNO;
			printf("error in opening %s for reading\n", filenameinzip);
			return err;
		}

		do
		{
			err = ZIP_OK;
			size_read = (int)fread(buf, 1, sizeof(buf), fin);

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
					printf("error in writing %s in the zipfile\n", filenameinzip);
				}

			}
		} while ((err == ZIP_OK) && (size_read > 0));

		fclose(fin);
		zipCloseFileInZip(zf);
	}


	zipClose(zf, "my comment for exterior file");

	return ZIP_OK;
}

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

int writejpeg(char *filename, jpeg_container container, secure_container sc_array[], int sc_arr_count)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	int i, j, k = 0;

	JSAMPROW row_pointer = NULL;
	JSAMPROW secure_rp = NULL;

	FILE *outfile = fopen(filename, "wb");

	FILE **sc_file = NULL;
	char **sc_file_path = NULL;

	struct jpeg_compress_struct **secure_item_info;

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

	jpeg_set_defaults(&cinfo);
	jpeg_start_compress(&cinfo, TRUE);

	//secure container 갯수 만큼 jpeg dcinfo 생성함
	secure_item_info = (struct jpeg_compress_struct**)malloc(sizeof(struct jpeg_compress_struct*) * sc_arr_count);
	sc_file = (FILE**)malloc(sizeof(FILE*) * sc_arr_count);
	sc_file_path = (char **)malloc(sizeof(char*)*sc_arr_count);


	for (i = 0; i < sc_arr_count; i++)
	{
		secure_container sc = sc_array[i];

		sc_file_path[i] = (char *)malloc(strlen(filename) + 30);
		secure_item_info[i] = (struct jpeg_compress_struct*)malloc(sizeof(struct jpeg_compress_struct));


		sprintf(sc_file_path[i], "%s%d", filename, i);
		printf("sc_file_path : %s\n", sc_file_path[i]);

		sc_file[i] = fopen(sc_file_path[i], "wb");

		secure_item_info[i]->err = jpeg_std_error(&jerr);

		jpeg_create_compress(secure_item_info[i]);
		jpeg_stdio_dest(secure_item_info[i], sc_file[i]);

		secure_item_info[i]->image_width = sc.width;
		secure_item_info[i]->image_height = sc.height;
		secure_item_info[i]->input_components = container.dcinfo.num_components;
		secure_item_info[i]->in_color_space = container.dcinfo.out_color_space;

		jpeg_set_defaults(secure_item_info[i]);
		jpeg_start_compress(secure_item_info[i], TRUE);
	}


	//secure_rp = (JSAMPROW**)malloc(sizeof(JSAMPROW*) * 2000);

	while (cinfo.next_scanline < cinfo.image_height) //한줄씩 읽음
	{
		row_pointer = &container.image[cinfo.next_scanline * cinfo.image_width * cinfo.input_components];

		//여기서 pos_x, pos_y겹치는곳 찾음
		for (j = 0; j < sc_arr_count; j++)
		{
			secure_container sc = sc_array[j];

			if (sc.pos_y < cinfo.next_scanline && sc.pos_y + sc.height >= cinfo.next_scanline)
			{
				secure_rp = &container.image[cinfo.next_scanline * cinfo.image_width * cinfo.input_components + sc.pos_y * cinfo.input_components];
				jpeg_write_scanlines(secure_item_info[j], &secure_rp, 1);

				for (i = sc.pos_y * cinfo.input_components, k = 0; i < (sc.pos_y + sc.width) * cinfo.input_components; i++, k++)
				{
					row_pointer[i] = 0;
				}
			}
		}

		jpeg_write_scanlines(&cinfo, &row_pointer, 1);
	}

	//fclose(sc_file);

	for (i = 0; i < sc_arr_count; i++)
	{
		jpeg_finish_compress(secure_item_info[i]);
		jpeg_destroy_compress(secure_item_info[i]);
		fclose(sc_file[i]);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	free(container.image);
	free(sc_file);

	fclose(outfile);

	return 1;
}

void compress_encode_stream(char * path)
{

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


