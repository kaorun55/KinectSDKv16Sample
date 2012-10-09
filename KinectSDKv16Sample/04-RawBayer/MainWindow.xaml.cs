using System;
using System.Collections.Generic;
using System.Linq;
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
using Microsoft.Kinect;

namespace _04_RawBayer
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        KinectSensor kinect;

        public MainWindow()
        {
            InitializeComponent();

            Loaded += ( s, e ) => StartKinect();
            Unloaded += ( s, e ) => StopKinect();
        }

        private void StartKinect()
        {
            try {
                kinect = KinectSensor.KinectSensors
                    .FirstOrDefault( k => k.Status == KinectStatus.Connected );
                if ( kinect == null ) {
                    throw new Exception( "利用可能なKinectがありません" );
                }

                kinect.ColorFrameReady += kinect_ColorFrameReady;
                kinect.ColorStream.Enable( ColorImageFormat.RawBayerResolution640x480Fps30 );
                kinect.Start();
            }
            catch ( Exception ex ) {
                MessageBox.Show( ex.Message );
                Close();
            }
        }

        private void StopKinect()
        {
            if ( kinect != null ) {
                kinect.Stop();
                kinect = null;
            }
        }

        void kinect_ColorFrameReady( object sender, ColorImageFrameReadyEventArgs e )
        {
            // RawBayerデータを表示する
            using ( ColorImageFrame colorFrame = e.OpenColorImageFrame() ) {
                if ( colorFrame != null ) {
                    // 画像データを取得する
                    byte[] color = new byte[colorFrame.PixelDataLength];
                    colorFrame.CopyPixelDataTo( color );

                    // 画像を表示する(8bitのグレースケール)
                    imageRawBayer.Source = BitmapSource.Create(
                        colorFrame.Width, colorFrame.Height,
                        96, 96, PixelFormats.Gray8, null, color,
                        colorFrame.Width * colorFrame.BytesPerPixel );
                }
            }
        }
    }
}
