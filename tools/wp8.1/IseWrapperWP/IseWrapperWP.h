#pragma once
#include "IseContainerWrapper.h"
#include "isecore.h"
#include <iostream>

using namespace IseWrapperWP;
using namespace Platform;
using namespace std;

namespace IseWrapperWP
{
	public ref class ImageSecureExtention sealed
	{
	public:
		static JpgxDecompressContainer^ getJpgxContainer(String^ filename, String^ key);
		static PngxDecompressContainer^ getPngxContainer(String^ filename, String^ key);

		static void makeJPGX(String^ filename, Windows::Foundation::Collections::IVector<SecureContainer^>^ scList, String^ key);
		static void makePNGX(String^ filename, Windows::Foundation::Collections::IVector<SecureContainer^>^ scList, String^ key);
	};
}