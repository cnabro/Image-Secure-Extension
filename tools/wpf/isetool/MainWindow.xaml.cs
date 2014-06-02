using isetool.Util;
using IseWrapper;
using System;
using System.Collections.Generic;
using System.Diagnostics;
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

            String path = "C:/Users/gyu-il/Desktop/판매/IMG_20140112_2208171.jpg";
            byte[] bytes = Encoding.ASCII.GetBytes(path);

            unsafe
            {
                List<SecureContainer> scList = new List<SecureContainer>();

                SecureContainer sc = new SecureContainer(100, 100, 300, 300);
                scList.Add(sc);

                ImageSecureExtention.makeJPGX(path, scList, "keytest");
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
