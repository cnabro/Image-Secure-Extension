// libise_wrapper.h

#pragma once
#include "isecore.h"
#include "IseContainerWrapper.h"
#include "Converter.h"

using namespace System;
using namespace std;

namespace IseWrapper
{
	public ref class ImageSecureExtention
	{
	public:
		static System::String^ getSecureJpegContainer(System::String^ filename, System::String^ key);
		static void getSecurePngContainer(System::String^ filename, System::String^ key);

		static void makeJPGX(System::String^ filename, System::Collections::Generic::List<SecureContainer^>^ scList, System::String^ key);
		static void makePNGX(System::String^ filename, System::Collections::Generic::List<SecureContainer^>^ scList, System::String^ key);
	};
	
	
	
}
