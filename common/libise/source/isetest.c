#include "isejpgx.h"

static const unsigned char des3_test_keys[24] =
{
	0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
	0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01,
	0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23
};

int main()
{
	int i, j = 0;

	char *infilename = "./test/test.jpg";
	char *outfilename = "./test/.abcd/out.jpg";
	char *compress = "./test/out.jpg";
	char *test = "./test/test.jpg";
	secure_container scarr[2];
	jpeg_container container = read_jpeg_container(infilename);

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
	write_jpeg_with_secure_container(outfilename, container, scarr, 2);
	encode_file_des("./test/.abcd/out.jpg0", "./test/.abcd/out_sec.jpg", des3_test_keys);
	decode_file_des("./test/.abcd/out_sec.jpg", "./test/.abcd/out_dec.jpg", des3_test_keys);


	make_prop_xml(scarr, "./test/.abcd/out.xml", 0); //for test

	return 0;
}