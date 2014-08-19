using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.Command;
using IseStudio.Model;
using IseWrapperWP;
using Microsoft.Practices.ServiceLocation;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Threading.Tasks;
using System.Windows;
using Windows.ApplicationModel.Activation;
using Windows.Storage.Pickers;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media.Imaging;

namespace IseStudio.ViewModel
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
            //DialogCancelCommand = new RelayCommand(() => HidePasswordDialog());
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
        private int _ImageHeight = 0;
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
        private int _ImageWidth = 0;
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
        /// Image Property
        /// </summary>
        private Boolean _PasswordDialogVisibility = false;
        public Boolean PasswordDialogVisibility
        {
            get
            {
                return this._PasswordDialogVisibility;
            }

            set
            {
                if (value != this._PasswordDialogVisibility)
                {
                    this._PasswordDialogVisibility = value;
                    RaisePropertyChanged("PasswordDialogVisibility");
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
                    this._FilePath = value.Replace("\\", "/");
                    RaisePropertyChanged("FilePath");
                }
            }
        }

        private ObservableCollection<SecureContainerModel> _ContainerList = new ObservableCollection<SecureContainerModel>();
        public ObservableCollection<SecureContainerModel> ContainerList
        {
            get
            {
                return _ContainerList;
            }
            set
            {
                if (value != _ContainerList)
                {
                    _ContainerList = value;
                    RaisePropertyChanged("ContainerList");
                }
            }
        }

        private ObservableCollection<SecureContainerModel> _ActualContainerList = new ObservableCollection<SecureContainerModel>();
        public ObservableCollection<SecureContainerModel> ActualContainerList
        {
            get
            {
                return _ActualContainerList;
            }
            set
            {
                if (value != _ContainerList)
                {
                    _ActualContainerList = value;
                    RaisePropertyChanged("ActualContainerList");
                }
            }
        }

        private void SaveFile()
        {
            if (FilePath != null && !String.IsNullOrEmpty(Password))
            {
                try
                {
                    List<SecureContainer> scList = new List<SecureContainer>();
                    foreach (Model.SecureContainerModel model in ActualContainerList)
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
                    MessageDialog dialog = new MessageDialog("Failed to save file.");
                    dialog.ShowAsync();
                }
                finally
                {
                    MessageDialog dialog = new MessageDialog("Success.");
                    dialog.ShowAsync();
                }
            }
            else
            {
                MessageDialog dialog = new MessageDialog("Please Input Password.");
                dialog.ShowAsync();
            }
        }

        private void ExcuteFileOpen()
        {
            CancelSelection();

            FileOpenPicker openPicker = new FileOpenPicker();
            openPicker.ViewMode = PickerViewMode.List;
            openPicker.SuggestedStartLocation = PickerLocationId.ComputerFolder;
            openPicker.FileTypeFilter.Add(".jpg");
            openPicker.FileTypeFilter.Add(".jpeg");
            openPicker.FileTypeFilter.Add(".png");
            openPicker.FileTypeFilter.Add(".pngx");
            openPicker.FileTypeFilter.Add(".jpgx");

            openPicker.PickSingleFileAndContinue();
        }

        public void Continue(FileOpenPickerContinuationEventArgs args)
        {
            if (args.Files.Count > 0)
            {
                FilePath = args.Files[0].Path;
                LoadFile();
            }
        }

        private void FileOpen(string path)
        {
            FilePath = path;
            Image = new BitmapImage(new Uri(FilePath, UriKind.RelativeOrAbsolute));
            Image.ImageOpened += Image_ImageOpened;
            
        }

        void Image_ImageOpened(object sender, RoutedEventArgs e)
        {
            BitmapImage image = sender as BitmapImage;

            ImageWidth = image.PixelWidth;
            ImageHeight = image.PixelHeight;
        }

        private void CancelSelection()
        {
            ContainerList.Clear();
            ActualContainerList.Clear();
        }

        private void FaceSelection()
        {
            CancelSelection();
        }

        private void AllSelection()
        {
            CancelSelection();

            ContainerList.Add(new Model.SecureContainerModel(0, 0, ImageWidth, ImageHeight));
        }

        private async void LoadSecurityFile(string path, string pwd = "")
        {
            if (path.EndsWith(".jpgx"))
            {
                JpgxDecompressContainer container = ImageSecureExtention.getJpgxContainer(path, pwd);
 
                Image = await container.getImageAsync();

                ImageWidth = container.getWidth();
                ImageHeight = container.getHeight();
            }
            else
            {
                PngxDecompressContainer container = ImageSecureExtention.getPngxContainer(path, pwd);

                Image = await container.getImageAsync();

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
            PasswordDialogVisibility = false;
        }

        private void OpenPasswordDialog()
        {
            PasswordDialogVisibility = true;
        }

    }
}