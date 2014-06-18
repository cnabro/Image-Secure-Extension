#include "isejpgx.h"
#include "iseutil.h"

int main()
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

	scarr[0]->height = 100;
	scarr[0]->width = 100;
	scarr[0]->type = ST_NORMAL;
	scarr[0]->pos_x = 200;
	scarr[0]->pos_y = 200;

	scarr[1]->height = 100;
	scarr[1]->width = 300;
	scarr[1]->type = ST_NORMAL;
	scarr[1]->pos_x = 10;
	scarr[1]->pos_y = 50;

	scarr[2]->height = 300;
	scarr[2]->width = 300;
	scarr[2]->type = ST_NORMAL;
	scarr[2]->pos_x = 100;
	scarr[2]->pos_y = 300;

	_mkdir(out_temp_folder);
	
	write_jpgx(infilename, container, scarr, 3, "test");
	

	printf("===============================\n\n", out_temp_folder);
	
	//decompress jpgx
	read_jpgx_container("./test/test.jpgx", "test");
	return 0;
}

