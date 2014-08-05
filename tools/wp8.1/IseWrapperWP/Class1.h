#pragma once

namespace IseWrapperWP
{
    public ref class Class1 sealed
    {
		public:
			static JpgxDecompressContainer getJpgxContainer(String filename, String key);
			static PngxDecompressContainer^ getPngxContainer(String filename, String key);

			static void makeJPGX(String filename, Windows::Foundation::Collections::IIterator<SecureContainer> scList, String key);
			static void makePNGX(String filename, Windows::Foundation::Collections::IIterator<SecureContainer> scList, String key);
    };
}