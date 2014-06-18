using GalaSoft.MvvmLight;
using isetool.Model;
using IseWrapper;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace isetool.ViewModel
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
