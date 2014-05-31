#include "isepack.h"
#include "iseutil.h"

int make_compress(char **in_file_arr, int file_count, char *out_file_name, char* file_tag)
{
	zip_fileinfo zfi;
	int size_read, i, err, ret = ZIP_OK;
	char buf[4086];

	const char* filenameinzip = out_file_name;
	char* filepath = NULL;
	char* filename = NULL;
	FILE * fin;

	zipFile zf = zipOpen(filenameinzip, APPEND_STATUS_CREATE);

	for (i = 0; i < file_count; i++)
	{
		filename = get_file_name_ex(in_file_arr[i]);
		printf("make_compress : %s\n", filename);
		filepath = in_file_arr[i];

		ret = zipOpenNewFileInZip(zf, filename, &zfi, NULL, 0, NULL, 0, file_tag, Z_DEFLATED, Z_DEFAULT_COMPRESSION);

		fin = fopen(filepath, "rb");
		if (fin == NULL)
		{
			err = ZIP_ERRNO;
			printf("error in opening %s for reading\n", filenameinzip);
			return err;
		}

		do
		{
			err = ZIP_OK;
			size_read = (int)fread(buf, 1, sizeof(buf), fin);

			if (size_read < sizeof(buf))
			{
				if (feof(fin) == 0)
				{
					printf("error in reading %s\n", filenameinzip);
					err = ZIP_ERRNO;
				}
			}

			if (size_read > 0)
			{
				err = zipWriteInFileInZip(zf, buf, size_read);
				if (err < 0)
				{
					printf("error in writing %s in the zipfile\n", filenameinzip);
				}

			}
		} while ((err == ZIP_OK) && (size_read > 0));

		fclose(fin);
		zipCloseFileInZip(zf);
	}


	zipClose(zf, file_tag);

	return ZIP_OK;
}