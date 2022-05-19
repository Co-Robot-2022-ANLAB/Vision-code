//	zxcvbnm 눌러서 간단하게 inrange 조작할수있게 했읍니다.
/* //**********************************************
220519 키보드 이벤트 처리하기
220428에 string 있다;
*/ //**********************************************

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void camera_in();
void mouse_callback(int event, int x, int y, int flags, void *param);

Mat b_b(Mat input, int hh, int ss, int vv);
Mat just_HSV(Mat input);

int H, S, V;
Mat img_color;

int main(void)
{
	camera_in();

	
	return 0;
}

void camera_in()
{
	/*
	VideoCapture cap(0);	//0번 기본카메라 1번은다음거..

	if (!cap.isOpened()) {	//예외처리코드
		cerr << "Camera open failed!" << endl;
		return;
	}
	*/
//	cout << "Frame width: " << cvRound(cap.get(CAP_PROP_FRAME_WIDTH)) << endl;
//	cout << "Frame height: " << cvRound(cap.get(CAP_PROP_FRAME_HEIGHT)) << endl;

	Mat cap = imread("ALLCOLOR.jpg");	//@@@@@@@@@@@@@@@@


	Mat frame;
	int hh = 0, ss = 0, vv = 0;
	while (true) {
//		cap >> frame;	//공부필요	//@@@@@@@@@@@@@@@@
		frame = cap;
		if (frame.empty()) {	//예외처리
			cerr << "Image load failed!" << endl;
			break;
		}
//		img_color = frame.clone();	//@@@@@@@@@@@@@@@@
		img_color = cap;	//@@@@@@@@@@@@@@@@
//		flip(frame,frame,1);	//좌우반전(거울모드)
		imshow("frame", frame);
		imshow("frame_hsv", just_HSV(frame));
		setMouseCallback("frame_hsv", mouse_callback);
		int kc = waitKey(5000);
		if (kc == 27) // ESC key 0.5초단위로 refresh
			break;
		else if (kc == 'z') {	//1
			hh += 10;
			cout << "z" << hh << ss << vv;
			imshow("frame_b", b_b(frame, hh, ss, vv));
		}
		else if (kc == 'x') {	//2
			ss += 10;
			cout << "x" << hh << ss << vv;
			imshow("frame_b", b_b(frame, hh, ss, vv));
		}
		else if (kc == 'c') {	//3
			vv += 10;
			cout << "c" << hh << ss << vv;
			imshow("frame_b", b_b(frame, hh, ss, vv));
		}
		else if (kc == 'v') {	//4
			hh -= 10;
			cout << "v" << hh << ss << vv;
			imshow("frame_b", b_b(frame, hh, ss, vv));
		}
		else if (kc == 'b') {	//5
			ss -= 10;
			cout << "b" << hh << ss << vv;
			imshow("frame_b", b_b(frame, hh, ss, vv));
		}
		else if (kc == 'n') {	//6
			vv -= 10;
			cout << "n" << hh << ss << vv;
			imshow("frame_b", b_b(frame, hh, ss, vv));
		}
		else if (kc == 'm') {	//7
			hh = ss = vv = 0;
			cout << "m" << hh << ss << vv;
			imshow("frame_b", b_b(frame, hh, ss, vv));
		}
	}

	destroyAllWindows();
	return;
}

Mat b_b(Mat input, int hh, int ss, int vv)
{
	Mat output,mid;
	cvtColor(input, mid, COLOR_BGR2HSV);
	inRange(mid, Scalar(hh, ss, vv), Scalar(255, 255, 255), output);
	//	erode(output, output, Mat(), Point(-1, -1), 2);		//2번침식
//	dilate(output, output, Mat(), Point(-1, -1), 2);	//2번팽창
	return output;
}

Mat just_HSV(Mat input)
{
	Mat output;
	cvtColor(input, output, COLOR_BGR2HSV);
	return output;
}

void mouse_callback(int event, int x, int y, int flags, void *param)
{
	if (event == EVENT_LBUTTONDBLCLK)
	{
		Vec3b color_pixel = img_color.at<Vec3b>(y, x);

		Mat bgr_color = Mat(1, 1, CV_8UC3, color_pixel);

		Mat hsv_color;
		cvtColor(bgr_color, hsv_color, COLOR_BGR2HSV);

		H = hsv_color.at<Vec3b>(0, 0)[0];
		S = hsv_color.at<Vec3b>(0, 0)[1];
		V = hsv_color.at<Vec3b>(0, 0)[2];

		cout << "H= " << H << endl;
		cout << "S= " << S << endl;
		cout << "V= " << V << endl;
	}
}
