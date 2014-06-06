using isetool.Util;
using IseWrapper;
using MahApps.Metro;
using MahApps.Metro.Controls;
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
    public partial class MainWindow : MetroWindow
    {
        public MainWindow()
        {
            InitializeComponent();

            //ThemeManager.ChangeTheme(this, ThemeManager.DefaultAccents.First(a => a.Name == "Red"), Theme.Light);

            //String path = "C:/Users/cnabro/Desktop/엄마꺼/IMG_20130626_155207.jpg";

            //unsafe
            //{
            //    List<SecureContainer> scList = new List<SecureContainer>();

            //    SecureContainer sc = new SecureContainer(100, 100, 300, 300);
            //    scList.Add(sc);

            //    ImageSecureExtention.makeJPGX(path, scList, "keytest");
            //}
        }

        
        
    }
}
