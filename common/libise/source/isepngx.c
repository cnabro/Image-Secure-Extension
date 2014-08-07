#include "isepngx.h"
#include "iseutil.h"
#include "isepack.h"
#include "iseprop.h"

png_decompress_container read_png_container(char *filename)
{
	png_decompress_container info;
	png_structp png_ptr;
	png_infop info_ptr;
	int number_of_passes;
	png_bytep * row_pointers;
	char header[8];    // 8 is the maximum size that can be checked

	int x, y;

	int width, height;
	png_byte color_type;
	png_byte bit_depth;

	/* 
		open file and test for it being a png 
	*/
	FILE *infile = fopen(filename, "rb");
	if (!infile)
	{
		info.status = ISE_STATUS_ERROR_IVALID_FILE;
		fclose(infile);

#ifdef _DEBUG
		printf("file open error\n");
#endif

		return info;
	}
		
	fread(header, 1, 8, infile);
	if (png_sig_cmp(header, 0, 8))
	{
		info.status = ISE_STATUS_ERROR_UNPACKING;
		fclose(infile);

		return info;
	}


	/* 
		initialize stuff 
	*/
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if (!png_ptr)
	{
		info.status = ISE_STATUS_ERROR_UNPACKING;
		fclose(infile);

#ifdef _DEBUG
		printf("png_ptr error\n");
#endif

		return info;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		info.status = ISE_STATUS_ERROR_UNPACKING;
		fclose(infile);
#ifdef _DEBUG
		printf("info_ptr error\n");
#endif
		return info;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		info.status = ISE_STATUS_ERROR_UNPACKING;
		fclose(infile);
#ifdef _DEBUG
		printf("setjmp error\n");
#endif
		return info;
	}

	png_init_io(png_ptr, infile);
	png_set_sig_bytes(png_ptr, 8);

	png_read_info(png_ptr, info_ptr);

	width = png_get_image_width(png_ptr, info_ptr);
	height = png_get_image_height(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	number_of_passes = png_set_interlace_handling(png_ptr);
	png_read_update_info(png_ptr, info_ptr);


	/* 
		read file 
	*/
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		info.status = ISE_STATUS_ERROR_UNPACKING;
		fclose(infile);
#ifdef _DEBUG
		printf("read file error\n");
#endif
		return info;
	}

	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
	for (y = 0; y<height; y++)
	{ 
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
	}
		

	png_read_image(png_ptr, row_pointers);

	fclose(infile);

	info.png_ptr = png_ptr;
	info.info_ptr = info_ptr;
	info.image = row_pointers;
	info.status = ISE_STATUS_OK;
#ifdef _DEBUG
	printf("success read_png_container\n");
#endif

	return info;
}

