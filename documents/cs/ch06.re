=  生のBayerデータを使ってみる
v1.6ではデコード前のカラーカメラのデータが取得できるようになりました。Bayerというデータで、検索しているとカメラ関係のがヒットします。Bayerデータを取得することで、RGBデータへの変換を自前でCPUなりGPUなりで行うことができるようになります。ここではRawBayerデータで取得して表示してみます。

//image[result][生のBayerデータ]


== 概要
=== カラーカメラ設定
赤外線と同様にRawBayerデータの取得にはカラーストリームを利用するので、ColorImageStream.Enable()のColorImageFormatとして設定します。RawBayerデータの取得には解像度によって2種類あるので、必要な方を設定します。

 * RawBayerResolution640x480Fps30
 * RawBayerResolution1280x960Fps12

//list[Bayer1][Bayerデータを取得する]{
kinect.ColorFrameReady += kinect_ColorFrameReady;
kinect.ColorStream.Enable( ColorImageFormat.RawBayerResolution640x480Fps30 );
kinect.Start();
//}


=== Bayerデータの表示
更新されたカラーストリームから、Bayerデータを取り出し表示します。基本的な流れはRGBデータと同じで、ColorImageFrame.CopyPixelDataTo()でデータを取得し、BitmapSource.Create()で画像化して表示します。ただし、RGBデータは32bitのBGRデータに対して、Bayerデータは8bitになるので、データ長や、表示のためのフォーマットが変わります。データ長(ColorImageFrame.PixelDataLength)やピクセルあたりのバイト数(ColorImageFrame.BytesPerPixel)はColorImageFrameのデータを使っていれば変更ありません。Bitmapデータのフォーマットのみ「PixelFormats.Gray8」になります。

//list[Bayer2][Bayerデータを表示する]{
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
//}


== 全体のコード
//list[Bayer4][MainWindow.xaml]{
﻿<Window x:Class="_04_RawBayer.MainWindow"
        xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
        Title="MainWindow" SizeToContent="WidthAndHeight">
    <Grid>
        <Image Name="imageRawBayer" Width="640" Height="480" />
    </Grid>
</Window>
//}

//list[Bayer5][MainWindow.xaml.cs]{
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
