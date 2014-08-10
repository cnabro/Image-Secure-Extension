#include "IseWrapperWP.h"

IseWrapperWP::JpgxDecompressContainer^ IseWrapperWP::ImageSecureExtention::getJpgxContainer(String^ filename, String^ key)
{
	/*std::wstring wstr_filename = msclr::interop::marshal_as<std::wstring>(filename);
	std::wstring wstr_key = msclr::interop::marshal_as<std::wstring>(key);

	char* path = ws2c(wstr_filename);
	char* skey = ws2c(wstr_key);

	jpgx_decompress_container container = get_jpgx_container(path, (char*)skey);


	JpgxDecompressContainer^ containerWrapper = ref new JpgxDecompressContainer(container.jdcinfo.image, container.jdcinfo.dcinfo.image_width, container.jdcinfo.dcinfo.image_height, container.jdcinfo.dcinfo.out_color_components, container.status);

	return containerWrapper;*/
	JpgxDecompressContainer^ containerWrapper;

	return containerWrapper;
}

IseWrapperWP::PngxDecompressContainer^ IseWrapperWP::ImageSecureExtention::getPngxContainer(String^ filename, String^ key)
{
	/*std::wstring wstr_filename = msclr::interop::marshal_as<std::wstring>(filename);
	std::wstring wstr_key = msclr::interop::marshal_as<std::wstring>(key);

	char* path = ws2c(wstr_filename);
	char* skey = ws2c(wstr_key);

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

	PngxDecompressContainer^ containerWrapper = gcnew PngxDecompressContainer(container.pdcinfo.image, width, height, input_components, container.status);

	return containerWrapper;*/
	PngxDecompressContainer^ containerWrapper;

	return containerWrapper;
}

void IseWrapperWP::ImageSecureExtention::makeJPGX(String^ filename, Windows::Foundation::Collections::IIterator<ImageSecureExtention^>^ scList, String^ key)
{
	/*std::wstring wstr_filename = msclr::interop::marshal_as<std::wstring>(filename);
	std::wstring wstr_key = msclr::interop::marshal_as<std::wstring>(key);

	secure_container **sc_array = (secure_container**)malloc(sizeof(secure_container*)*scList->Count);
	for (int i = 0; i < scList->Count; i++)
	{
		sc_array[i] = (secure_container*)malloc(sizeof(secure_container));
		sc_array[i]->height = scList[i]->getHeight();
		sc_array[i]->width = scList[i]->getWidth();
		sc_array[i]->pos_x = scList[i]->getPosX();
		sc_array[i]->pos_y = scList[i]->getPosY();
	}

	char* path = ws2c(wstr_filename);
	char* skey = ws2c(wstr_key);

	make_jpgx(path, sc_array, scList->Count, skey);

	free(sc_array);
	free(path);*/


	return;
}

void IseWrapperWP::ImageSecureExtention::makePNGX(String^ filename, Windows::Foundation::Collections::IIterator<ImageSecureExtention^>^ scList, String^ key)
{
	/*std::wstring wstr_filename = msclr::interop::marshal_as<std::wstring>(filename);
	std::wstring wstr_key = msclr::interop::marshal_as<std::wstring>(key);

	secure_container **sc_array = (secure_container**)malloc(sizeof(secure_container*)*scList->Count);
	for (int i = 0; i < scList->Count; i++)
	{
		sc_array[i] = (secure_container*)malloc(sizeof(secure_container));
		sc_array[i]->height = scList[i]->getHeight();
		sc_array[i]->width = scList[i]->getWidth();
		sc_array[i]->pos_x = scList[i]->getPosX();
		sc_array[i]->pos_y = scList[i]->getPosY();
	}

	char* path = ws2c(wstr_filename);
	char* skey = ws2c(wstr_key);

	make_pngx(path, sc_array, scList->Count, skey);

	free(sc_array);
	free(path);*/

	return;
}