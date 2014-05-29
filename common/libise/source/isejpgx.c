#include "isejpgx.h"
#include "iseutil.h"

jpeg_container read_jpeg_container(char *filename)
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

int write_jpeg_with_secure_container(char *filename, jpeg_container container, secure_container sc_array[], int sc_arr_count, char *key)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	int i, j, k = 0;
	des3_context ctx3;

	JSAMPROW row_pointer = NULL;
	JSAMPROW secure_rp = NULL;

	char *out_temp_folder = str_concat(3, get_current_path(filename), ".", get_file_name(filename));
	char *core_file_path = str_concat(2, out_temp_folder, "/core.jpg");
	FILE *core_file = fopen(core_file_path, "wb");

	FILE **sc_file = NULL;
	char **sc_file_path = NULL;
	char **sc_enc_file_path = NULL;

	struct jpeg_compress_struct **secure_item_info;

	if (!core_file)
	{
		printf("Error opening output jpeg file %s\n!", filename);
		return -1;
	}

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, core_file);

	cinfo.image_width = container.dcinfo.image_width;
	cinfo.image_height = container.dcinfo.image_height;
	cinfo.input_components = container.dcinfo.num_components;
	cinfo.in_color_space = container.dcinfo.out_color_space;

	jpeg_set_defaults(&cinfo);
	jpeg_start_compress(&cinfo, TRUE);

	//secure container 갯수 만큼 jpeg dcinfo 생성함
	secure_item_info = (struct jpeg_compress_struct**)malloc(sizeof(struct jpeg_compress_struct*) * sc_arr_count);
	sc_file = (FILE**)malloc(sizeof(FILE*)* sc_arr_count);
	sc_file_path = (char **)malloc(sizeof(char*)*sc_arr_count);
	sc_enc_file_path = (char **)malloc(sizeof(char*)*sc_arr_count);

	for (i = 0; i < sc_arr_count; i++)
	{
		secure_container sc = sc_array[i];
		
		char *out_file_name = "core.jpg";
		sc_file_path[i] = (char*)malloc(strlen(out_temp_folder) + 4 + 4);
		sc_enc_file_path[i] = (char*)malloc(strlen(out_temp_folder) + 4 + 4);
		sprintf(sc_file_path[i], "%s/item%d.jpg", out_temp_folder, i);
		sprintf(sc_enc_file_path[i], "%s/item%d.ise", out_temp_folder, i);
		
		secure_item_info[i] = (struct jpeg_compress_struct*)malloc(sizeof(struct jpeg_compress_struct));

		printf("sc_file_path : %s\n", sc_file_path[i]);
		printf("sc_enc_file_path : %s\n", sc_enc_file_path[i]);
		printf("\n");
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

	for (i = 0; i < sc_arr_count; i++)
	{
		jpeg_finish_compress(secure_item_info[i]);
		jpeg_destroy_compress(secure_item_info[i]);

		fclose(sc_file[i]);

		if (encode_file_des(sc_file_path[i], sc_enc_file_path[i], key) > 0 && remove(sc_file_path[i]) >= 0)
		{
			printf("remove file success: %s\n", sc_file_path[i]);
			printf("\n");
		}
		else
		{
			printf("remove file failed: %s\n", sc_file_path[i]);
			printf("\n");
		}
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);

	free(container.image);
	free(sc_file);

	fclose(core_file);

	return 1;
}