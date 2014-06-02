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
}