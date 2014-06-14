using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.Command;
using System;

namespace isetool.ViewModel
{
    public class MainViewModel : ViewModelBase
    {

        public MainViewModel()
        {
            MenuFileOpenCommand = new RelayCommand(() => ExcuteFileOpenCommand());
        }

        /// <summary>
        /// Top Menu Commands
        /// </summary>
        public RelayCommand MenuFileOpenCommand { get; set; }
        public RelayCommand MenuFileSaveCommand { get; set; }
        public RelayCommand MenuFileSaveAsCommand { get; set; }
        public RelayCommand MenuAddEncryptionRectangleCommand { get; set; }
        public RelayCommand MenuAddEncryptionFaceCommand { get; set; }

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
                    this._FilePath = value;
                    RaisePropertyChanged("FilePath");
                }
            }
        }

        private void ExcuteFileOpenCommand()
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();

            //dlg.DefaultExt = ".png";
            dlg.Filter = "JPEG Files (*.jpeg)|*.jpeg|PNG Files (*.png)|*.png|JPG Files (*.jpg)|*.jpg|GIF Files (*.gif)|*.gif";

            Nullable<bool> result = dlg.ShowDialog();

            if (result == true)
            {
                FilePath = dlg.FileName;
            }

        }
    }
}