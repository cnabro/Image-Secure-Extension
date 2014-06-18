#include "stdafx.h"

#include "IseWrapper.h"


IseWrapper::JpgxDecompressContainer^ IseWrapper::ImageSecureExtention::getJpgxContainer(System::String^ filename, System::String^ key)
{
	std::wstring wstr_filename = msclr::interop::marshal_as<std::wstring>(filename);
	std::wstring wstr_key = msclr::interop::marshal_as<std::wstring>(key);

	char* path = ws2c(wstr_filename);
	char* skey = ws2c(wstr_key);

	jpgx_decompress_container container = get_jpgx_container(path, (char*)skey);
	
	
	JpgxDecompressContainer^ containerWrapper = gcnew JpgxDecompressContainer(container.jdcinfo.image, container.jdcinfo.dcinfo.image_width, container.jdcinfo.dcinfo.image_height, container.jdcinfo.dcinfo.out_color_components, container.status);
	printf("test getSecureJpegContainer : %d %d", container.jdcinfo.dcinfo.image_width, container.jdcinfo.dcinfo.image_height);

	return containerWrapper;
}

IseWrapper::PngxDecompressContainer^ IseWrapper::ImageSecureExtention::getPngxContainer(System::String^ filename, System::String^ key)
{
	std::wstring wstr_filename = msclr::interop::marshal_as<std::wstring>(filename);
	std::wstring wstr_key = msclr::interop::marshal_as<std::wstring>(key);


	printf("test getSecurePngContainer : %s %s", wstr_filename.c_str(), wstr_key.c_str());
	//return get_secure_png_container(filename, key);

	return gcnew PngxDecompressContainer(); // test code
}

void IseWrapper::ImageSecureExtention::makeJPGX(System::String^ filename, System::Collections::Generic::List<SecureContainer^>^ scList, System::String^ key)
{
	std::wstring wstr_filename = msclr::interop::marshal_as<std::wstring>(filename);
	std::wstring wstr_key = msclr::interop::marshal_as<std::wstring>(key);

	secure_container **sc_array = (secure_container**)malloc(sizeof(secure_container*)*scList->Count);
	for (int i = 0; i < scList->Count; i++)
	{
		sc_array[i] = (secure_container*)malloc(sizeof(sc_array));
		sc_array[i]->height = scList[i]->getHeight();
		sc_array[i]->width = scList[i]->getWidth();
		sc_array[i]->pos_x = scList[i]->getPosX();
		sc_array[i]->pos_y = scList[i]->getPosY();
	}

	char* path = ws2c(wstr_filename);
	char* skey = ws2c(wstr_key);

	make_jpgx(path, sc_array, scList->Count, (char*)skey);

	free(sc_array);
	free(path);
}

void IseWrapper::ImageSecureExtention::makePNGX(System::String^ filename, System::Collections::Generic::List<SecureContainer^>^ scList, System::String^ key)
{
	std::wstring wstr_filename = msclr::interop::marshal_as<std::wstring>(filename);
	std::wstring wstr_key = msclr::interop::marshal_as<std::wstring>(key);

	secure_container **sc_array = (secure_container**)malloc(sizeof(secure_container*)*scList->Count);
	for (int i = 0; i < scList->Count; i++)
	{
		sc_array[i] = (secure_container*)malloc(sizeof(sc_array));
		sc_array[i]->height = scList[i]->getHeight();
		sc_array[i]->width = scList[i]->getWidth();
		sc_array[i]->pos_x = scList[i]->getPosX();
		sc_array[i]->pos_y = scList[i]->getPosY();
	}
		
	//make_jpgx(WCharToChar(wstr_filename.c_str()), sc_array, scList->Count, (char*)des3_test_keys);
	make_jpgx("C:/Users/gyu-il/Desktop/ÆÇ¸Å/IMG_20140112_220817.jpg", sc_array, scList->Count, (char*)des3_test_keys);
}

