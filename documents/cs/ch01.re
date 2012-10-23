= Kinect for Windows SDK v1.6 の変更点
Kinect for Windows SDK v1.6 の変更点は以下の通りです。

 * 環境
 ** Windows  8 および Visual Studio 2012、Microsoft .NET Framework 4.5に正式に対応
 ** VMのサポート
 *** Microsoft Hyper-V, VMWare, Parallels
 * SDK
 ** 3軸加速度センサーの値を取得できる
 ** 4mを超えた距離データの取得ができる(ただし、4m以上の精度は低くなる)
 ** カラーカメラのホワイトバランスや露出をアプリケーションから設定できる
 ** カラーカメラの生データを取得することができる
 ** 赤外線
 *** 赤外線画像を取得できる
 **** カラーカメラのキャリブレーションしたい場合
 **** 暗所でグレースケール画像を取得した場合
 *** 赤外線のON/OFFを制御可能になった
 **** 複数のセンサーで重複した場所を映すことができる
 ** 一つのアプリケーションで複数台のKinectを接続した際の、スケルトンが一台しか利用できない制限が解除されている
 *** 一台当たりの認識人数(2人)は変わりないようなので、2人×4台で8人までは1アプリケーションで認識できるはず
 * Developer Toolkit
 ** 「Human Interface Guidelines」に沿ったサンプルが追加(Basic User Interactions – WPF)
 ** C#のWPFと、C++のDirect3Dを組み合わせたサンプルが追加
 * 追加されたクラスなど(C#)
 ** クラス
 *** ColorCameraSettings
 *** CoordinateMapper
 ** 構造体
 *** DepthImagePixel
 ** 列挙体
 *** BacklightCompensationMode
 *** PowerLineFrequency
 ** メソッド、プロパティ
 *** KinectSensor
 **** AccelerometerGetCurrentReading()
 **** IsKnownPoint()
 **** CoordinateMapper
 **** ForceInfraredEmitterOff
 *** ColorImageStream
 **** CameraSettings
 *** DepthImageFrame
 **** CopyDepthImagePixelDataTo()
