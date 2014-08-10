#pragma once
#include "IseContainerWrapper.h"
#include "isecore.h"

using namespace IseWrapperWP;
using namespace Platform;

namespace IseWrapperWP
{
	public ref class ImageSecureExtention sealed
	{
	public:
		static JpgxDecompressContainer^ getJpgxContainer(String^ filename, String^ key);
		static PngxDecompressContainer^ getPngxContainer(String^ filename, String^ key);

		static void makeJPGX(String^ filename, Windows::Foundation::Collections::IIterator<ImageSecureExtention^>^ scList, String^ key);
		static void makePNGX(String^ filename, Windows::Foundation::Collections::IIterator<ImageSecureExtention^>^ scList, String^ key);
	};
}