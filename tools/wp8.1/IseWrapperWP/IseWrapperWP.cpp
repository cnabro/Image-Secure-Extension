#include "IseWrapperWP.h"

IseWrapperWP::JpgxDecompressContainer^ IseWrapperWP::ImageSecureExtention::getJpgxContainer(String^ filename, String^ key)
{
	char* path = (char*)malloc(sizeof(char)*filename->Length());
	char* skey = (char*)malloc(sizeof(char)*filename->Length());

	std::wcstombs(path, filename->Data(), strlen(path));
	std::wcstombs(skey, key->Data(), strlen(path));

	jpgx_decompress_container container = get_jpgx_container(path, (char*)skey);

	JpgxDecompressContainer^ containerWrapper = ref new JpgxDecompressContainer(container.jdcinfo.dcinfo.image_width, container.jdcinfo.dcinfo.image_height, container.jdcinfo.dcinfo.out_color_components, container.status);
	containerWrapper->setImageSource(container.jdcinfo.image);

	return containerWrapper;
}

IseWrapperWP::PngxDecompressContainer^ IseWrapperWP::ImageSecureExtention::getPngxContainer(String^ filename, String^ key)
{
	char* path = (char*)malloc(sizeof(char)*filename->Length());
	char* skey = (char*)malloc(sizeof(char)*filename->Length());

	std::wcstombs(path, filename->Data(), strlen(path));
	std::wcstombs(skey, key->Data(), strlen(path));

	pngx_decompress_container container = get_pngx_container(path, (char*)skey);

	int width = png_get_image_width(container.pdcinfo.png_ptr, container.pdcinfo.info_ptr);
	int height = png_get_image_height(container.pdcinfo.png_ptr, container.pdcinfo.info_ptr);
	png_byte color_type = png_get_color_type(container.pdcinfo.png_ptr, container.pdcinfo.info_ptr);

	int input_components = 3;

	if (color_type == PNG_COLOR_TYPE_RGB)
	{
		input_components = 3;
	}
	else if (color_type == PNG_COLOR_TYPE_RGBA)
	{
		input_components = 4;
	}

	PngxDecompressContainer^ containerWrapper = ref new PngxDecompressContainer(width, height, input_components, container.status);
	containerWrapper->setImageSource(container.pdcinfo.image);

	return containerWrapper;
}

void IseWrapperWP::ImageSecureExtention::makeJPGX(String^ filename, Windows::Foundation::Collections::IVector<SecureContainer^>^ scList, String^ key)
{
	secure_container **sc_array = (secure_container**)malloc(sizeof(secure_container*)*scList->Size);
	for (int i = 0; i < scList->Size; i++)
	{
		sc_array[i] = (secure_container*)malloc(sizeof(secure_container));
		sc_array[i]->height = scList->GetAt(i)->getHeight();
		sc_array[i]->width = scList->GetAt(i)->getWidth();
		sc_array[i]->pos_x = scList->GetAt(i)->getPosX();
		sc_array[i]->pos_y = scList->GetAt(i)->getPosY();
	}

	char* path = (char*)malloc(sizeof(char)*filename->Length());
	char* skey = (char*)malloc(sizeof(char)*filename->Length());

	std::wcstombs(path, filename->Data(), strlen(path));
	std::wcstombs(skey, key->Data(), strlen(path));

	make_jpgx(path, sc_array, scList->Size, skey);

	free(sc_array);
	free(path);

	return;
}

void IseWrapperWP::ImageSecureExtention::makePNGX(String^ filename, Windows::Foundation::Collections::IVector<SecureContainer^>^ scList, String^ key)
{
	secure_container **sc_array = (secure_container**)malloc(sizeof(secure_container*)*scList->Size);
	for (int i = 0; i < scList->Size; i++)
	{
		sc_array[i] = (secure_container*)malloc(sizeof(secure_container));
		sc_array[i]->height = scList->GetAt(i)->getHeight();
		sc_array[i]->width = scList->GetAt(i)->getWidth();
		sc_array[i]->pos_x = scList->GetAt(i)->getPosX();
		sc_array[i]->pos_y = scList->GetAt(i)->getPosY();
	}

	char* path = (char*)malloc(sizeof(char)*filename->Length());
	char* skey = (char*)malloc(sizeof(char)*filename->Length());

	std::wcstombs(path, filename->Data(), strlen(path));
	std::wcstombs(skey, key->Data(), strlen(path));

	make_pngx(path, sc_array, scList->Size, skey);

	free(sc_array);
	free(path);
	free(skey);
	return;
}