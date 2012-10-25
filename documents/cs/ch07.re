= カラーカメラの設定を使ってみる

//image[result][カラーカメラの設定値]


== 概要
カラーカメラの設定は ColorCameraSettings型として、 ColorImageStream.CameraSettings にあります。詳しい内容はこちら( http://msdn.microsoft.com/en-us/library/microsoft.kinect.colorcamerasettings_members.aspx ) を参照してもらうのが良いと思うのですが、一通り調べてみました。

KinectExplorerと同じようにグループ分けをして、それぞれの型、値(初期値)、最大値、最小値を表示しています。いちおう調べた意味(ほぼ直訳ですが)も入れています。


== 全体のコード
//list[camerasettings1][MainWindow.xaml]{
<Window x:Class="_05_ColorCameraSettings.MainWindow"
        xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
        xmlns:local="clr-namespace:_05_ColorCameraSettings"
        Title="MainWindow" SizeToContent="WidthAndHeight" >
    <Window.Resources>
        <x:Array x:Key="AutoExposureProperties" Type="{x:Type local:Property}">
            <local:Property Name="AutoExposure" Detail="自動露出機能の有効/無効" />

            <local:Property Name="PowerLineFrequency"
                Detail="電源の周波数:50/60Hz(自動露出機能の有効時のみ)" />

            <local:Property Name="BacklightCompensationMode" 
                Detail="逆光補正モード(自動露出機能の有効時のみ)" />

            <local:Property Name="Brightness" 
                Detail="明るさ(自動露出機能の有効時のみ)" />
            <local:Property Name="FrameInterval" 
                Detail="フレームレート？(自動露出機能の無効時のみ)" />
            <local:Property Name="ExposureTime" 
                Detail="露光時間(自動露出機能の無効時のみ)" />
            <local:Property Name="Gain" 
                Detail="ゲイン(自動露出機能の無効時のみ)" />
        </x:Array>

        <x:Array x:Key="AutoWhiteBalanceProperties" Type="{x:Type local:Property}">
            <local:Property Name="AutoWhiteBalance" 
                Detail="自動ホワイトバランスの有効/無効" />

            <local:Property Name="WhiteBalance" 
                Detail="ホワイトバランス(自動ホワイトバランスの無効時のみ)" />
            <local:Property Name="Contrast" 
                Detail="コントラスト" />
            <local:Property Name="Gamma" 
                Detail="ガンマ" />
            <local:Property Name="Hue" 
                Detail="色相" />
            <local:Property Name="Saturation" 
                Detail="彩度" />
            <local:Property Name="Sharpness" 
                Detail="シャープネス" />
        </x:Array>
    </Window.Resources>
    
    <StackPanel Orientation="Vertical">
        <GroupBox Header="自動露出">
            <DataGrid x:Name="AutoExposureProperties"  
                ItemsSource="{StaticResource AutoExposureProperties}" />
        </GroupBox>

        <GroupBox Header="自動ホワイトバランス">
            <DataGrid x:Name="AutoWhiteBalanceProperties"  
                ItemsSource="{StaticResource AutoWhiteBalanceProperties}" />
        </GroupBox>
    </StackPanel>
</Window>
//}

//list[camerasettings2][MainWindow.xaml.cs]{
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
//}
