/*
@@@@@	여기서 복사해야할것
1. 전역변수 두줄
2. 함수 calibration
@@@@@	실행방법
CAM_NUM , A B C D 변수 설정해주고 실행하면됨
esc키로 종료하면 A B C D 에 calibration 한 값 저장됨.
*/

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

//전역변수 선언 , ABCD는 자연수범위로설정
int CAM_NUM = 0;
int A = 130, B = 40, C = 384, D = 380;

//함수 선언
void calibration();

int main()
{	
	calibration();

	return 0;
}

void calibration()
{
	printf("\n\n\nVideo Calibration Initiating...\n\n\n");
	VideoCapture cap(CAM_NUM);
	if (!cap.isOpened()) {	//예외처리
		cerr << "Camera open failed!" << endl;
		return;
	}
	Mat c_frame;
	int interval = 1;
	//buffer
	int a = A, b = B, c = C, d = D;
	while (true)
	{
		cap >> c_frame;
//		c_frame = imread("c_test_frame_3.png");	//테스트용!!!
		imshow("c_frame", c_frame);
		int wait_time = 3000;
		int kc = waitKey(wait_time);	//refresh-rate
		if (kc == 27) {					//ESC key
			cout << "esc 감지" << endl;
			printf("\n\n\nVideo Calibration Terminated\n\n\n");
			break;
		}
		else if (kc == 'z') {
			a += interval;
		}
		else if (kc == 'x') {
			a -= interval;
		}
		else if (kc == 'c') {
			b += interval;
		}
		else if (kc == 'v') {
			b -= interval;
		}
		else if (kc == 'a') {
			c += interval;
		}
		else if (kc == 's') {
			c -= interval;
		}
		else if (kc == 'd') {
			d += interval;
		}
		else if (kc == 'f') {
			d -= interval;
		}
		else if (kc == 'q') {
			interval = 1;
			printf("interval = %d\n", interval);
		}
		else if (kc == 'w') {
			interval = 5;
			printf("interval = %d\n", interval);
		}
		else if (kc == 'e') {
			interval = 15;
			printf("interval = %d\n", interval);
		}
		//예외처리
		if (a + c > 640 || b + d > 480 || a <= 0 || b <= 0 || c <= 0 || d <= 0) {
			a = A, b = B, c = C, d = D;
		}
		else {
			A = a, B = b, C = c, D = d;
		}
		printf("A : %d\tB : %d\tC : %d\tD : %d\t\n", A, B, C, D);
		c_frame = c_frame(Rect(A, B, C, D));
		imshow("c_frame_Rect", c_frame);
		//printf("press key Z X C V A S D F Q W E\n");
		printf("\n");
	}
	printf("cali result\nA : %3d\tB : %3d\tC : %3d\tD : %3d\t\n", A, B, C, D);
	waitKey();
	destroyAllWindows();
	return;
}