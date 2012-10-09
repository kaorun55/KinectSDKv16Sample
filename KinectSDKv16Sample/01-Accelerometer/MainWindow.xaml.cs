using System;
using System.Linq;
using System.Windows;
using Microsoft.Kinect;

namespace _01_Accelerometer
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

                // 加速度センサーの値を定期的に読ませるために、
                // (使わないけど)とりあえずカラーカメラを動かしておく
                kinect.ColorStream.Enable();
                kinect.AllFramesReady += kinect_AllFramesReady;
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
        /// 表示データを更新する
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void kinect_AllFramesReady( object sender, AllFramesReadyEventArgs e )
        {
            Vector4 accelerometer = kinect.AccelerometerGetCurrentReading();
            textAccelerometerX.Text = accelerometer.X.ToString();
            textAccelerometerY.Text = accelerometer.Y.ToString();
            textAccelerometerZ.Text = accelerometer.Z.ToString();

            textAngle.Text = GetAccelerometerAngle().ToString();

            textTiltAngle.Text = kinect.ElevationAngle.ToString();
        }

        /// <summary>
        /// 加速度センサーの値からZ軸の角度を求める
        /// </summary>
        /// <returns></returns>
        int GetAccelerometerAngle()
        {
            // http://thinkit.co.jp/story/2011/11/11/2329?page=0,2
            Vector4 accelerometer = kinect.AccelerometerGetCurrentReading();
            return -(int)Math.Round( Math.Asin( accelerometer.Z ) * 180 / Math.PI );
        }

        /// <summary>
        /// チルト角度を設定する
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonBaseUpdate_Click_1( object sender, RoutedEventArgs e )
        {
            try {
                kinect.ElevationAngle = int.Parse( textBaseAngle.Text );
            }
            catch ( Exception ex ) {
                MessageBox.Show( ex.Message );
            }
        }
    }
}
