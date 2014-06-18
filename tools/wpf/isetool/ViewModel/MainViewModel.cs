using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.Command;
using IseWrapper;
using Microsoft.Practices.ServiceLocation;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;

namespace isetool.ViewModel
{
    public class MainViewModel : ViewModelBase
    {
        public enum StartMode
        {
            MODE_VIEWER, MODE_MAKER
        }

        public MainViewModel()
        {
            string[] args = Environment.GetCommandLineArgs();
            if (args.Length > 1)
            {
                Mode = StartMode.MODE_VIEWER;

                FilePath = args[1];
                LoadSecurityFile(FilePath);
            }

            MenuFileSaveCommand = new RelayCommand(() => SaveFile());
            MenuFileOpenCommand = new RelayCommand(() => ExcuteFileOpen());
            DialogSubmitCommand = new RelayCommand(() => DialogSubmit());
            
        }

        public StartMode Mode = StartMode.MODE_MAKER;

        /// <summary>
        /// Top Menu Commands
        /// </summary> 
        public RelayCommand MenuFileOpenCommand { get; set; }
        public RelayCommand MenuFileSaveCommand { get; set; }
        //public RelayCommand MenuFileSaveAsCommand { get; set; }
        public RelayCommand MenuAddEncryptionRectangleCommand { get; set; }
        public RelayCommand MenuAddEncryptionFaceCommand { get; set; }

        public RelayCommand DialogSubmitCommand { get; set; }

        /// <summary>
        /// Image Height Property
        /// </summary>
        private int _ImageHeight;
        public int ImageHeight
        {
            get
            {
                return this._ImageHeight;
            }

            set
            {
                if (value != this._ImageHeight)
                {
                    this._ImageHeight = value;
                    RaisePropertyChanged("ImageHeight");
                }
            }
        }

        /// <summary>
        /// Password Property
        /// </summary>
        private string _Password = string.Empty;
        public string Password
        {
            get
            {
                return this._Password;
            }

            set
            {
                if (value != this._Password)
                {
                    this._Password = value;
                    RaisePropertyChanged("Password");
                }
            }

        }

        /// <summary>
        /// Dialog Password Property
        /// </summary>
        private string _DialogPassword = string.Empty;
        public string DialogPassword
        {
            get
            {
                return this._DialogPassword;
            }

            set
            {
                if (value != this._DialogPassword)
                {
                    this._DialogPassword = value;
                    RaisePropertyChanged("DialogPassword");
                }
            }

        }

        /// <summary>
        /// Image Width Property
        /// </summary>
        private int _ImageWidth;
        public int ImageWidth
        {
            get
            {
                return this._ImageWidth;
            }

            set
            {
                if (value != this._ImageWidth)
                {
                    this._ImageWidth = value;
                    RaisePropertyChanged("ImageWidth");
                }
            }

        }

        /// <summary>
        /// Image Property
        /// </summary>
        private BitmapImage _Image;
        public BitmapImage Image
        {
            get
            {
                return this._Image;
            }

            set
            {
                if (value != this._Image)
                {
                    this._Image = value;
                    RaisePropertyChanged("Image");
                }
            }

        }

        /// <summary>
        /// File Path Property
        /// </summary>
        private string _FilePath;
        public string FilePath
        {
            get
            {
                return this._FilePath;
            }

            set
            {
                if (value != this._FilePath)
                {
                    this._FilePath = value.Replace("\\","/");
                    RaisePropertyChanged("FilePath");
                }
            }
        }

        private void SaveFile()
        {
            if(FilePath != null)
            {
                MultiSelectionCanvasViewModel vm = ServiceLocator.Current.GetInstance<MultiSelectionCanvasViewModel>();
                List<SecureContainer> scList = new List<SecureContainer>();
                foreach(Model.SecureContainerModel model in vm.ContainerList)
                {
                    scList.Add(new SecureContainer(model.Width, model.Height, model.X, model.Y));
                }

                ImageSecureExtention.makeJPGX(FilePath,scList,Password);
            }
        }

        private void ExcuteFileOpen()
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();

            dlg.Filter = "JPEG Files (*.jpeg)|*.jpeg|PNG Files (*.png)|*.png|JPG Files (*.jpg)|*.jpg";

            Nullable<bool> result = dlg.ShowDialog();

            if (result == true)
            {
                FilePath = dlg.FileName;
                Image = new BitmapImage(new Uri(FilePath, UriKind.RelativeOrAbsolute));
                ImageWidth = Image.PixelWidth;
                ImageHeight = Image.PixelHeight;
            }
        }

        private void LoadSecurityFile(string path,string pwd="")
        {
            JpgxDecompressContainer container = ImageSecureExtention.getJpgxContainer(path, pwd);
            Bitmap bitmap = container.getImageBitmapRGB24();

            MemoryStream ms = new MemoryStream();
            bitmap.Save(ms, System.Drawing.Imaging.ImageFormat.Bmp);

            BitmapImage image = new BitmapImage();
            image.BeginInit();
            ms.Seek(0, SeekOrigin.Begin);
            image.StreamSource = ms;
            image.EndInit();

            Image = image;

            ImageWidth = container.getWidth();
            ImageHeight = container.getHeight();
        }

        private void DialogSubmit()
        {
            LoadSecurityFile(FilePath, DialogPassword);
        }

        private void ClearSelect()
        {
            
        }

    }
}