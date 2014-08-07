using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.Command;
using IseWrapper;
using MahApps.Metro.Controls;
using MahApps.Metro.Controls.Dialogs;
using Microsoft.Practices.ServiceLocation;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media.Imaging;

namespace isetool.ViewModel
{
    public enum StartMode
    {
        MODE_VIEWER, MODE_MAKER
    }

    public class MainViewModel : ViewModelBase
    {
        public MainViewModel()
        {

            MenuFileSaveCommand = new RelayCommand(() => SaveFile());
            MenuFileOpenCommand = new RelayCommand(() => ExcuteFileOpen());
            DialogSubmitCommand = new RelayCommand(() => DialogSubmit());
            DialogCancelCommand = new RelayCommand(() => HidePasswordDialog());
            MenuSelectionAllCommand = new RelayCommand(() => AllSelection());
            MenuSelectionCancelCommand = new RelayCommand(() => CancelSelection());
            MenuSelectionFaceCommand = new RelayCommand(() => FaceSelection());
        }

        /// <summary>
        /// Top Menu Commands
        /// </summary> 
        public RelayCommand MenuFileOpenCommand { get; set; }
        public RelayCommand MenuFileSaveCommand { get; set; }
        //public RelayCommand MenuFileSaveAsCommand { get; set; }
        public RelayCommand MenuSelectionAllCommand { get; set; }
        public RelayCommand MenuSelectionCancelCommand { get; set; }
        public RelayCommand MenuSelectionFaceCommand { get; set; }

        public RelayCommand DialogSubmitCommand { get; set; }
        public RelayCommand DialogCancelCommand { get; set; }

        /// <summary>
        /// Open Mode Property
        /// </summary>
        private StartMode _StMode = StartMode.MODE_MAKER;
        public StartMode StMode
        {
            get
            {
                return this._StMode;
            }

            set
            {
                if (value != this._StMode)
                {
                    this._StMode = value;
                    RaisePropertyChanged("StMode");
                }
            }
        }

        /// <summary>
        /// Zoom Level Property
        /// </summary>
        private int _ZoomLevel = 3;
        public int ZoomLevel
        {
            get
            {
                return this._ZoomLevel;
            }

            set
            {
                if (value != this._ZoomLevel)
                {
                    this._ZoomLevel = value;
                    RaisePropertyChanged("ZoomLevel");
                }
            }
        }

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
            if (FilePath != null && !String.IsNullOrEmpty(Password))
            {
                try 
                {
                    MultiSelectionCanvasViewModel vm = ServiceLocator.Current.GetInstance<MultiSelectionCanvasViewModel>();
                    List<SecureContainer> scList = new List<SecureContainer>();
                    foreach (Model.SecureContainerModel model in vm.ContainerList)
                    {
                        scList.Add(new SecureContainer(model.Width, model.Height, model.X, model.Y));
                    }

                    if (FilePath.EndsWith(".jpg"))
                    {
                        ImageSecureExtention.makeJPGX(FilePath, scList, Password);
                    }
                    else
                    {
                        ImageSecureExtention.makePNGX(FilePath, scList, Password);
                    }
                    
                }
                catch (Exception e)
                {
                    DialogManager.ShowMessageAsync(Application.Current.MainWindow as MetroWindow, "Error", "Failed to save file.");
                }
                finally
                {
                    DialogManager.ShowMessageAsync(Application.Current.MainWindow as MetroWindow, "Result", "Success.");
                }
            }
            else
            {
                DialogManager.ShowMessageAsync(Application.Current.MainWindow as MetroWindow, "Warning", "Please Input Password.");
            }
        }

        private void ExcuteFileOpen()
        {
            CancelSelection();

            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();

            dlg.Filter = "JPEG Files (*.jpeg,*.jpg)|*.jpg;*.jpeg|PNG Files (*.png)|*.png|JPGX Files (*.jpgx)|*.jpgx|PNGX Files (*.pngx)|*.pngx";

            Nullable<bool> result = dlg.ShowDialog();

            if (result == true)
            {
                FilePath = dlg.FileName;
                LoadFile();
            }
        }

        public void ParseParameter()
        {
            string[] args = Environment.GetCommandLineArgs();

            if (args.Length > 1)
            {
                FilePath = args[1];
                LoadFile();
            }
        }

        private void FileOpen(string path)
        {
            FilePath = path;
            Image = new BitmapImage(new Uri(FilePath, UriKind.RelativeOrAbsolute));
            ImageWidth = Image.PixelWidth;
            ImageHeight = Image.PixelHeight;
        }

        private void CancelSelection()
        {
            MultiSelectionCanvasViewModel vm = ServiceLocator.Current.GetInstance<MultiSelectionCanvasViewModel>();
            vm.ContainerList.Clear();
        }

        private void FaceSelection()
        {
            CancelSelection();
        }

        private void AllSelection()
        {
            CancelSelection();

            MultiSelectionCanvasViewModel vm = ServiceLocator.Current.GetInstance<MultiSelectionCanvasViewModel>();
            vm.ContainerList.Add(new Model.SecureContainerModel(0, 0, ImageWidth, ImageHeight));
        }

        private void LoadSecurityFile(string path,string pwd="")
        {
            if(path.EndsWith(".jpgx"))
            {
                JpgxDecompressContainer container = ImageSecureExtention.getJpgxContainer(path, pwd);
                Bitmap bitmap = container.getImageBitmapRGB24();

                MemoryStream ms = new MemoryStream();
                ms.Seek(0, SeekOrigin.Begin);

                bitmap.Save(ms, System.Drawing.Imaging.ImageFormat.Bmp);

                BitmapImage image = new BitmapImage();
                image.BeginInit();

                image.StreamSource = ms;
                image.EndInit();

                Image = image;

                ImageWidth = container.getWidth();
                ImageHeight = container.getHeight();
            }
            else
            {
                PngxDecompressContainer container = ImageSecureExtention.getPngxContainer(path, pwd);
                Bitmap bitmap = container.getImageBitmap();

                MemoryStream ms = new MemoryStream();
                ms.Seek(0, SeekOrigin.Begin);

                bitmap.Save(ms, System.Drawing.Imaging.ImageFormat.Bmp);

                BitmapImage image = new BitmapImage();
                image.BeginInit();

                image.StreamSource = ms;
                image.EndInit();

                Image = image;

                ImageWidth = container.getWidth();
                ImageHeight = container.getHeight();
            }
           
        }

        private void LoadFile()
        {
            if (FilePath.EndsWith(".jpg") || FilePath.EndsWith(".jpeg") || FilePath.EndsWith(".png"))
            {
                StMode = StartMode.MODE_MAKER;
                FileOpen(FilePath);
            }
            else
            {
                StMode = StartMode.MODE_VIEWER;
                LoadSecurityFile(FilePath);
                OpenPasswordDialog();
            }
        }

        private void DialogSubmit()
        {
            LoadSecurityFile(FilePath, DialogPassword);
            HidePasswordDialog();
        }

        private void OpenPasswordDialog()
        {
            MetroWindow window = Application.Current.MainWindow as MetroWindow;
            var dialog = (BaseMetroDialog)window.Resources["PasswordDialog"];

            window.ShowMetroDialogAsync(dialog);
        }

        private void HidePasswordDialog()
        {
            MetroWindow window = Application.Current.MainWindow as MetroWindow;
            var dialog = (BaseMetroDialog)window.Resources["PasswordDialog"];

            window.HideMetroDialogAsync(dialog);
        }

    }
}