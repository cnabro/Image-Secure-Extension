﻿using isetool.Util;
using isetool.ViewModel;
using IseWrapper;
using MahApps.Metro;
using MahApps.Metro.Controls;
using MahApps.Metro.Controls.Dialogs;
using Microsoft.Practices.ServiceLocation;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace isetool
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : MetroWindow
    {
        private BaseMetroDialog dialog; 

        public MainWindow()
        {
            InitializeComponent();

            SetBackgoround();
        }

        public void setModifyMode()
        {

        }

        public void SetBackgoround()
        {
            GeometryDrawing drawing = new GeometryDrawing();

            RectangleGeometry rect = new RectangleGeometry();
            rect.Rect = new Rect(0, 0, 5, 5);

            RectangleGeometry rect2 = new RectangleGeometry();
            rect2.Rect = new Rect(5, 5, 5, 5);

            GeometryGroup group = new GeometryGroup();
            group.Children.Add(rect);
            group.Children.Add(rect2);

            drawing.Geometry = group;
            drawing.Brush = System.Windows.Media.Brushes.Black;  

            DrawingBrush brush = new DrawingBrush();
            brush.Drawing = drawing;
            brush.Viewport = new Rect(0, 0, 0.03, 0.035);
            brush.TileMode = TileMode.Tile;

            gridBG.Background = brush;  
        }

        private void MetroWindow_Loaded(object sender, RoutedEventArgs e)
        {
            MainViewModel model = ServiceLocator.Current.GetInstance<MainViewModel>();
            model.ParseParameter();
        }
    }
}
