#include "isejpgx.h"
#include "isepngx.h"
#include "iseutil.h"

#if _DEBUG
void test_jpgx();
void test_pngx();

int main()
{
	//test_jpgx();
	test_pngx();

	return 0;
}

void test_pngx()
{
	char *infilename = "./test/test.png";
	png_decompress_container container = read_png_container(infilename);
	char *out_temp_folder = str_concat(3, get_current_path(infilename), ".", get_file_name(infilename));

	secure_container **scarr;
	scarr = (secure_container**)malloc(sizeof(secure_container*) * 3);
	scarr[0] = (secure_container*)malloc(sizeof(secure_container));
	scarr[1] = (secure_container*)malloc(sizeof(secure_container));
	scarr[2] = (secure_container*)malloc(sizeof(secure_container));
	
	/*
		des
	*/
	des3_context ctx3;
	unsigned char key[24];
	unsigned char buf[8];

	printf("file name with extention : %s\n", get_file_name_ex(infilename));
	printf("file name without extention : %s\n", get_file_name(infilename));
	printf("file working directory : %s\n", get_current_path(infilename));
	printf("temp directory : %s\n\n", out_temp_folder);

	scarr[0]->height = 100;
	scarr[0]->width = 100;
	scarr[0]->type = ST_NORMAL;
	scarr[0]->pos_x = 0;
	scarr[0]->pos_y = 0;

	scarr[1]->height = 10;
	scarr[1]->width = 10;
	scarr[1]->type = ST_NORMAL;
	scarr[1]->pos_x = 200;
	scarr[1]->pos_y = 200;
#if WIN32
	_mkdir(out_temp_folder);
#else
	mkdir(out_temp_folder);
#endif
	write_pngx(infilename, container, scarr, 2, "test");
	printf("===============================\n\n", out_temp_folder);

	//decompress pngx
	pngx_decompress_container pdc = read_pngx_container("./test/test.pngx", "test");
}

void test_jpgx()
{
	int i, j = 0;

	char *infilename = "./test/test.jpg";
	jpeg_decompress_container container = read_jpeg_container(infilename);
	char *out_temp_folder = str_concat(3, get_current_path(infilename), ".", get_file_name(infilename));

	secure_container **scarr;
	scarr = (secure_container**)malloc(sizeof(secure_container*) * 3);
	scarr[0] = (secure_container*)malloc(sizeof(secure_container));
	scarr[1] = (secure_container*)malloc(sizeof(secure_container));
	scarr[2] = (secure_container*)malloc(sizeof(secure_container));

	/*
	des
	*/
	des3_context ctx3;
	unsigned char key[24];
	unsigned char buf[8];

	//test

	/*
		jpeg crozping test
	*/


	printf("file name with extention : %s\n", get_file_name_ex(infilename));
	printf("file name without extention : %s\n", get_file_name(infilename));
	printf("file working directory : %s\n", get_current_path(infilename));
	printf("temp directory : %s\n\n", out_temp_folder);

	scarr[0]->height = 720;
	scarr[0]->width = 960;
	scarr[0]->type = ST_NORMAL;
	scarr[0]->pos_x = 0;
	scarr[0]->pos_y = 0;
#if WIN32
	_mkdir(out_temp_folder);
#else
	mkdir(out_temp_folder);
#endif
	//while (1)
	{
		write_jpgx(infilename, container, scarr, 1, "test");


		printf("===============================\n\n", out_temp_folder);

		//decompress jpgx
		read_jpgx_container("./test/test.jpgx", "test");
	}
}
#endif