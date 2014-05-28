using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace isetool
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            String path = "E:/opensource/ise/source/library/lib-win/test/test.jpg";
            byte[] bytes = Encoding.ASCII.GetBytes(path);

            unsafe
            {
                fixed (byte* p = bytes)
                {
                    IseWrapper.IseWrapperClass iseLib = new IseWrapper.IseWrapperClass();

                    //E:/opensource/ise/source/library/lib-win/test/test.jpg
                    byte* imgBuffer = iseLib.getSecureJpegBuffer((sbyte*)p);
                    
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
