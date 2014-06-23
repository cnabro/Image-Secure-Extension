#include "isepack.h"
#include "iseutil.h"

#define MAX_FILENAME 512
#define READ_SIZE 4028

int make_compress(char **in_file_arr, int file_count, char *out_file_name, char* file_tag)
{
	zip_fileinfo zfi;
	int size_read, i, err, ret = ZIP_OK;
	char buf[READ_SIZE];

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

int make_decompress(char *filepath)
{
	unzFile *zipfile = unzOpen(filepath);
	unz_global_info global_info;
	int i = 0;
	char buf[4086];
	char *out_temp_folder = str_concat(3, get_current_path(filepath), ".", get_file_name(filepath));

	if (zipfile == NULL)
	{
		printf("%s: not found\n");
		return -1;
	}
	
	if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
	{
		printf("could not read file global info\n");
		unzClose(zipfile);
		return -1;
	}

	for (i = 0; i < global_info.number_entry; ++i)
	{
		// Get info about current file.
		unz_file_info file_info;
		char filename[MAX_FILENAME];

		if (unzGetCurrentFileInfo(
			zipfile,
			&file_info,
			filename,
			MAX_FILENAME,
			NULL, 0, NULL, 0) != UNZ_OK)
		{
			printf("could not read file info\n");
			unzClose(zipfile);
			return -1;
		}

		_mkdir(out_temp_folder);

		// Entry is a file, so extract it.
		printf("file:%s\n", filename);
		if (unzOpenCurrentFile(zipfile) != UNZ_OK)
		{
			printf("could not open file\n");
			unzClose(zipfile);
			return -1;
		}

		FILE *out = fopen(str_concat(3, out_temp_folder , "/", filename), "wb");
		if (out == NULL)
		{
			printf("could not open destination file\n");
			unzCloseCurrentFile(zipfile);
			unzClose(zipfile);
			return -1;
		}

		int error = UNZ_OK;
		do
		{
			error = unzReadCurrentFile(zipfile, buf, sizeof(buf));
			if (error < 0)
			{
				printf("error %d\n", error);
				unzCloseCurrentFile(zipfile);
				unzClose(zipfile);
				return -1;
			}

			// Write data to file.
			if (error > 0)
			{
				fwrite(buf, error, 1, out);
			}
		} while (error > 0);

		fclose(out);

		unzCloseCurrentFile(zipfile);

		if ((i + 1) < global_info.number_entry)
		{
			if (unzGoToNextFile(zipfile) != UNZ_OK)
			{
				printf("cound not read next file\n");
				unzClose(zipfile);
				return -1;
			}
		}
	}

	unzClose(zipfile);

}