pngx_compress_container write_pngx(char *filename, png_decompress_container container, secure_container **sc_array, int sc_arr_count, char *user_key)
{
	/*
		Load png info
	*/
	int width = png_get_image_width(container.png_ptr, container.info_ptr);
	int height = png_get_image_height(container.png_ptr, container.info_ptr);
	png_byte color_type = png_get_color_type(container.png_ptr, container.info_ptr);
	png_byte bit_depth = png_get_bit_depth(container.png_ptr, container.info_ptr);

	int input_components = 3;

	png_bytep *row_pointers;
	png_bytep secure_row_pointer;

	char *out_temp_folder = str_concat(3, get_current_path(filename), ".", get_file_name(filename));
	char *core_file_path = str_concat(2, out_temp_folder, "/core.png");
	FILE *core_file = fopen(core_file_path, "wb");

	FILE **sc_file = NULL;
	char **sc_file_path = NULL;
	char **sc_enc_file_path = NULL;

	char **pack_file_path = NULL;
	char *pngx_file_path = str_concat(3, get_current_path(filename), get_file_name(filename), ".pngx");
	char *prop_file_path = str_concat(2, out_temp_folder, "/prop.xml");

	char * key = make_des_key(user_key);

	int pack_file_count = 0;

	pngx_compress_container pngx_container; /* output pngx container */
	png_infop *png_item_info_ptr;
	png_structp *png_item_ptr;

	int i, j, k = 0;
	
	if (color_type == PNG_COLOR_TYPE_RGB)
	{
		input_components = 3;
	}
	else if (color_type == PNG_COLOR_TYPE_RGBA)
	{
		input_components = 4;
	}

	/*
		create file
	*/

	if (!core_file)
	{
#ifdef _DEBUG
		printf("create file error");
#endif
	}

	/*
		initialize stuff
	*/
	container.png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	container.info_ptr = png_create_info_struct(container.png_ptr);

	setjmp(png_jmpbuf(container.png_ptr));
	png_init_io(container.png_ptr, core_file);
	setjmp(png_jmpbuf(container.png_ptr));
	png_set_IHDR(container.png_ptr, container.info_ptr, width, height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(container.png_ptr, container.info_ptr);

	/*
		malloc secure item
	*/
	png_item_info_ptr = (png_infop*)malloc(sizeof(png_infop) * sc_arr_count);
	png_item_ptr = (png_structp*)malloc(sizeof(png_structp) * sc_arr_count);

	sc_file = (FILE**)malloc(sizeof(FILE*)* sc_arr_count);
	sc_file_path = (char **)malloc(sizeof(char*)*sc_arr_count);
	sc_enc_file_path = (char **)malloc(sizeof(char*)*sc_arr_count);

	row_pointers = container.image;


	for (i = 0; i < sc_arr_count; i++)
	{
		secure_container sc = *sc_array[i];

		char *out_file_name = "core.png";

		/*
			malloc secure item
		*/
		sc_file_path[i] = (char*)malloc(strlen(out_temp_folder) + 128);
		sc_enc_file_path[i] = (char*)malloc(strlen(out_temp_folder) + 128);
		//png_item_info_ptr[i] = (png_infop)malloc(sizeof(png_infop));
		//png_item_ptr[i] = (png_structp)malloc(sizeof(png_structp));

		/*
			set file path
		*/
		sprintf(sc_file_path[i], "%s/item%d.png", out_temp_folder, i);
		sprintf(sc_enc_file_path[i], "%s/item%d.ise", out_temp_folder, i);

		/*
			log
		*/
		printf("sc_file_path : %s\n", sc_file_path[i]);
		printf("sc_enc_file_path : %s\n", sc_enc_file_path[i]);
		printf("\n");

		sc_file[i] = fopen(sc_file_path[i], "wb");

		/*
			make png file
		*/
		png_item_ptr[i] = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		png_item_info_ptr[i] = png_create_info_struct(png_item_ptr[i]);

		setjmp(png_jmpbuf(png_item_ptr[i]));
		png_init_io(png_item_ptr[i], sc_file[i]);
		setjmp(png_jmpbuf(png_item_ptr[i]));

		png_set_IHDR(png_item_ptr[i], png_item_info_ptr[i], sc.width, sc.height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

		png_write_info(png_item_ptr[i], png_item_info_ptr[i]);
	}

	for (i = 0; i < height; i++)
	{
		png_byte* row = row_pointers[i];

		for (j = 0; j < sc_arr_count; j++)
		{
			secure_container sc = *sc_array[j];
			/*
				if secure container is matching, then copy to ise
			*/
			if (sc.pos_y <= i && sc.pos_y + sc.height > i)
			{
				secure_row_pointer = (png_bytep)malloc(input_components * width * sizeof(png_byte));

				for (k = sc.pos_x * input_components; k < (sc.pos_x + sc.width) * input_components; k++)
				{
					secure_row_pointer[k - (sc.pos_x * input_components)] = row[k];
					row[k] = 0;
				}

				png_write_row(png_item_ptr[j], secure_row_pointer);
				free(secure_row_pointer);
			}
		}
	}

	png_write_image(container.png_ptr, row_pointers);

	if (core_file != NULL) fclose(core_file);
	if (container.info_ptr != NULL) png_free_data(container.png_ptr, container.info_ptr, PNG_FREE_ALL, -1);
	if (container.png_ptr != NULL) png_destroy_write_struct(&container.png_ptr, (png_infopp)NULL);
	if (row_pointers != NULL) free(row_pointers);

	for (i = 0; i < sc_arr_count; i++)
	{
		if (sc_file[i] != NULL) fclose(sc_file[i]);
		if (png_item_info_ptr[i] != NULL) png_free_data(png_item_ptr[i], png_item_info_ptr[i], PNG_FREE_ALL, -1);
		if (png_item_ptr[i] != NULL) png_destroy_write_struct(&png_item_ptr[i], (png_infopp)NULL);

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

	/*
		make property file
	*/
	make_prop_xml(sc_array, sc_arr_count, prop_file_path, 0);

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

	if (make_compress(pack_file_path, pack_file_count, pngx_file_path, "jpgx") == ZIP_OK)
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

#if WIN32
	_rmdir(out_temp_folder);
#else
	rmdir(out_temp_folder);
#endif

	pngx_container.file_path = filename;
	pngx_container.status = ISE_STATUS_OK;
	pngx_container.sc_arr = sc_array;
	pngx_container.sc_cnt = sc_arr_count;

	return pngx_container;
}

pngx_decompress_container read_pngx_container(char* filename, char* user_key)
{
	pngx_decompress_container pngx_container;

	int i, j = 0;
	secure_container **sc_array;
	png_decompress_container container;

	prop_info_container prop;
	int output_components = 3;
	int width = 0;
	int height = 0;

	char * key = make_des_key(user_key);

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
		container = read_png_container(str_concat(2, out_temp_folder, "/core.png"));

		png_bytep * row_pointer = container.image;

		width = png_get_image_width(container.png_ptr, container.info_ptr);
		height = png_get_image_height(container.png_ptr, container.info_ptr);

		sc_array = prop.sc_arr;

		for (i = 0; i < prop.sc_count; i++)
		{
			printf("file name : %s\n", prop.file_name[i]);
			decode_file_des(str_concat(3, out_temp_folder, "/", prop.file_name[i]), decode_path, key);
			png_decompress_container pdc = read_png_container(decode_path);
			png_bytep * secure_row_pointer = pdc.image;

			if (pdc.status > 0)
			{
				int x, y = 0;

				for (y = 0; y < sc_array[i]->height; y++)//y cood
				{
					for (x = 0; x < sc_array[i]->width*output_components; x++)
					{
						/*
							combine buffers uinsg core.png and ise files
						*/
						row_pointer[sc_array[i]->pos_y + y][sc_array[i]->pos_x*output_components + x] = secure_row_pointer[y][x];
					}
				}
			}

			/*
				remove temp decode files
			*/
			remove(decode_path);
			remove(str_concat(3, out_temp_folder, "/", prop.file_name[i]));
		}

		remove(str_concat(2, out_temp_folder, "/core.png"));
		remove(str_concat(2, out_temp_folder, "/prop.xml"));
#if WIN32
		_rmdir(out_temp_folder);
#else
		rmdir(out_temp_folder);
#endif

		pngx_container.pdcinfo = container;
		pngx_container.sc_cnt = prop.sc_count;
		pngx_container.sc_arr = sc_array;
		pngx_container.status = ISE_STATUS_OK;

		return pngx_container;
	}

	pngx_container.status = ISE_STATUS_ERROR_UNPACKING;


	return pngx_container;
}