#include "isedes.h"

#define DES3_KEY_LENGTH 24

int encode_file_des(char *in_file_path, char *out_file_path, char * key)
{
	FILE    *in_file;
	FILE    *out_file;
	int     n;
	void*   buf = (void *)malloc(8);
	int     lerrno;
	des3_context ctx3;

	if ((in_file = fopen(in_file_path, "rb")) < 0)
	{
		printf("file open error\n");
		return -1;
	}

	if ((out_file = fopen(out_file_path, "wb")) < 0)
	{
		printf("file open error\n");
		return -1;
	}

	des3_set3key_enc(&ctx3, key);

	while (0 < (n = fread((void*)buf, 1, 8, in_file)))
	{
		des3_crypt_ecb(&ctx3, buf, buf);
		fwrite(buf, 1, 8, out_file);
	}

	fclose(in_file);
	fclose(out_file);
	printf("encode_file_des: success\n");

	return 1;
}

int decode_file_des(char *in_file_path, char *out_file_path, char * key)
{
	FILE    *in_file;
	FILE    *out_file;
	int     n;
	void*   buf = (void *)malloc(8);
	int     lerrno;
	des3_context ctx3;

	if ((in_file = fopen(in_file_path, "rb")) < 0)
	{
		printf("file open error\n");
		return -1;
	}

	if ((out_file = fopen(out_file_path, "wb")) < 0)
	{
		printf("file open error\n");
		return -1;
	}

	des3_set3key_dec(&ctx3, key);

	while (0 < (n = fread((void*)buf, 1, 8, in_file)))
	{
		des3_crypt_ecb(&ctx3, buf, buf);
		fwrite(buf, 1, 8, out_file);
	}

	fclose(in_file);
	fclose(out_file);
	printf("decode_file_des: success\n");

	return 1;
}

char * make_des_key(char * key)
{
	char* deskey = malloc(DES3_KEY_LENGTH + 1);

	strncpy(deskey, (char*)padding_bit, DES3_KEY_LENGTH);
	strncpy(deskey, key, strlen(key));

	printf("key : %s\n\n", deskey);

	return deskey;
}