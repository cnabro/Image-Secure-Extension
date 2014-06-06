using MvvmExample.Helpers;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace isetool.ViewModel
{
    class MainWindowVM : BaseViewModel
    {

        public MainWindowVM()
        {

        }

        /*
         * menu command
         */
        public RelayCommand MenuFileOpenCommand { get; set; }
        public RelayCommand MenuFileSaveCommand { get; set; }
        public RelayCommand MenuFileSaveAsCommand { get; set; }
        public RelayCommand MenuAddEncryptionRectangleCommand { get; set; }
        public RelayCommand MenuAddEncryptionFaceCommand { get; set; }

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
                    NotifyPropertyChanged();
                }
            }

        }

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
                    NotifyPropertyChanged();
                }
            }

        }

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
                    NotifyPropertyChanged();
                }
            }
        }
    }
}
