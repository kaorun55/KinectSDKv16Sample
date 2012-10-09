using System;
using System.Linq;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using Microsoft.Kinect;

namespace _02_ExtendedRangeDepthData
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

                kinect.DepthFrameReady += kinect_DepthFrameReady;
                kinect.DepthStream.Enable( DepthImageFormat.Resolution640x480Fps30 );
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

        /// <summary>
        /// 距離データの更新
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void kinect_DepthFrameReady( object sender, DepthImageFrameReadyEventArgs e )
        {
            using ( DepthImageFrame frame = e.OpenDepthImageFrame() ) {
                if ( frame != null ) {
                    // DepthImagePixel で取得すると、16bitの距離データ+プライヤーインデックスが取得できる
                    DepthImagePixel[] depth = new DepthImagePixel[frame.PixelDataLength];
                    frame.CopyDepthImagePixelDataTo( depth );

                    // 中心点の距離を表示する
                    int index = (frame.Height / 2) * frame.Width + (frame.Width / 2);
                    textDepth.Text = string.Format( "{0}mm", depth[index].Depth );

                    // 可視画像に変換する(14bitで16m)
                    // どこまでいけるかは不明だけどOpenNI時の10m弱くらいが限界？
                    short[] pixel = new short[frame.PixelDataLength];
                    for ( int i = 0; i < depth.Length; i++ ) {
                        pixel[i] = (short)~(depth[i].Depth * 0xFFFF / 0x3FFF);
                    }

                    imageDepth.Source = BitmapSource.Create( frame.Width, frame.Height, 96, 96,
                        PixelFormats.Gray16, null, pixel, frame.Width * frame.BytesPerPixel );
                }
            }
        }
    }
}
