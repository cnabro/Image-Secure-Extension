#include "isejpgx.h"
#include "iseutil.h"
#include "isepack.h"
#include "iseprop.h"

jpeg_decompress_container read_jpeg_container(char *filename)
{
	jpeg_decompress_container info;
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

jpgx_compress_container write_jpgx(char *filename, jpeg_decompress_container container, secure_container **sc_array, int sc_arr_count, char *key)
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

	char **pack_file_path = NULL;
	char *jpgx_file_path = str_concat(3, get_current_path(filename), get_file_name(filename), ".jpgx");
	char *prop_file_path = str_concat(2, out_temp_folder, "/prop.xml");

	
	int pack_file_count = 0;

	jpgx_compress_container jpgx_container; /* output jpgx container */

	struct jpeg_compress_struct **secure_item_info;

	if (!core_file)
	{
		printf("Error opening output jpeg file %s\n!", filename);
		return;
	}

	/*
		deep copy decompress info
	*/
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, core_file);

	cinfo.image_width = container.dcinfo.image_width;
	cinfo.image_height = container.dcinfo.image_height;
	cinfo.input_components = container.dcinfo.num_components;
	cinfo.in_color_space = container.dcinfo.out_color_space;

	jpeg_set_defaults(&cinfo);
	jpeg_start_compress(&cinfo, TRUE);

	/*
		malloc secure item
	*/
	secure_item_info = (struct jpeg_compress_struct**)malloc(sizeof(struct jpeg_compress_struct*) * sc_arr_count);
	sc_file = (FILE**)malloc(sizeof(FILE*)* sc_arr_count);
	sc_file_path = (char **)malloc(sizeof(char*)*sc_arr_count);
	sc_enc_file_path = (char **)malloc(sizeof(char*)*sc_arr_count);

	for (i = 0; i < sc_arr_count; i++)
	{
		secure_container sc = *sc_array[i];
		
		char *out_file_name = "core.jpg";

		/*
			malloc secure item
		*/
		sc_file_path[i] = (char*)malloc(strlen(out_temp_folder) + 4 + 4);
		sc_enc_file_path[i] = (char*)malloc(strlen(out_temp_folder) + 4 + 4);
		secure_item_info[i] = (struct jpeg_compress_struct*)malloc(sizeof(struct jpeg_compress_struct));

		/*
			set file path
		*/
		sprintf(sc_file_path[i], "%s/item%d.jpg", out_temp_folder, i);
		sprintf(sc_enc_file_path[i], "%s/item%d.ise", out_temp_folder, i);
		
		/*
			log
		*/
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

	while (cinfo.next_scanline < cinfo.image_height) 
	{
		/*
			read jpeg row pointer
		*/
		row_pointer = &container.image[cinfo.next_scanline * cinfo.image_width * cinfo.input_components];

		/*
			find each container exists.
		*/
		for (j = 0; j < sc_arr_count; j++)
		{
			secure_container sc = *sc_array[j];

			/*
				if secure container is matching, then copy to ise
			*/
			if (sc.pos_y < cinfo.next_scanline && sc.pos_y + sc.height >= cinfo.next_scanline)
			{
				secure_rp = &container.image[cinfo.next_scanline * cinfo.image_width * cinfo.input_components + sc.pos_x * cinfo.input_components];
				jpeg_write_scanlines(secure_item_info[j], &secure_rp, 1);

				for (i = sc.pos_x * cinfo.input_components, k = 0; i < (sc.pos_x + sc.width) * cinfo.input_components; i++, k++)
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

		if (encode_file_des(sc_file_path[i], sc_enc_file_path[i], key) > 0)
		{
			printf("encode file  : %s\n", sc_file_path[i]);
			printf("\n");
		}
		else
		{
			printf("encode file failed: %s\n", sc_file_path[i]);
			printf("\n");
		}
	}

	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose(core_file);

	/*
		make property file
	*/
	make_prop_xml(sc_array, sc_arr_count, prop_file_path, 0); //for test
	
	/*
		packing files
	*/
	pack_file_count = sc_arr_count + 2;
	pack_file_path = (char **)malloc(sizeof(char*)*pack_file_count);

	pack_file_path[0] = core_file_path;
	pack_file_path[1] = prop_file_path; /* prop path */

	for (i = 0; i < sc_arr_count; i++)
	{
		pack_file_path[i + 2] = sc_enc_file_path[i];
		printf("pack_file_path[%d] : %s\n", (i + 2), pack_file_path[i + 2]);
	}

	if (make_compress(pack_file_path, pack_file_count, jpgx_file_path, "jpgx") == ZIP_OK)
	{
		printf("compress : success\n");
	}
	else
	{
		printf("compress : fail\n");
	}

	/*
		remove temp files
	*/
	for (i = 0; i < sc_arr_count; i++)
	{
		remove(sc_file_path[i]); /* item files */
	}

	for (i = 0; i < pack_file_count; i++)
	{
		remove(pack_file_path[i]); /* packing files */
	}

	_rmdir(out_temp_folder);

	/* 
		return jpgx container
	*/

	free(container.image);
	free(sc_file);

	jpgx_container.file_path = jpgx_file_path;
	jpgx_container.sc_arr = &secure_item_info;
	jpgx_container.sc_cnt = sc_arr_count;

	return jpgx_container;
}

jpgx_decompress_container read_jpgx_container(char* filename, char* key)
{
	jpgx_decompress_container jpgx_container;

	int i, j = 0;
	secure_container **sc_array;
	jpeg_decompress_container container;
	JSAMPROW row_pointer = NULL;
	prop_info_container prop;

	/*
		set file path
	*/
	char *out_temp_folder = str_concat(3, get_current_path(filename), ".", get_file_name(filename));
	char *decode_path = str_concat(2, out_temp_folder, "/.decode");

	if (make_decompress(filename) == UNZ_OK)
	{
		/*
			init struct prop_xml, jpeg_container
		*/
		prop = parse_prop_xml(str_concat(2, out_temp_folder, "/prop.xml"));
		container = read_jpeg_container(str_concat(2, out_temp_folder, "/core.jpg"));

		sc_array = prop.sc_arr;

		for (i = 0; i < prop.sc_count; i++)
		{
			int core_pos = 0;
			int sc_pos = 0;

			printf("file name : %s\n", prop.file_name[i]);
			decode_file_des(str_concat(3, out_temp_folder, "/", prop.file_name[i]), decode_path, key);
			jpeg_decompress_container jdc = read_jpeg_container(decode_path);
			JSAMPROW secure_row_pointer;

			for (core_pos = sc_array[i]->pos_y + 1, sc_pos = 0; core_pos < (int)(sc_array[i]->pos_y + sc_array[i]->height + 1); core_pos++, sc_pos++)
			{
				row_pointer = &container.image[core_pos * container.dcinfo.image_width * container.dcinfo.output_components];
				secure_row_pointer = &jdc.image[sc_pos * jdc.dcinfo.image_width * jdc.dcinfo.output_components];

				for (j = 0; j < jdc.dcinfo.image_width*jdc.dcinfo.output_components; j++)
				{
					/*
						combine buffers uinsg core.jpg and ise files
					*/
					row_pointer[j + (sc_array[i]->pos_x)*jdc.dcinfo.output_components] = secure_row_pointer[j];
				}
			}

			/*
				remove temp decode files
			*/
			remove(decode_path);
			remove(str_concat(3, out_temp_folder, "/", prop.file_name[i]));
		}

		remove(str_concat(2, out_temp_folder, "/core.jpg"));
		remove(str_concat(2, out_temp_folder, "/prop.xml"));
		_rmdir(out_temp_folder);
	
		/*
			test code
		
		char *out = "./test/test1234.jpg";
		FILE *file = NULL;
		struct jpeg_compress_struct cinfo;
		struct jpeg_error_mgr jerr;

		file = fopen(out, "wb");

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
		*/
		jpgx_container.jdcinfo = container;
		jpgx_container.sc_cnt = prop.sc_count;
		jpgx_container.sc_arr = sc_array;
		jpgx_container.status = ISE_STATUS_OK;

		return jpgx_container;
	}

	jpgx_container.status = ISE_STATUS_ERROR_UNPACKING;
	

	return jpgx_container;
}