using System;
using System.Linq;
using System.Windows;
using Microsoft.Kinect;

namespace _05_ColorCameraSettings
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

            kinect = KinectSensor.KinectSensors
                .FirstOrDefault( k => k.Status == KinectStatus.Connected );

            foreach ( Property item in AutoExposureProperties.ItemsSource ) {
                item.Type = PropertyType<ColorCameraSettings>( item.Name );
                item.Value = GetProtertyValue( item.Name );
                item.Max = GetProtertyValue( "Max" + item.Name );
                item.Min = GetProtertyValue( "Min" + item.Name );
            }

            foreach ( Property item in AutoWhiteBalanceProperties.ItemsSource ) {
                item.Type = PropertyType<ColorCameraSettings>( item.Name );
                item.Value = GetProtertyValue( item.Name );
                item.Max = GetProtertyValue( "Max" + item.Name );
                item.Min = GetProtertyValue( "Min" + item.Name );
            }
        }

        /// <summary>
        /// プロパティの名前から、値を取得する
        /// </summary>
        /// <param name="propertyName"></param>
        /// <returns></returns>
        private string GetProtertyValue( string propertyName )
        {
            var value = PropertyInvoke( kinect.ColorStream.CameraSettings, propertyName );
            return (value != null) ?  value.ToString() : "---";
        }

        /// <summary>
        /// プロパティの名前の文字列から、プロパティの値を取得する
        /// http://devlabo.blogspot.jp/2010/12/c.html
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="instance"></param>
        /// <param name="propertyName"></param>
        /// <returns></returns>
        private object PropertyInvoke<T>( T instance, string propertyName )
        {
            var type = typeof( T );
            var property = type.GetProperty( propertyName );
            if ( property == null ) {
                return null;
            }

            return property.GetValue( instance );
        }

        /// <summary>
        /// プロパティの名前から、プロパティの型を取得する
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="propertyName"></param>
        /// <returns></returns>
        private string PropertyType<T>( string propertyName )
        {
            var type = typeof( T );
            var property = type.GetProperty( propertyName );
            if ( property == null ) {
                return null;
            }

            return property.PropertyType.ToString();
        }
    }

    /// <summary>
    /// 表示する項目
    /// </summary>
    public class Property
    {
        public string Name
        {
            get;
            set;
        }

        public string Type
        {
            get;
            set;
        }

        public string Value
        {
            get;
            set;
        }

        public string Max
        {
            get;
            set;
        }

        public string Min
        {
            get;
            set;
        }


        public string Detail
        {
            get;
            set;
        }
    }
}
