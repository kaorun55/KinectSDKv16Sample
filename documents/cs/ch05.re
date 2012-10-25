=  赤外線をいろいろ使ってみる
v1.6で利用可能になった、赤外線画像と、赤外線エミッタのON/OFF切り替えを行ってみます。赤外線画像はカラーストリームのデータとして取得できる16bitグレースケールのデータになります。カラーストリームを利用するため、カラーカメラのデータと赤外線データは排他になるようです。

//image[result][画像化された赤外線データ]


== 概要
=== 赤外線画像のためのカラーカメラ設定
繰り返しになりますが、赤外線の画像はカラーストリームを利用するので、これを取得したい場合にはColorImageStream.Enable()のColorImageFormatとして設定します。ColorImageFormatは今回の更新で次の項目が追加されています。それぞれ赤外線画像と、生のカラーデータです。ここでは赤外線画像を取得するため、InfraredResolution640x480Fps30を設定します。

 * InfraredResolution640x480Fps30
 * RawBayerResolution640x480Fps30
 * RawBayerResolution1280x960Fps12

//list[Infrared1][赤外線画像を取得する]{
kinect.AllFramesReady += kinect_AllFramesReady;
kinect.ColorStream.Enable( ColorImageFormat.InfraredResolution640x480Fps30 );
kinect.DepthStream.Enable( DepthImageFormat.Resolution640x480Fps30 );
kinect.Start();
//}

=== 赤外線画像の表示
更新されたカラーストリームから、赤外線画像を取り出し表示します。基本的な流れはRGBデータと同じで、ColorImageFrame.CopyPixelDataTo()でデータを取得し、BitmapSource.Create()で画像化して表示します。ただし、RGBデータは32bitのBGRデータに対して、赤外線画像データは16bitのグレースケールになるので、データ長や、表示のためのフォーマットが変わります。データ長(ColorImageFrame.PixelDataLength)やピクセルあたりのバイト数(ColorImageFrame.BytesPerPixel)はColorImageFrameのデータを使っていれば変更ありません。Bitmapデータのフォーマットのみ「PixelFormats.Gray16」になります。

//list[Infrared2][赤外線画像を表示する]{
// 赤外線画像を表示する
using ( ColorImageFrame colorFrame = e.OpenColorImageFrame() ) {
    if ( colorFrame != null ) {
        // 赤外線画像データを取得する
        byte[] color = new byte[colorFrame.PixelDataLength];
        colorFrame.CopyPixelDataTo( color );

        // 赤外線画像を表示する(16bitのグレースケール)
        imageInfrared.Source = BitmapSource.Create( 
            colorFrame.Width, colorFrame.Height,
            96, 96, PixelFormats.Gray16, null, color,
            colorFrame.Width * colorFrame.BytesPerPixel );
    }
}
//}


=== 赤外線エミッタのON/OFFを切り替える
赤外線のON/OFFを切り替えます。この設定はKinectSensor.ForceInfraredEmitterOffにありますので、チェックボックスによって切り替わるようにします。なお、KinectSensor.ForceInfraredEmitterOffはtrueでOFF,falseでONになっているので注意してください。

//list[Infrared3][赤外線エミッタのON/OFFを切り替える]{
private void CheckBox_Checked_1( object sender, RoutedEventArgs e )
{
    kinect.ForceInfraredEmitterOff = !(bool)checkInfrared.IsChecked;
}
//}

赤外線をOFFにすると、赤外線画像も、距離データも取得できなくなるため、画面は真っ黒になります。

//image[infraedoff][赤外線エミッタをOFFにする]


また、赤外線画像と距離画像を重ねてみると、奇麗に重なります。両方とも距離カメラのデータを使ってるので、意図した結果ですが、念のため。

//image[depth][赤外線画像と距離画像を重ねる]

== 全体のコード
//list[Infrared4][MainWindow.xaml]{
<Window x:Class="_03_Infrared.MainWindow"
        xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
        Title="MainWindow" SizeToContent="WidthAndHeight">
    <Grid>
        <StackPanel Orientation="Vertical">
            <CheckBox Name="checkInfrared" Content="赤外線を利用する"
                    Unchecked="CheckBox_Checked_1" Checked="CheckBox_Checked_1"/>
            <StackPanel Orientation="Horizontal">
                <Image Name="imageInfrared" Width="640" Height="480" />
                <Image Name="imageDepth" Width="640" Height="480" />
            </StackPanel>
        </StackPanel>
    </Grid>
</Window>
//}

//list[Infrared5][MainWindow.xaml.cs]{
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
                    imageInfrared.Source = BitmapSource.Create(
                        colorFrame.Width, colorFrame.Height,
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

                    imageDepth.Source = BitmapSource.Create(
                        depthFrame.Width, depthFrame.Height,
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
}//}
