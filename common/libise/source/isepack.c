#include "isepack.h"

int make_compress(char *infiles[], int file_count, char *out_zip)
{
	zip_fileinfo zfi;
	int size_read, i, err, ret = ZIP_OK;
	char buf[4086];

	const char* filenameinzip = out_zip;
	char* filepath = NULL;
	char* filename = NULL;
	FILE * fin;

	zipFile zf = zipOpen(filenameinzip, APPEND_STATUS_CREATE);
	//char *test = "./test/test.jpg";

	for (i = 0; i < file_count; i++)
	{
		filename = getFileName(infiles[i]);
		printf("%s\n", filename);
		filepath = infiles[i];

		ret = zipOpenNewFileInZip(zf, filename, &zfi, NULL, 0, NULL, 0, "my comment for this interior file", Z_DEFLATED, Z_DEFAULT_COMPRESSION);

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


	zipClose(zf, "my comment for exterior file");

	return ZIP_OK;
}