using isetool.Util;
using IseWrapper;
using MahApps.Metro;
using MahApps.Metro.Controls;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
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
    public partial class MainWindow : MetroWindow
    {
        public MainWindow()
        {
            InitializeComponent();

            //ThemeManager.ChangeTheme(this, ThemeManager.DefaultAccents.First(a => a.Name == "Red"), Theme.Light);
            
            

            setImageJPGX();
        }

        public void setImageJPGX()
        {
            String path1 = "E:/opensource/ise/source/library/lib-win/test/temp.jpg";
            imageViewer.Source = new BitmapImage(new Uri(path1, UriKind.RelativeOrAbsolute));
            List<SecureContainer> scList = new List<SecureContainer>();

            SecureContainer sc = new SecureContainer(100, 100, 300, 300);
            SecureContainer s2 = new SecureContainer(200, 200, 400, 400);
            scList.Add(s2);

            ImageSecureExtention.makeJPGX(path1, scList, "keytest");
            ///////////
            /**/
            String path = "E:/opensource/ise/source/library/lib-win/test/temp.jpgx";

            JpgxDecompressContainer container = ImageSecureExtention.getSecureJpegContainer(path, "test");
            Bitmap bitmap = container.getImageBitmapRGB24();

            MemoryStream ms = new MemoryStream();
            bitmap.Save(ms, System.Drawing.Imaging.ImageFormat.Bmp);

            BitmapImage image = new BitmapImage();
            image.BeginInit();
            ms.Seek(0, SeekOrigin.Begin);
            image.StreamSource = ms;
            image.EndInit();

            imageViewer.Source = image;
        }



        
        
    }
}
