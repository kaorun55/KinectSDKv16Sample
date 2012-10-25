= Kinect for Windows SDK v1.6 の Tipsなど

== 座標系変換

Kinect for Windows SDK v1.5までは距離カメラ座標からスケルトン座標(その反対も)や、距離カメラ座標からカラーカメラ座標に変換するメソッドはKinectSensorにありました(Map～)。

Kinect for Windows SDK v1.6ではこれらの機能が CoordinateMapper クラスに移され、そちらを利用することが推奨されているようです。 なお、 CoordinateMapper クラスのインスタンスは、 KinectSensor.CoordinateMapper プロパティにて取得できます。

なお、v1.6では従来の変換メソッドに加えて、拡張された距離データである DepthImagePixel の変換もサポートしているため、v1.6までの5つが8つになっています。変換できる座標は次の通りです。DepthFrameとついているのがv1.6で追加された DepthImagePixel の変換になりますが、ColorFrame とついているものも DepthImagePixel の変換になるようです。 DepthPointとついているのが従来の距離データのようです。

また、変換後のデータの妥当性(変換後の座標に収まっているか)は、  KinectSensor.IsKnownPoint() で確認できるようです。

 * MapColorFrameToDepthFrame
 * MapColorFrameToSkeletonFrame
 * MapDepthFrameToColorFrame
 * MapDepthFrameToSkeletonFrame
 * MapDepthPointToColorPoint
 * MapDepthPointToSkeletonPoint
 * MapSkeletonPointToColorPoint
 * MapSkeletonPointToDepthPoint


== プレイヤーとスケルトントラッキングについて
Kinect for Windows SDK v1.6 で、距離データが4m以上を取得できるようになりましたが、プレイヤーおよびスケルトンのトラッキング範囲は従来通り、DepthImageStream.Rangeに依存して0.8m-4mまたは0.4m-3mのようです。

== VM(仮想マシン)対応について
結論から言うと、ガリガリ開発という用途は難しいかもしれませんが、MacユーザーがBootCampなしにKinect for Windows を試せる環境や、テストを少しでも効率化するための環境はできたのだと感じています。

=== VM対応について
こちらのページにVM対応について書かれています。

 * @<href>{http://msdn.microsoft.com/en-us/library/jj663795.aspx,Using Kinect for Windows with a Virtual Machine}

意訳するとこのようになります。

 * Kinect for Windows SDK がサポートするVM環境は Microsoft HyperV、VMWare、Parallels の3つです。これらの環境で動作できることによって、Kinect for Windows の有用性が高まります。また、自動テストなど開発のシナリオにおいても重要な役目を果たします。
 * VM上で動作するのはKinect for Windows のみです。Kinect for Xbox はサポートしません。
 * 1つのVM上で複数の Kinect for Windows の利用はサポートしません。
 * 1つのVM上で複数の Kinect for Windows を利用しようとした場合、2つ目以降は無視されます。再起動すると認識できるようになります。
 * 訳注：複数台のKinectを同時に利用できないが、複数のうちの1台を選択するのであれば可能ということ？
 * Kinect for Windows を Microsoft HyperV で動作させる場合、サポートする物理マシンのOSは Windows 8 Enterprise に限られます。 Kinect for Windows は Windows Server 上の HyperV では動作しません。
 ** 訳注：Kinect for Windows を Microsoft HyperV で動作させる場合にはRemoteFXという機能が必要だが、これをサポートするOSは Windows 8 Enterprise のみ。Windows ServerはそもそもKinect for Windowsが動作しないと思われる。

=== 動かしてみる
MacBook + Parallels で試したところ、カラー画像で20FPS、距離画像で4PFSといったところです。ちなみにMacBookは4年くらい前に買った Core 2 Duo + HDD + 2GB RAMなので、最近のMacであればもっと性能がでるのではないかと思います。

//image[vmmac][OSX 上の Parallels で動作させた]


VMWare Playerで試したところ、認識がするのですがカメラ画像が表示されませんでした。物理マシンはCore i7　+ SSDなので遅いということはなさそうなのですが。 この辺りは続報を待ちたいところです。Core i5のiMac上の Windows 8 では、カラーおよび距離カメラともに 30FPS でていました。しかし、距離カメラのデータが取得できない場合があったりと、多少バギーのようにも見受けられます。

//image[vmwin][Windows 8 上の VMWare Playerで動作させた]

