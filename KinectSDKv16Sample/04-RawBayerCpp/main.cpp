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

        kinect->NuiInitialize( NUI_INITIALIZE_FLAG_USES_COLOR );

        HANDLE imageHandle;
        kinect->NuiImageStreamOpen( NUI_IMAGE_TYPE_COLOR_RAW_BAYER,
            NUI_IMAGE_RESOLUTION_640x480, 0, 2, 0, &imageHandle );

        DWORD width = 0;
        DWORD height = 0;
        ::NuiImageResolutionToSize( NUI_IMAGE_RESOLUTION_640x480, width, height );

        while ( true ) {
            // カラーカメラのフレームデータを取り出す
            NUI_IMAGE_FRAME imageFrame = { 0 };
            kinect->NuiImageStreamGetNextFrame( imageHandle, INFINITE, &imageFrame );

            NUI_LOCKED_RECT rect = { 0 };
            imageFrame.pFrameTexture->LockRect( 0, &rect, 0, 0 );

            // 表示する
            cv::Mat camera( height, width, CV_8U, rect.pBits );
            cv::imshow( "04-RawBayerCpp", camera );

            kinect->NuiImageStreamReleaseFrame( imageHandle, &imageFrame );

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
