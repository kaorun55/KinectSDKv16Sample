= 拡張された距離データを取得する

//image[result][拡張された距離データを表示する]

== 概要
続いて拡張された距離データを取得してみます。v1.5までの距離データは、1ピクセルあたり16bitのうち、上位13bitを距離データ、下位3bitをプレイヤーインデックスとして利用していました。13bitの距離データも、最上位ビットは使用しないため、実際には12bitのデータであるため、4096までのデータしか取得することができませんでした。

v1.6では新たにDepthImagePixelとして、16bitの距離データとプレイヤーインデックスを持つ構造体が定義され、DepthImageFrame.CopyDepthImagePixelDataTo()で全体の距離データを取得することができるようになりました。これによって4000mm(4m)以上の距離データを取得できるようになりました。ただし、通常の範囲外の距離データについては、範囲内のデータ(800-4000mm)に比べて精度が低くなるようです。

上限および下限値はDepthImageStream.MinDepthおよびDepthImageStream.MaxDepthに定義されていますが、v1.5までと値が変わらないため正確な値は不明です。実測したところでは大体次のようになっています(OpenNIでKinectを利用していた時の値と大体同じです)。

Defaultモード：490mm-9300mm
Nearモード：450mm-8300mm
なお、プレイヤーおよびスケルトンの追跡は、拡張された距離データは適用されず、従来通り0.8m-4mまたは0.4m-3mになるようです。

DepthImagePixelコード上の定義は次のようになっています。
//image[DepthImagePixel][DepthImagePixelの定義]


コード上でもそれほど変わらず、従来はshortの配列に対してDepthImageFrame.CopyPixelDataTo()で取得していた部分を、DepthImagePixelの配列に対してDepthImageFrame.CopyDepthImagePixelDataTo()で取得するようになっています。DepthImagePixelではDepthとPlayerがそれぞれ定義されているため、v1.5までのようにビットシフトする必要もなくなりました。ここでは中心点の距離を表示しています。


//list[depth1][MainWindow.xaml]{
void kinect_DepthFrameReady( object sender, DepthImageFrameReadyEventArgs e )
{
    using ( DepthImageFrame frame = e.OpenDepthImageFrame() ) {
        if ( frame == null ) {
            return;
        }

        // DepthImagePixel で取得すると、16bitの距離データ+プレイヤーインデックス
        // が取得できる
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
//}

== 全体のコード
//list[depth2][MainWindow.xaml]{
<Window x:Class="_02_ExtendedRangeDepthData.MainWindow"
        xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
        Title="MainWindow" SizeToContent="WidthAndHeight">
    <Grid>
        <Image Name="imageDepth" Width="640" Height="480" />
        <Ellipse Stroke="Green" StrokeThickness="20"
                HorizontalAlignment="Center" VerticalAlignment="Center" />
        <TextBlock Name="textDepth" FontSize="40" TextAlignment="Right"
                HorizontalAlignment="Center" VerticalAlignment="Center" 
                Margin="220,0,0,0" Width="200" />
    </Grid>
</Window>
//}

//list[depth3][MainWindow.xaml.cs]{
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
                kinect.DepthStream.Range = DepthRange.Near;
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
                if ( frame == null ) {
                    return;
                }

                // DepthImagePixel で取得すると、16bitの距離データ+プライヤーインデックス
                // が取得できる
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
                    PixelFormats.Gray16, null, pixel,  frame.Width * frame.BytesPerPixel );
            }
        }
    }
}//}
