#include "iseutil.h"

char * get_file_name_ex(char* path)
{
	char *file_path;
	char *token = NULL;
	char *file_name = NULL;

	char *seperator = "/";

	file_path = (char*)malloc(strlen(path));
	strcpy(file_path, path);

	token = strtok(file_path, seperator);
	file_name = token;

	while ((token = strtok(NULL, seperator)) != NULL)
	{
		file_name = token;
	}

	return file_name;
}

char * get_file_name(char* path)
{
	char *file_path;
	char *token = NULL;
	char *file_name = NULL;
	
	char *path_seperator = "/";
	char *extention_seperator = ".";

	file_path = (char*)malloc(strlen(path));
	strcpy(file_path, path);

	token = strtok(file_path, path_seperator);
	file_name = token;

	while ((token = strtok(NULL, path_seperator)) != NULL)
	{
		file_name = token;
	}

	if ((token = strtok(file_name, extention_seperator)) != NULL)
	{
		file_name = token;
	}

	return file_name;
}

char * get_current_path(char* path)
{
	char *file_path;
	char *token = NULL;
	char *file_name = get_file_name_ex(path);

	char *current_path = str_replace_all(path, file_name, "\0");
	return current_path;
}

char * str_replace_all(char *s, const char *olds, const char *news)
{
	char *result, *sr;
	size_t i, count = 0;
	size_t oldlen = strlen(olds); if (oldlen < 1) return s;
	size_t newlen = strlen(news);


	if (newlen != oldlen)
	{
		for (i = 0; s[i] != '\0';)
		{
			if (memcmp(&s[i], olds, oldlen) == 0)
			{
				count++;
				i += oldlen;
			}
			else
			{
				i++;
			}
		}
	}
	else
	{
		i = strlen(s);
	}

	result = (char *)malloc(count * (newlen - oldlen) + i + 1);
	if (result == NULL) return NULL;

	sr = result;
	while (*s)
	{
		if (memcmp(s, olds, oldlen) == 0) 
		{
			memcpy(sr, news, newlen);
			sr += newlen;
			s += oldlen;
		}
		else
		{
			*sr++ = *s++;
		}
	}
	*sr = '\0';

	return result;
}

char* str_concat(int count, ...)
{
	va_list ap;
	int i;

	// Find required length to store merged string
	int len = 1; // room for NULL
	va_start(ap, count);
	for (i = 0; i<count; i++)
		len += strlen(va_arg(ap, char*));
	va_end(ap);

	// Allocate memory to concat strings
	char *merged = calloc(sizeof(char), len);
	int null_pos = 0;

	// Actually concatenate strings
	va_start(ap, count);
	for (i = 0; i<count; i++)
	{
		char *s = va_arg(ap, char*);
		strcpy(merged + null_pos, s);
		null_pos += strlen(s);
	}
	va_end(ap);

	return merged;
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



