#include <Windows.h>
#include <NuiApi.h>

#include <iostream>
#include <conio.h>

#include <opencv2\opencv.hpp>

void main()
{
	try {
		int count = 0;
		::NuiGetSensorCount( &count );
		if ( count == 0 ) {
			throw std::runtime_error( "利用可能なKinectがありません" );
		}

		INuiSensor* kinect;
		::NuiCreateSensorByIndex( 0, & kinect );

		kinect->NuiInitialize( NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX );

		HANDLE depthHandle;
		kinect->NuiImageStreamOpen( NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX,
			NUI_IMAGE_RESOLUTION_640x480, 0, 2, 0, &depthHandle );

		DWORD width = 0;
		DWORD height = 0;
		::NuiImageResolutionToSize( NUI_IMAGE_RESOLUTION_640x480, width, height );

		while ( true ) {
			// 距離カメラのフレームデータを取り出す
			NUI_IMAGE_FRAME depthFrame = { 0 };
			kinect->NuiImageStreamGetNextFrame( depthHandle, INFINITE, &depthFrame );

			// フレームデータを元に、拡張距離データを取得する
			BOOL nearMode = FALSE;
			INuiFrameTexture *frameTexture = 0;
			kinect->NuiImageFrameGetDepthImagePixelFrameTexture(
				depthHandle, &depthFrame, &nearMode, &frameTexture );

			// NUI_DEPTH_IMAGE_PIXELを取り出す
			NUI_LOCKED_RECT rect = { 0 };
			frameTexture->LockRect( 0, &rect, 0, 0 );

			//typedef struct _NUI_DEPTH_IMAGE_PIXEL
			//{
			//	USHORT playerIndex;
			//	USHORT depth;
			//} NUI_DEPTH_IMAGE_PIXEL;
			NUI_DEPTH_IMAGE_PIXEL* depthPixel = (NUI_DEPTH_IMAGE_PIXEL*)rect.pBits;

			// 表示する
			cv::Mat camera( height, width, CV_8UC3 );
			for ( int y = 0; y < height; ++y ) {
				for ( int x = 0; x < width; ++x ) {
					int depthIndex = (y * width) + x;
					int cameraIndex = depthIndex * 3;

					// プレイヤーを認識していな場合
					if ( depthPixel[depthIndex].playerIndex == 0 ) {
						unsigned char gray =
							(unsigned char)~(depthPixel[depthIndex].depth * 255 / 0x3FFF);

						// 4mより遠いところは赤っぽく
						if ( depthPixel[depthIndex].depth > 4000 ) {
							camera.data[cameraIndex] = gray;
							camera.data[cameraIndex + 1] = gray;
							camera.data[cameraIndex + 2] = 255;
						}
						// 4mより近い場合は通常モード
						else {
							camera.data[cameraIndex] = gray;
							camera.data[cameraIndex + 1] = gray;
							camera.data[cameraIndex + 2] = gray;
						}
					}
					// プレイヤーを認識した場合
					else {
						camera.data[cameraIndex] = 0;
						camera.data[cameraIndex + 1] = 255;
						camera.data[cameraIndex + 2] = 0;
					}
				}
			}

			cv::imshow( "02-ExtendedRangeDepthDataCpp", camera );

			kinect->NuiImageStreamReleaseFrame( depthHandle, &depthFrame );

			int key = cv::waitKey( 10 );
			if ( key == 'q' ) {
				break;
			}
		}
	}
	catch ( std::exception& ex ) {
		std::cout << ex.what() << std::endl;
	}
}
