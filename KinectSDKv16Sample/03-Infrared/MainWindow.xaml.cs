using System;
using System.Linq;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using Microsoft.Kinect;

namespace _03_Infrared
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

                kinect.AllFramesReady += kinect_AllFramesReady;
                kinect.ColorStream.Enable( ColorImageFormat.InfraredResolution640x480Fps30 );
                kinect.DepthStream.Enable( DepthImageFormat.Resolution640x480Fps30 );
                kinect.Start();

                checkInfrared.IsChecked = !kinect.ForceInfraredEmitterOff;
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

        void kinect_AllFramesReady( object sender, AllFramesReadyEventArgs e )
        {
            // 赤外線画像を表示する
            using ( ColorImageFrame colorFrame = e.OpenColorImageFrame() ) {
                if ( colorFrame != null ) {
                    // 赤外線画像データを取得する
                    byte[] color = new byte[colorFrame.PixelDataLength];
                    colorFrame.CopyPixelDataTo( color );

                    // 赤外線画像を表示する(16bitのグレースケール)
                    imageInfrared.Source = BitmapSource.Create( colorFrame.Width, colorFrame.Height,
                        96, 96, PixelFormats.Gray16, null, color,
                        colorFrame.Width * colorFrame.BytesPerPixel );
                }
            }

            // 距離データを表示する
            using ( DepthImageFrame depthFrame = e.OpenDepthImageFrame() ) {
                if ( depthFrame != null ) {
                    // 可視画像に変換する
                    short[] depth = new short[depthFrame.PixelDataLength];
                    depthFrame.CopyPixelDataTo( depth );

                    for ( int i = 0; i < depth.Length; i++ ) {
                        depth[i] = (short)~depth[i];
                    }

                    imageDepth.Source = BitmapSource.Create( depthFrame.Width, depthFrame.Height,
                        96, 96, PixelFormats.Gray16, null, depth,
                        depthFrame.Width * depthFrame.BytesPerPixel );
                }
            }
        }

        /// <summary>
        /// 赤外線のON/OFF切り替え
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CheckBox_Checked_1( object sender, RoutedEventArgs e )
        {
            kinect.ForceInfraredEmitterOff = !(bool)checkInfrared.IsChecked;
        }
    }
}
