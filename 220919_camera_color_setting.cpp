/*
둘다 순차적으로 실행할 수 있는 프로그램
@@@@@	여기서 복사해야할것
1. 전역변수 네줄
2. 함수 calibration , color_setting , mouse_callback_c
@@@@@	실행방법
CAM_NUM 변수 설정해주고 실행하면됨
esc키로 종료하면 A B C D 에 calibration 한 값 저장됨.
다른 변수도 자동으로 저장됨.
*/

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

//전역변수 선언
int CAM_NUM = 0;
int A = 130, B = 40, C = 384, D = 380;
Mat cc_frame;
int H, S, V, hh = 20, ss = 130, vv = 80, HH = 110, SS = 255, VV = 255;

//함수 선언
void calibration();
void color_setting();
void mouse_callback_c(int event, int x, int y, int flags, void *param);

int main(void)
{
	calibration();

	color_setting();

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
		c_frame = imread("c_test_frame_3.png");	//테스트용!!!
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

void color_setting()
{
	printf("\n\n\nColor Setting Initiating...\n\n\n");
	VideoCapture cap(CAM_NUM);	//
	if (!cap.isOpened()) {	//예외처리
		cerr << "Camera open failed!" << endl;
		return;
	}
	Mat c_frame_b;
	while (true)
	{
		cap >> cc_frame;
		cc_frame = imread("c_test_frame_3.png");	//테스트용!!!
		cc_frame = cc_frame(Rect(A, B, C, D));
		imshow("cc_frame", cc_frame);
		int wait_time = 3000;
		int kc = waitKey(wait_time);	//refresh-rate
		if (kc == 27) {					//ESC key
			cout << "esc 감지" << endl;
			printf("\n\n\nColor Setting Terminated\n\n\n");
			break;
		}
		else if (kc == 'z') {
			printf("hh : ");
			scanf_s("%d", &hh);
		}
		else if (kc == 'x') {
			printf("ss : ");
			scanf_s("%d", &ss);
		}
		else if (kc == 'c') {
			printf("vv : ");
			scanf_s("%d", &vv);
		}
		else if (kc == 'a') {
			printf("HH : ");
			scanf_s("%d", &HH);
		}
		else if (kc == 's') {
			printf("SS : ");
			scanf_s("%d", &SS);
		}
		else if (kc == 'd') {
			printf("VV : ");
			scanf_s("%d", &VV);
		}
		cvtColor(cc_frame, cc_frame, COLOR_BGR2HSV);
		inRange(cc_frame, Scalar(hh, ss, vv), Scalar(HH, SS, VV), c_frame_b);
		printf("hh : %3d\tss : %3d\tvv : %3d\nHH : %3d\tSS : %3d\tVV : %3d\n\n", hh, ss, vv, HH, SS, VV);
		imshow("c_frame_b", c_frame_b);
		setMouseCallback("c_frame_b", mouse_callback_c);
	}
	printf("color setting result\nhh : %3d\tss : %3d\tvv : %3d\nHH : %3d\tSS : %3d\tVV : %3d\n", hh, ss, vv, HH, SS, VV);
	waitKey();
	destroyAllWindows();
	return;
}

void mouse_callback_c(int event, int x, int y, int flags, void *param)
{
	if (event == EVENT_LBUTTONDBLCLK)
	{
		Vec3b color_pixel = cc_frame.at<Vec3b>(y, x);

		Mat bgr_color = Mat(1, 1, CV_8UC3, color_pixel);

		Mat hsv_color;
		cvtColor(bgr_color, hsv_color, COLOR_BGR2HSV);

		H = hsv_color.at<Vec3b>(0, 0)[0];
		S = hsv_color.at<Vec3b>(0, 0)[1];
		V = hsv_color.at<Vec3b>(0, 0)[2];

		cout << "H= " << H << endl;
		cout << "S= " << S << endl;
		cout << "V= " << V << endl << endl;
	}
}