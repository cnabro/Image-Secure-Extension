using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.Ioc;
using IseStudio.ViewModel;
using Microsoft.Practices.ServiceLocation;

namespace IseStudio
{
    /// <summary>
    /// This class contains static references to all the view models in the
    /// application and provides an entry point for the bindings.
    /// </summary>
    public class ViewModelLocator
    {
        /// <summary>
        /// Initializes a new instance of the ViewModelLocator class.
        /// </summary>
        public ViewModelLocator()
        {
            ServiceLocator.SetLocatorProvider(() => SimpleIoc.Default);

            SimpleIoc.Default.Register<MainViewModel>();
            SimpleIoc.Default.Register<MultiSelectionCanvasViewModel>();
        }

        public MainViewModel Main
        {
            get
            {
                return ServiceLocator.Current.GetInstance<MainViewModel>();
            }
        }

        public MultiSelectionCanvasViewModel MultiSelectionCanvas
        {
            get
            {
                return ServiceLocator.Current.GetInstance<MultiSelectionCanvasViewModel>();
            }
        }
        
        public static void Cleanup()
        {
            
        }
    }
}