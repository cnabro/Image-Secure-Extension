#include <wrl.h>
#include <robuffer.h>
#include <Converter.h>
#include <png.h>
#include <ppltasks.h>

using namespace concurrency;
using namespace Microsoft::WRL;

using namespace Platform;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation;

namespace IseWrapperWP
{
	public ref class SecureContainer sealed
	{
	public:
		SecureContainer(int w, int h, int x, int y)
		{
			this->width = w;
			this->height = h;
			this->pos_x = x;
			this->pos_y = y;
		}

		/*
		getter
		*/
		int getWidth()
		{
			return this->width;
		}

		int getHeight()
		{
			return this->height;
		}

		int getPosX()
		{
			return this->pos_x;
		}

		int getPosY()
		{
			return this->pos_y;
		}

	private:
		int width;
		int height;
		int pos_x;
		int pos_y;
	};

	public ref class JpgxDecompressContainer sealed
	{
	public:
		JpgxDecompressContainer(
			int width,
			int height,
			int color_space,
			int status)
		{
			this->image_height = height;
			this->image_width = width;
			this->color_space = color_space;
			this->status = status;
		}

		JpgxDecompressContainer()
		{

		}

		int getColorSpace()
		{
			return this->color_space;
		}

		int getStatus()
		{
			return this->status;
		}

		int getWidth()
		{
			return this->image_width;
		}

		int getHeight()
		{
			return this->image_height;
		}

		IAsyncOperation<BitmapImage^>^ GetImageAsync()
		{
			IAsyncOperation<BitmapImage^>^ asynctask = create_async([this]() -> task<BitmapImage^>
			{
				IRandomAccessStream^ stream = ref new InMemoryRandomAccessStream();
				IAsyncOperation<BitmapEncoder^>^ encoder = BitmapEncoder::CreateAsync(BitmapEncoder::JpegEncoderId, stream);

				task<BitmapEncoder^> encodertask = create_task(encoder);

				return encodertask.then([this, stream](BitmapEncoder^ encoder)->task<BitmapImage^>
				{
					byte *rgba;

					if (color_space == 3)
					{
						rgba = new byte[image_width * image_height * 4];

						byte * rgb = static_cast<byte*>(image);

						//convert rgb24 to bgr24
						for (int y = 0; y < image_height; y++)
						{
							for (int x = 0; x < image_width; x++)
							{
								int r = image[y * image_width * 3 + x * 3];
								int g = image[y * image_width * 3 + x * 3 + 1];
								int b = image[y * image_width * 3 + x * 3 + 2];
								//int a = 255;

								rgba[y * image_width * 4 + x * 4] = r;
								rgba[y * image_width * 4 + x * 4 + 1] = g;
								rgba[y * image_width * 4 + x * 4 + 2] = b;
								rgba[y * image_width * 4 + x * 4 + 3] = 255;
							}
						}
					}
					else
					{
						rgba = static_cast<byte*>(image);
					}

					Platform::ArrayReference<uint8> blobArray(rgba, image_width * image_height * 4);

					encoder->SetPixelData(BitmapPixelFormat::Rgba8, BitmapAlphaMode::Straight, image_width, image_height, 96, 96, blobArray);
					task<void> flushtask = create_task(encoder->FlushAsync());
					
					return flushtask.then([stream]()->BitmapImage^
					{
						BitmapImage^ bitmapImage = ref new BitmapImage();
						bitmapImage->SetSource(stream);

						return bitmapImage;
					});
				});
			});
			
			return asynctask;
		}

	internal:
		void setImageSource(unsigned char * source)
		{
			image = source;
		}

	private:
		unsigned char * image;
		int color_space;
		int image_width;
		int image_height;
		int status;
	};

	public ref class PngxDecompressContainer sealed
	{
	public:
		PngxDecompressContainer(
			int width,
			int height,
			int color_space,
			int status)
		{
			this->image_height = height;
			this->image_width = width;
			this->color_space = color_space;
			this->status = status;
		}

		PngxDecompressContainer()
		{

		}

		int getColorSpace()
		{
			return this->color_space;
		}

		int getStatus()
		{
			return this->status;
		}

		int getWidth()
		{
			return this->image_width;
		}

		int getHeight()
		{
			return this->image_height;
		}

		//Windows::UI::Xaml::Media::Imaging::BitmapImage^ getImageBitmap()
		//{
		//	for (int y = 0; y < image_height; y++) {
		//		for (int x = 0; x < image_width * color_space; x = x + color_space) {
		//			int r = image[y][x];
		//			int g = image[y][x + 1];
		//			int b = image[y][x + 2];

		//			image[y][x] = b;
		//			image[y][x + 1] = g;
		//			image[y][x + 2] = r;
		//		}
		//	}

		////	System::Drawing::Bitmap^ systemBitmap;
		////	if (color_space == 3)
		////	{
		////		systemBitmap = gcnew System::Drawing::Bitmap(image_width, image_height, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
		////	}
		////	else if (color_space == 4)
		////	{
		////		systemBitmap = gcnew System::Drawing::Bitmap(image_width, image_height, System::Drawing::Imaging::PixelFormat::Format32bppArgb);
		////	}

		////	System::Drawing::Rectangle rect;// = new System::Drawing::Rectangle(0, 0, image_width, image_height);
		////	rect.X = 0;
		////	rect.Y = 0;
		////	rect.Width = image_width;
		////	rect.Height = image_height;

		////	System::Drawing::Imaging::BitmapData^ bitmapData = systemBitmap->LockBits(rect, System::Drawing::Imaging::ImageLockMode::WriteOnly, systemBitmap->PixelFormat);

		////	array<unsigned char>^ values = gcnew array<unsigned char>(image_height*bitmapData->Stride);
		////	for (int i = 0; i < image_height; i++)
		////	{
		////		System::Runtime::InteropServices::Marshal::Copy(System::IntPtr((void *)(image[i])), values, bitmapData->Stride*i, image_width*color_space);
		////	}


		////	System::Runtime::InteropServices::Marshal::Copy(values, 0, bitmapData->Scan0, image_height*bitmapData->Stride);
		////	systemBitmap->UnlockBits(bitmapData);

		////	return systemBitmap;
		//}

	internal:
		void setImageSource(png_bytep * source)
		{
			image = source;
		}

	private:
		png_bytep *image;
		int color_space;
		int image_width;
		int image_height;
		int status;
	};
}