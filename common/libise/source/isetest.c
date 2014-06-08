#include "isejpgx.h"
#include "iseutil.h"

void decodeJPGX()
{ 	
	int i,j,k,l,arr_cnt = 0;
	//unsigned long location = 0;
	secure_container **sc_array;

	make_decompress("E:/opensource/ise/source/library/lib-win/test/test.jpgx");
	jpeg_decompress_container container = read_jpeg_container("./test/.test/core.jpg");

	FILE *fp;
	mxml_node_t	*xml, *prop, *key, *items, *item = NULL;

	JSAMPROW row_pointer = NULL;

	fp = fopen("./test/.test/prop.xml", "r");
	xml = mxmlLoadFile(NULL, fp, MXML_TEXT_CALLBACK);

	prop = mxmlFindElement(xml, xml, "prop", NULL, NULL, MXML_DESCEND);
	//printf("prop : %s\n", prop->value);
	key = mxmlFindElement(prop, prop, "key", "enc", NULL, MXML_DESCEND);
	printf("key : %s\n", mxmlElementGetAttr(key,"enc"));

	items = mxmlFindElement(prop, prop, "items", NULL, NULL, MXML_DESCEND);
	arr_cnt = atoi(mxmlElementGetAttr(items, "count"));
	sc_array = (secure_container**)malloc(sizeof(secure_container*) * arr_cnt);

	for (item = mxmlFindElement(items, items, "item", NULL, NULL, MXML_DESCEND),i=0; item != NULL; item = mxmlFindElement(item, items, "item", NULL, NULL, MXML_DESCEND),i++)
	{
		char *decode_path = "./test/.test/decode.jpg";
		decode_file_des(str_concat(2, "./test/.test/", mxmlElementGetAttr(item, "name")), decode_path, des3_test_keys);
		jpeg_decompress_container jdc = read_jpeg_container(decode_path);
		JSAMPROW secure_row_pointer;

		sc_array[i] = (secure_container*)malloc(sizeof(secure_container));
		sc_array[i]->height = atoi(mxmlElementGetAttr(item, "height"));
		sc_array[i]->width = atoi(mxmlElementGetAttr(item, "width"));
		sc_array[i]->pos_x = atoi(mxmlElementGetAttr(item, "x"));
		sc_array[i]->pos_y = atoi(mxmlElementGetAttr(item, "y"));
		printf("item : %s\n", mxmlElementGetAttr(item, "name"));

		for (l = sc_array[i]->pos_y+1, k = 0; l < (int)(sc_array[i]->pos_y + sc_array[i]->height +1); l++, k++)
		{
			row_pointer = &container.image[l * container.dcinfo.image_width * container.dcinfo.output_components];
			secure_row_pointer = &jdc.image[k * jdc.dcinfo.image_width * jdc.dcinfo.output_components];

			for (j = 0; j < jdc.dcinfo.image_width*jdc.dcinfo.output_components; j++)
			{
				row_pointer[j + (sc_array[i]->pos_x)*jdc.dcinfo.output_components] = secure_row_pointer[j];
			}
		}
	}
	
	//test code
	char *filename = "./test/test1234.jpg";
	FILE *file = NULL;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	file = fopen(filename, "wb");

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, file);

	cinfo.image_width = container.dcinfo.image_width;
	cinfo.image_height = container.dcinfo.image_height;
	cinfo.input_components = container.dcinfo.num_components;
	cinfo.in_color_space = container.dcinfo.out_color_space;

	jpeg_set_defaults(&cinfo);
	jpeg_start_compress(&cinfo, TRUE);

	while (cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer = &container.image[cinfo.next_scanline * cinfo.image_width * cinfo.input_components];
		jpeg_write_scanlines(&cinfo, &row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose(file);

	//decode_file_des("./test/.test/item0.ise", "./test/.test/item0.jpg", des3_test_keys);
	//decode_file_des("./test/.test/item1.ise", "./test/.test/item1.jpg", des3_test_keys);
	
	//item0 = read_jpeg_container("./test/.test/item0.jpg");
	//item1 = read_jpeg_container("./test/.test/item0.jpg");
	//mxmlGetElement(items, "items");

	fclose(fp);
}

int main()
{
	int i, j = 0;

	char *infilename = "./test/test.jpg";
	jpeg_decompress_container container = read_jpeg_container(infilename);
	char *out_temp_folder = str_concat(3, get_current_path(infilename), ".", get_file_name(infilename));

	secure_container **scarr;
	scarr = (secure_container**)malloc(sizeof(secure_container*) * 2);
	scarr[0] = (secure_container*)malloc(sizeof(secure_container));
	scarr[1] = (secure_container*)malloc(sizeof(secure_container));
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

	_mkdir(out_temp_folder);
	
	write_jpgx(infilename, container, scarr, 2, des3_test_keys);
	

	printf("===============================\n\n", out_temp_folder);
	
	//decompress jpgx
	decodeJPGX();
	return 0;
}

