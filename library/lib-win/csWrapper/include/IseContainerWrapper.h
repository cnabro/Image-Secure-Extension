#using <System.Drawing.dll>

namespace IseWrapper
{
	public ref class SecureContainer
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
		
	private :
		int width;
		int height;
		int pos_x;
		int pos_y;
	};

	public ref class JpgxDecompressContainer
	{
	public :
		JpgxDecompressContainer(
			unsigned char *image, 
			//System::Collections::Generic::List<SecureContainer^>^ list, 
			int width, 
			int height,
			int color_space,
			int status)
		{
			this->image = image;
			//this->scList = list;
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

		System::Drawing::Bitmap^ getImageBitmapRGB()
		{
			//convert rgb24 to bgr24
			int offset = 0;

			for (int y = 0; y < image_height; y++) {
				for (int x = 0; x < image_width; x++) {
					int r = image[offset];
					int g = image[offset + 1];
					int b = image[offset + 2];
					
					image[offset] = b;
					image[offset+1] = g;
					image[offset+2] = r;

					offset += color_space;
				}
			}

			System::Drawing::Bitmap^ systemBitmap;
			if (color_space == 3)
			{
				systemBitmap = gcnew System::Drawing::Bitmap(image_width, image_height, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
			}
			else if (color_space == 4)
			{
				systemBitmap = gcnew System::Drawing::Bitmap(image_width, image_height, System::Drawing::Imaging::PixelFormat::Format32bppArgb);
			}

			System::Drawing::Rectangle rect;// = new System::Drawing::Rectangle(0, 0, image_width, image_height);
			rect.X = 0;
			rect.Y = 0;
			rect.Width = image_width;
			rect.Height = image_height;

			System::Drawing::Imaging::BitmapData^ bitmapData = systemBitmap->LockBits(rect, System::Drawing::Imaging::ImageLockMode::WriteOnly, systemBitmap->PixelFormat);

			array<unsigned char>^ values = gcnew array<unsigned char>(image_height*bitmapData->Stride);
			for (int i = 0; i < image_height; i++)
			{
				System::Runtime::InteropServices::Marshal::Copy(System::IntPtr((void *)(image + i*image_width*color_space)), values, bitmapData->Stride*i, image_width*color_space);
			}
			

			System::Runtime::InteropServices::Marshal::Copy(values, 0, bitmapData->Scan0, image_height*bitmapData->Stride);
			systemBitmap->UnlockBits(bitmapData);

			return systemBitmap;
		}

	private :
		//System::Collections::Generic::List<SecureContainer^>^ scList;
		unsigned char *image;
		int color_space;
		int image_width;
		int image_height;
		int status;
	};

	public ref class PngxDecompressContainer
	{
	public:
		PngxDecompressContainer(
			png_bytep *image,
			//System::Collections::Generic::List<SecureContainer^>^ list, 
			int width,
			int height,
			int color_space,
			int status)
		{
			this->image = image;
			//this->scList = list;
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

		System::Drawing::Bitmap^ getImageBitmap()
		{
			for (int y = 0; y < image_height; y++) {
				for (int x = 0; x < image_width * color_space; x = x + color_space) {
					int r = image[y][x];
					int g = image[y][x + 1];
					int b = image[y][x + 2];

					image[y][x] = b;
					image[y][x + 1] = g;
					image[y][x + 2] = r;
				}
			}

			System::Drawing::Bitmap^ systemBitmap;
			if (color_space == 3)
			{
				systemBitmap = gcnew System::Drawing::Bitmap(image_width, image_height, System::Drawing::Imaging::PixelFormat::Format24bppRgb);
			}
			else if (color_space == 4)
			{
				systemBitmap = gcnew System::Drawing::Bitmap(image_width, image_height, System::Drawing::Imaging::PixelFormat::Format32bppArgb);
			}

			System::Drawing::Rectangle rect;// = new System::Drawing::Rectangle(0, 0, image_width, image_height);
			rect.X = 0;
			rect.Y = 0;
			rect.Width = image_width;
			rect.Height = image_height;

			System::Drawing::Imaging::BitmapData^ bitmapData = systemBitmap->LockBits(rect, System::Drawing::Imaging::ImageLockMode::WriteOnly, systemBitmap->PixelFormat);

			array<unsigned char>^ values = gcnew array<unsigned char>(image_height*bitmapData->Stride);
			for (int i = 0; i < image_height; i++)
			{
				System::Runtime::InteropServices::Marshal::Copy(System::IntPtr((void *)(image[i])), values, bitmapData->Stride*i, image_width*color_space);
			}


			System::Runtime::InteropServices::Marshal::Copy(values, 0, bitmapData->Scan0, image_height*bitmapData->Stride);
			systemBitmap->UnlockBits(bitmapData);

			return systemBitmap;
		}

	private:
		//System::Collections::Generic::List<SecureContainer^>^ scList;
		png_bytep *image;
		int color_space;
		int image_width;
		int image_height;
		int status;
	};
}