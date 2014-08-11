using IseStudio.ViewModel;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Data;

namespace IseStudio.Converter
{
    public class BoolToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter ,string culture)
        {
            Boolean bvalue = (Boolean)value;

            if (bvalue)
            {
                return Visibility.Visible;
            }
            else
            {
                return Visibility.Collapsed;
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, string culture)
        {
            return null;
        }
    }

    public class ModeToVisibilityConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, string culture)
        {
            StartMode mode = (StartMode)value;
            switch (mode)
            {
                case StartMode.MODE_MAKER:
                    return Visibility.Visible;

                case StartMode.MODE_VIEWER:
                    return Visibility.Collapsed;

                default:
                    return 250;
            }
        }

        public object ConvertBack(object value, Type targetType, object parameter, string culture)
        {
            return null;
        }
    }

    public class ModeToSizeConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, string culture)
        {
            StartMode mode = (StartMode)value;
            switch(mode)
            {
                case StartMode.MODE_MAKER:
                    return 250;
                    
                case StartMode.MODE_VIEWER:
                    return 0;

                default :
                    return 250;
            }
            
        }

        public object ConvertBack(object value, Type targetType, object parameter, string culture)
        {
            return null;
        }
    }
}
