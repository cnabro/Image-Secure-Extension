using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;

namespace IseStudio.Controls
{
    internal static class CanvasAssistant
    {
        #region Dependency Properties

        public static readonly DependencyProperty BoundChildrenProperty =
            DependencyProperty.RegisterAttached("BoundChildren", typeof(object), typeof(CanvasAssistant),
                                                new PropertyMetadata(null, onBoundChildrenChanged));

        #endregion

        public static void SetBoundChildren(DependencyObject dependencyObject, string value)
        {
            dependencyObject.SetValue(BoundChildrenProperty, value);
        }

        private static void onBoundChildrenChanged(DependencyObject dependencyObject,
                                                   DependencyPropertyChangedEventArgs e)
        {
            if (dependencyObject == null)
            {
                return;
            }
            var canvas = dependencyObject as Canvas;
            if (canvas == null) return;

            var objects = (ObservableCollection<UIElement>)e.NewValue;

            if (objects == null)
            {
                canvas.Children.Clear();
                return;
            }

            //TODO: Create Method for that.
            objects.CollectionChanged += (sender, args) =>
            {
                if (args.Action == NotifyCollectionChangedAction.Add)
                    foreach (object item in args.NewItems)
                    {
                        canvas.Children.Add((UIElement)item);
                    }
                if (args.Action == NotifyCollectionChangedAction.Remove)
                    foreach (object item in args.OldItems)
                    {
                        canvas.Children.Remove((UIElement)item);
                    }
            };

            foreach (UIElement item in objects)
            {
                canvas.Children.Add(item);
            }
        }
    }
}
