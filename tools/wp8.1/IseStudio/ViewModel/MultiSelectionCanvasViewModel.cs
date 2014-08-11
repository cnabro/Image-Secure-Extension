﻿using GalaSoft.MvvmLight;
using IseStudio.Model;
using IseWrapperWP;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IseStudio.ViewModel
{
    public class MultiSelectionCanvasViewModel : ViewModelBase
    {
        public MultiSelectionCanvasViewModel()
        {
            
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
    }
}
