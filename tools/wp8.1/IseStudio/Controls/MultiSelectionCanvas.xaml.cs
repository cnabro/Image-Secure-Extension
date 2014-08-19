using IseStudio.ViewModel;
using Microsoft.Practices.ServiceLocation;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using Windows.Foundation;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml;
using Windows.UI.Input;

namespace IseStudio.Controls
{
    /// <summary>
    /// MultiSelectionControl.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MultiSelectionControl : Canvas
    {
        public static readonly DependencyProperty PixelHeightProperty =
            DependencyProperty.RegisterAttached("PixelHeight", typeof(int), typeof(MultiSelectionControl), new PropertyMetadata(0, UpdateSize));

        public static readonly DependencyProperty PixelWidthProperty =
            DependencyProperty.RegisterAttached("PixelWidth", typeof(int), typeof(MultiSelectionControl), new PropertyMetadata(0, UpdateSize));

        public static void SetPixelHeight(DependencyObject dp, int value)
        {
            dp.SetValue(PixelHeightProperty, value);
        }

        public static int GetPixelHeight(DependencyObject dp)
        {
            return (int)dp.GetValue(PixelHeightProperty);
        }

        public static void SetPixelWidth(DependencyObject dp, int value)
        {
            dp.SetValue(PixelWidthProperty, value);
        }

        public static int GetPixelWidth(DependencyObject dp)
        {
            return (int)dp.GetValue(PixelWidthProperty);
        }

        private static void UpdateSize(DependencyObject sender, DependencyPropertyChangedEventArgs e)
        {
            MultiSelectionControl control = sender as MultiSelectionControl;

            int imgHeight = GetPixelHeight(sender);
            int imgWidth = GetPixelWidth(sender);

            if (imgHeight > 0 && imgWidth > 0)
            {
                float ratio = (float)control.ActualHeight / (float)imgHeight;

                control.Width = imgWidth * ratio;
                control.setImageRatio(ratio);
            }
        }

        private bool isLeftMouseButtonDownOnWindow = false;
        private bool isDraggingSelectionRect = false;
        private Point origMouseDownPoint;
        private static readonly double DragThreshold = 5;
        private float imageRatio = 0;

        //private Rectangle dragSelectionBorder;

        public MultiSelectionControl()
        {
            InitializeComponent();

            this.PointerPressed += OnMouseDown;
            this.PointerMoved += OnMouseMove;
            this.PointerReleased += OnMouseUp;
        }

        protected void OnMouseDown(object sender, Windows.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            Debug.WriteLine("OnMouseDown");

            isLeftMouseButtonDownOnWindow = true;
            origMouseDownPoint = e.GetCurrentPoint(this).Position;

            e.Handled = true;

            resetDragBorder();   
        }

        private void resetDragBorder()
        {
            dragSelectionBorder.Visibility = Visibility.Visible;
            Canvas.SetLeft(dragSelectionBorder, 0);
            Canvas.SetTop(dragSelectionBorder, 0);
            dragSelectionBorder.Width = 0;
            dragSelectionBorder.Height = 0;
        }

        protected void OnMouseMove(object sender, Windows.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            if (isDraggingSelectionRect)
            {
                Point curMouseDownPoint = e.GetCurrentPoint(this).Position;
                UpdateDragSelectionRect(origMouseDownPoint, curMouseDownPoint);

                e.Handled = true;
            }
            else if (isLeftMouseButtonDownOnWindow)
            {
                Point curMouseDownPoint = e.GetCurrentPoint(this).Position;
                double dragDistance = Math.Sqrt(Math.Pow((curMouseDownPoint.X - origMouseDownPoint.X),2) + Math.Pow((curMouseDownPoint.Y - origMouseDownPoint.Y),2));
                if (dragDistance > DragThreshold)
                {
                    isDraggingSelectionRect = true;

                    InitDragSelectionRect(origMouseDownPoint, curMouseDownPoint);
                }

                e.Handled = true;
            }
        }

        protected void OnMouseUp(object sender, Windows.UI.Xaml.Input.PointerRoutedEventArgs e)
        {
            if (isDraggingSelectionRect)
            {
                isDraggingSelectionRect = false;
                ApplyDragSelectionRect();

                e.Handled = true;
            }

            if (isLeftMouseButtonDownOnWindow)
            {
                isLeftMouseButtonDownOnWindow = false;
               
                e.Handled = true;
            }
        }

        /// <summary>
        /// Initialize the rectangle used for drag selection.
        /// </summary>
        private void InitDragSelectionRect(Point pt1, Point pt2)
        {
            UpdateDragSelectionRect(pt1, pt2);
        }

        /// <summary>
        /// Update the position and size of the rectangle used for drag selection.
        /// </summary>
        private void UpdateDragSelectionRect(Point pt1, Point pt2)
        {
            double x, y, width, height;

            //
            // Determine x,y,width and height of the rect inverting the points if necessary.
            // 

            if (pt2.X < pt1.X)
            {
                x = pt2.X;
                width = pt1.X - pt2.X;
            }
            else
            {
                x = pt1.X;
                width = pt2.X - pt1.X;
            }

            if (pt2.Y < pt1.Y)
            {
                y = pt2.Y;
                height = pt1.Y - pt2.Y;
            }
            else
            {
                y = pt1.Y;
                height = pt2.Y - pt1.Y;
            }

            //
            // Update the coordinates of the rectangle used for drag selection.
            //
            Canvas.SetLeft(dragSelectionBorder, x);
            Canvas.SetTop(dragSelectionBorder, y);
            dragSelectionBorder.Width = width;
            dragSelectionBorder.Height = height;
        }

        private void ApplyDragSelectionRect()
        {
            //dragSelectionCanvas.Visibility = Visibility.Collapsed;

            double x = Canvas.GetLeft(dragSelectionBorder);
            double y = Canvas.GetTop(dragSelectionBorder);
            double width = dragSelectionBorder.Width;
            double height = dragSelectionBorder.Height;

            MainViewModel vm = ServiceLocator.Current.GetInstance<MainViewModel>();
            vm.ContainerList.Add(new Model.SecureContainerModel((int)(x), (int)(y), (int)(width), (int)(height)));
            vm.ActualContainerList.Add(new Model.SecureContainerModel((int)(x / imageRatio), (int)(y / imageRatio), (int)(width / imageRatio), (int)(height / imageRatio)));

            dragSelectionBorder.Visibility = Visibility.Collapsed;
        }

        public void setImageRatio(float ratio)
        {
            imageRatio = ratio;
        }
    }
}
