#include "stdafx.h"

#include "IseWrapper.h"


System::String^ IseWrapper::ImageSecureExtention::getSecureJpegContainer(System::String^ filename, System::String^ key)
{
	//jpgx_decompress_container jdc = get_secure_jpeg_container(filename, key);

	//JpgxDecompressContainer jdcWrapper;
	//jdcWrapper.sc_cnt = jdc.sc_cnt;
	//return jdc;
	std::wstring wstr_filename = msclr::interop::marshal_as<std::wstring>(filename);
	std::wstring wstr_key = msclr::interop::marshal_as<std::wstring>(key);


	printf("test getSecureJpegContainer : %s %s", wstr_filename.c_str(), wstr_key.c_str());


	return filename;
}

void IseWrapper::ImageSecureExtention::getSecurePngContainer(System::String^ filename, System::String^ key)
{
	std::wstring wstr_filename = msclr::interop::marshal_as<std::wstring>(filename);
	std::wstring wstr_key = msclr::interop::marshal_as<std::wstring>(key);


	printf("test getSecurePngContainer : %s %s", wstr_filename.c_str(), wstr_key.c_str());
	//return get_secure_png_container(filename, key);
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

	make_jpgx(path, sc_array, scList->Count, (char*)des3_test_keys);

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
