//통합할 프로그램 0701 이진화까지만 완성시킴.

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void camera_in();
Mat b_b(Mat input, int hh, int ss, int vv, int hhh, int sss, int vvv);

int main()
{
	cout << "Hello OpenCV " << CV_VERSION << endl;
	
	camera_in();

	waitKey();
	return 0;
}

void camera_in()
{
	VideoCapture cap(1);	//0번 기본카메라(웹캠) 1번은다음거..
	if (!cap.isOpened()) {	//예외처리코드
		cerr << "Camera open failed!" << endl;
		return;
	}

	Mat frame;
	while (true) {
		cap >> frame;	//@@@@@@@@@@@@@@@@
		if (frame.empty()) {	//예외처리
			cerr << "Image load failed!" << endl;
			break;
		}

//############################################################################################

//		flip(frame,frame,1);	//좌우반전(거울모드)
		imshow("frame", frame);
//		imshow("frame_b", b_b(frame, 80, 170, 100, 100, 255, 205));
		imshow("frame_b", b_b(frame, 80, 170, 100, 100, 255, 205));
		int kc = waitKey(5);	// 5ms refresh-rate
		if (kc == 27) { // ESC key
			cout << "esc 감지" << endl;
			break;
		}

//############################################################################################

	}
	destroyAllWindows();
	return;
}

Mat b_b(Mat input, int hh, int ss, int vv, int hhh, int sss, int vvv)
{
	Mat output, mid;
	cvtColor(input, mid, COLOR_BGR2HSV);
	inRange(mid, Scalar(hh, ss, vv), Scalar(hhh, sss, vvv), output);
//	erode(output, output, Mat(), Point(-1, -1), 2);		//2번침식
//	dilate(output, output, Mat(), Point(-1, -1), 2);	//2번팽창
	return output;
}
