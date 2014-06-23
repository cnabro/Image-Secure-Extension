using isetool.ViewModel;
using Microsoft.Practices.ServiceLocation;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace isetool.Controls
{
    /// <summary>
    /// MultiSelectionControl.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MultiSelectionControl : Canvas
    {
        private bool isLeftMouseButtonDownOnWindow = false;
        private bool isDraggingSelectionRect = false;
        private Point origMouseDownPoint;
        private static readonly double DragThreshold = 5;

        //private Rectangle dragSelectionBorder;

        public MultiSelectionControl()
        {
            InitializeComponent();
        }

        protected override void OnMouseDown(MouseButtonEventArgs e)
        {
            base.OnMouseDown(e);

            Debug.WriteLine("OnMouseDown");

            if (e.ChangedButton == MouseButton.Left)
            {
                isLeftMouseButtonDownOnWindow = true;
                origMouseDownPoint = e.GetPosition(this);

                this.CaptureMouse();

                e.Handled = true;

                resetDragBorder();   
            }
        }

        private void resetDragBorder()
        {
            dragSelectionBorder.Visibility = Visibility.Visible;
            Canvas.SetLeft(dragSelectionBorder, 0);
            Canvas.SetTop(dragSelectionBorder, 0);
            dragSelectionBorder.Width = 0;
            dragSelectionBorder.Height = 0;
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);

            if (isDraggingSelectionRect)
            {
                Point curMouseDownPoint = e.GetPosition(this);
                UpdateDragSelectionRect(origMouseDownPoint, curMouseDownPoint);

                e.Handled = true;
            }
            else if (isLeftMouseButtonDownOnWindow)
            {
                Point curMouseDownPoint = e.GetPosition(this);
                var dragDelta = curMouseDownPoint - origMouseDownPoint;
                double dragDistance = Math.Abs(dragDelta.Length);
                if (dragDistance > DragThreshold)
                {
                    isDraggingSelectionRect = true;

                    InitDragSelectionRect(origMouseDownPoint, curMouseDownPoint);
                }

                e.Handled = true;
            }
        }

        protected override void OnMouseUp(MouseButtonEventArgs e)
        {
            base.OnMouseUp(e);

            Debug.WriteLine("OnMouseUp");

            if (e.ChangedButton == MouseButton.Left)
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
                    this.ReleaseMouseCapture();

                    e.Handled = true;
                }
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

            MultiSelectionCanvasViewModel vm = ServiceLocator.Current.GetInstance<MultiSelectionCanvasViewModel>();
            vm.ContainerList.Add(new Model.SecureContainerModel((int)x, (int)y, (int)width, (int)height));

            dragSelectionBorder.Visibility = System.Windows.Visibility.Collapsed;
        }
    }
}
