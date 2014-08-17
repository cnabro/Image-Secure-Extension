using IseWrapperWP;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.ApplicationModel.Activation;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Graphics.Imaging;
using Windows.Storage.Pickers;
using Windows.Storage.Streams;
using Windows.UI.Core;
using Windows.UI.Popups;
using Windows.UI.ViewManagement;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Media.Imaging;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=391641

namespace IseStudio
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
        {
            this.InitializeComponent();

            this.NavigationCacheMode = NavigationCacheMode.Required;
            ApplicationView.GetForCurrentView().SetDesiredBoundsMode(ApplicationViewBoundsMode.UseCoreWindow);
            
        }

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.
        /// This parameter is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
            
        }

        public async void fileOpen()
        {
            //FileOpenPicker openPicker = new FileOpenPicker();
            //openPicker.ViewMode = PickerViewMode.List;
            ////openPicker.SuggestedStartLocation = PickerLocationId.ComputerFolder;
            //openPicker.FileTypeFilter.Add(".jpg");
            //openPicker.FileTypeFilter.Add(".jpeg");
            //openPicker.FileTypeFilter.Add(".png");
            //openPicker.FileTypeFilter.Add(".pngx");
            //openPicker.FileTypeFilter.Add(".jpgx");

            //openPicker.PickSingleFileAndContinue();

            
            //JpgxDecompressContainer jdc = ImageSecureExtention.getJpgxContainer("D:/test_d2.jpgx", "test");
            PngxDecompressContainer jdc = ImageSecureExtention.getPngxContainer("D:/ise_studio.pngx", "test");
            BitmapImage image = await jdc.getImageAsync();
            sourceImage.Source = image;


            Debug.WriteLine("set source");
        }

        public async void SetImageFromByteArray(byte[] data, Windows.UI.Xaml.Controls.Image image)
        {

            using (InMemoryRandomAccessStream raStream =
                new InMemoryRandomAccessStream())
            {
                using (DataWriter writer = new DataWriter(raStream))
                {
                    // Write the bytes to the stream
                    writer.WriteBytes(data);

                    // Store the bytes to the MemoryStream
                    await writer.StoreAsync();

                    // Not necessary, but do it anyway
                    await writer.FlushAsync();

                    // Detach from the Memory stream so we don't close it
                    writer.DetachStream();
                }

                raStream.Seek(0);

                BitmapImage bitMapImage = new BitmapImage();
                bitMapImage.SetSource(raStream);

                image.Source = bitMapImage;
            }
        }
         
        public async void Continue(FileOpenPickerContinuationEventArgs args)
        {
            if (args.Files.Count > 0)
            {
                //MessageDialog dialog = new MessageDialog("Picked photo: " + args.Files[0].Path);
                //await dialog.ShowAsync();

                //List<SecureContainer> scList = new List<SecureContainer>();
                //scList.Add(new SecureContainer(100, 100, 50, 50));

                //ImageSecureExtention.makePNGX("D:/test333.png", scList, "test");
                
            }
            else
            {
                MessageDialog dialog = new MessageDialog("Canceled");
                await dialog.ShowAsync();
            }
        }

        private void OpenBtn_Click(object sender, RoutedEventArgs e)
        {
            fileOpen();
        }
    }
}
