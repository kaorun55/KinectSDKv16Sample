= Kinect for Windows SDK v1.6 環境の構築
Kinect for Windows SDKの開発環境を構築します。

== 必要なソフトウェア
 * Windows 7または Windows 8(Kienct for Windows SDK v1.6以降)
 * Visual C# 2010 Express または その他の Visual Studio 製品
 * Visual Studio 2012 Express for Desktop または その他の Visual Studio 製品
 ** @<tt>{http://www.microsoft.com/visualstudio/jpn/downloads}
 * Kinect for Windows SDK v1.6 および Kinect for Windows Developer Toolkit v1.6
 ** @<tt>{http://www.microsoft.com/en-us/kinectforwindows/}
 * Kinect for Windows または Kinect for Xbox(開発用のみ)

== セットアップ
1. Kinect for Windows のサイト( @<tt>{http://www.microsoft.com/en-us/kinectforwindows/} )からSDKをダウンロードします。
//image[kinectsdk_1][SDKのダウンロードサイト]
//image[kinectsdk_2][SDKのダウンロードサイト]

2. Kinect for Windows SDK ( KinectSDK-v1.6-Setup.exe ) をインストールします。
//image[kinectsdk_3][SDKのダウンロードサイト]
//image[kinectsdk_5][SDKのダウンロードサイト]

3. Kinect for Windows Developer Toolkit ( KinectDeveloperToolkit-v1.6.0-Setup.exe ) をインストールします。
//image[kinectsdk_6][SDKのダウンロードサイト]
//image[kinectsdk_8][SDKのダウンロードサイト]


== 開発環境の設定
=== Visual C# 2010 Express
1. Visual C# 2010 Express を起動し、新しいプロジェクトを選択します。
//image[vs2010_1][SDKのダウンロードサイト]

2. WPFアプリケーションを選択し、プロジェクト名を入力します。
//image[vs2010_2][SDKのダウンロードサイト]

3. プロジェクトにある「参照設定」を右クリックし「参照の追加」を選択します。
//image[vs2010_3][SDKのダウンロードサイト]

4. 「.NET」タブから「Microsoft.Kienct」を選択します。
//image[vs2010_4][SDKのダウンロードサイト]


=== Visual Studio 2012 Express for Windows Desktop
1. Visual Studio 2012 Express for Windows Desktop を起動し、新しいプロジェクトを選択します。
//image[vs2012_1][SDKのダウンロードサイト]

2. WPFアプリケーションを選択し、プロジェクト名を入力します。
//image[vs2012_2][SDKのダウンロードサイト]

3. プロジェクトにある「参照設定」を右クリックし「参照の追加」を選択します。
//image[vs2012_3][SDKのダウンロードサイト]

4. 「アセンブリ」タブを選択し、検索ボックスに「Kienct」と入力します。 表示された「Microsoft.Kienct」にチェックを入れ「OK」を押します。
//image[vs2012_4][SDKのダウンロードサイト]
