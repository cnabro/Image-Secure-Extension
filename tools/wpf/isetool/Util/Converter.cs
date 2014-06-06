using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;

namespace isetool.Util
{
    unsafe class Converter
    {
        public static sbyte* StringToSbyte(string str)
        {
            byte[] bytes = Encoding.ASCII.GetBytes(str);

            unsafe
            {
                fixed (byte* p = bytes)
                {
                    sbyte* sp = (sbyte*)p;
                    return sp;
                }
            }
        }

        public BitmapSource ByteToBitmapSource(byte[] image)
        {
            BitmapImage imageSource = new BitmapImage();
            using (MemoryStream stream = new MemoryStream(image))
            {
                stream.Seek(0, SeekOrigin.Begin);
                imageSource.BeginInit();
                imageSource.StreamSource = stream;
                imageSource.CacheOption = BitmapCacheOption.OnLoad;
                imageSource.EndInit();
            }
            return imageSource;
        }
    }
}
