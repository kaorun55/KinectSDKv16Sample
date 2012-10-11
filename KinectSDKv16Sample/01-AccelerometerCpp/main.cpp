#include <Windows.h>
#include <NuiApi.h>

#include <iostream>
#include <conio.h>

void main()
{
	try {
		int count = 0;
		::NuiGetSensorCount( &count );
		if ( count == 0 ) {
			throw std::runtime_error( "—˜—p‰Â”\‚ÈKinect‚ª‚ ‚è‚Ü‚¹‚ñ" );
		}

		INuiSensor* kinect;
		::NuiCreateSensorByIndex( 0, & kinect );

		while ( true ) {
			Vector4 accelerometer = { 0 };
			kinect->NuiAccelerometerGetCurrentReading( &accelerometer );

			std::cout << '\r' << accelerometer.x << ", "
				              << accelerometer.y << ", "
							  << accelerometer.z;

			if ( ::_kbhit() ) {
				break;
			}
		}
	}
	catch ( std::exception& ex ) {
		std::cout << ex.what() << std::endl;
	}
}
