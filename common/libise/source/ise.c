#include "ise.h"

int main()
{
	char *infilename = "./test/test.jpg";
	char *outfilename = "./test/out.jpg";
	char *compress = "./test/out.jpg";
	char *test = "./test/test.txt";

	//jpeg_container container = readjpeg(infilename);
	//writejpeg(outfilename, container);
	compressJPGX(infilename, "./test/out.jpgx");
	return 0;
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
		for (i = 0; i  <cinfo.image_width*cinfo.num_components; i++)
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

int writejpeg(char *filename, jpeg_container container)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	secure_container scarr[2];
	int i,j =0;

	JSAMPROW row_pointer[1];
	FILE *outfile = fopen(filename, "rb");


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

	jpeg_set_defaults(&cinfo);

	jpeg_start_compress(&cinfo, TRUE);

	//2. sampling image from secure container


	//3. encode sampling images


	//4. write sampling info on xml file


	//5. compress images with jpgx

	while (cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer[0] = &container.image[cinfo.next_scanline * cinfo.image_width *  cinfo.input_components];

		//여기서 pos_x, pos_y겹치는곳 찾음
		for( j = 0 ; j < 2 ; j++)
		{
			secure_container sc = scarr[j];
			if(sc.pos_y < cinfo.next_scanline && sc.pos_y + sc.height > cinfo.next_scanline)
			{
				for(i = sc.pos_y * cinfo.input_components ; i < (sc.pos_y + sc.width) * cinfo.input_components ; i++)
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

void compressJPGX(char *frompath, char *topath)
{
    char    *filename   = NULL;
    char    *gzfilename = NULL;

    gzFile  zfp;
    FILE    *fd;
    int     n;
    void*   buf;
    int     lerrno;

    filename = frompath;
    gzfilename = (char *)malloc(strlen(topath)*sizeof(char));
	buf = (void *)malloc(255);

    //sprintf(gzfilename, "%s.jpgx", filename);
	gzfilename = topath;

	if ((fd = fopen(filename, "rb")) < 0)
	{
        printf("file open error\n");
        return;  
    }
    
    if ((zfp = gzopen(gzfilename, "wb")) == NULL)
    {
		printf("jpgx open error\n");
        return;
    }

	while(0 < (n = fread((void*)buf, 1, 255, fd)))
    {
		printf("read cnt : %d\n" , n);
		
		printf("%s\n",gzerror(zfp, &lerrno));
        if (gzwrite(zfp, buf, n) < 0)
        {
            printf("%s\n",gzerror(zfp, &lerrno));
        }
    }
    gzclose(zfp);
	fclose(fd);
    printf("success");
}
