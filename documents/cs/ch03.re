= 加速度センサーを使ってみる
== 概要
v1.6で新たに取得できるようになった、3軸加速度センサーの値を見てみましょう。加速度センサーは特に設定など必要なく、値が欲しいときにKinectSensor.AccelerometerGetCurrentReading()を呼び出すとVector4として加速度の値が返ってきます。それぞれ軸の方向は次のようになっています。

//image[Accelerometer][加速度センサーの座標系]

単純に値を表示するだけであれば、次のようにします。

//list[accelerometer1][加速度センサーの値を取得する]{
Vector4 accelerometer = kinect.AccelerometerGetCurrentReading();
textAccelerometerX.Text = accelerometer.X.ToString();
textAccelerometerY.Text = accelerometer.Y.ToString();
textAccelerometerZ.Text = accelerometer.Z.ToString();
//}


この加速度センサーの値から現在のKinectの角度を取得してみましょう。加速度センサーの扱いは、Windows Phoneと同じようにできるので、こちらのサイト( http://thinkit.co.jp/story/2011/11/11/2329?page=0,2 )を参考に角度を求めてみました。座標系が異なるため軸と値を少しいじっています。

//list[accelerometer2][加速度センサーの値を角度に変換する]{
int GetAccelerometerAngle()
{
    // http://thinkit.co.jp/story/2011/11/11/2329?page=0,2
    Vector4 accelerometer = kinect.AccelerometerGetCurrentReading();
    return -(int)Math.Round( Math.Asin( accelerometer.Z ) * 180 / Math.PI );
}
//}


これをやっていて初めて気づいたのですが、KinectSensor.ElevationAngleで取得される角度は、上記GetAccelerometerAngle()で取得される値と同じなのですね。ずっとチルトの角度だと思っていたのですが、水平位置に対する角度だったようです。台座をいろいろな角度で持ってみてKinectSensor.ElevationAngleを0に設定すると、Kinectは水平方向に動きました。

== 全体のコード
//list[accelerometer3][MainWindow.xaml]{
<Window x:Class="_01_Accelerometer.MainWindow"
        xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation"
        xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml"
        Title="MainWindow" Height="350" Width="525">
    <Viewbox>
        <Grid>
            <Grid.ColumnDefinitions>
                <ColumnDefinition Width="180" />
                <ColumnDefinition Width="100" />
                <ColumnDefinition Width="*" />
                <ColumnDefinition Width="10" />
            </Grid.ColumnDefinitions>
            <Grid.RowDefinitions>
                <RowDefinition Height="*" />
                <RowDefinition Height="*" />
                <RowDefinition Height="*" />
                <RowDefinition Height="*" />
                <RowDefinition Height="*" />
                <RowDefinition Height="*" />
            </Grid.RowDefinitions>

            <TextBlock Text="X：" HorizontalAlignment="Right" Grid.Column="0" Grid.Row="0" />
            <TextBlock Name="textAccelerometerX" Grid.Column="1" Grid.Row="0" />

            <TextBlock Text="Y：" HorizontalAlignment="Right" Grid.Column="0" Grid.Row="1"/>
            <TextBlock Name="textAccelerometerY"  Grid.Column="1" Grid.Row="1" />

            <TextBlock Text="Z：" HorizontalAlignment="Right" Grid.Column="0" Grid.Row="2"/>
            <TextBlock Name="textAccelerometerZ"  Grid.Column="1" Grid.Row="2"  />

            <TextBlock Text="加速度センサーから取得した角度：" HorizontalAlignment="Right" Grid.Column="0" Grid.Row="3"/>
            <TextBlock Name="textAngle"  Grid.Column="1" Grid.Row="3"  />

            <TextBlock Text="チルト角度(現在の値)：" HorizontalAlignment="Right" Grid.Column="0" Grid.Row="4"/>
            <TextBlock Name="textTiltAngle"  Grid.Column="1" Grid.Row="4" />

            <TextBlock Text="チルト角度(変更する値)：" HorizontalAlignment="Right" Grid.Column="0" Grid.Row="5"/>
            <TextBox Name="textBaseAngle"  Grid.Column="1" Grid.Row="5" />
            <Button Name="buttonBaseUpdate" Content="更新" Click="buttonBaseUpdate_Click_1"  Grid.Column="2" Grid.Row="5" />
        </Grid>
    </Viewbox>
</Window>
//}

//list[accelerometer3][MainWindow.xaml]{
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
//}
