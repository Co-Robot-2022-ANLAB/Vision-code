//������ ���α׷� 0701 ����ȭ������ �ϼ���Ŵ.

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
	VideoCapture cap(1);	//0�� �⺻ī�޶�(��ķ) 1����������..
	if (!cap.isOpened()) {	//����ó���ڵ�
		cerr << "Camera open failed!" << endl;
		return;
	}

	Mat frame;
	while (true) {
		cap >> frame;	//@@@@@@@@@@@@@@@@
		if (frame.empty()) {	//����ó��
			cerr << "Image load failed!" << endl;
			break;
		}

//############################################################################################

		flip(frame,frame,1);	//�¿����(�ſ���)
		imshow("frame", frame);
//		imshow("frame_b", b_b(frame, 80, 170, 100, 100, 255, 205));
//		imshow("frame_b", b_b(frame, 80, 170, 100, 100, 255, 205));
		int kc = waitKey(5);
		if (kc == 27) { // ESC key 0.5�ʴ����� refresh
			cout << "esc ����" << endl;
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
//	erode(output, output, Mat(), Point(-1, -1), 2);		//2��ħ��
//	dilate(output, output, Mat(), Point(-1, -1), 2);	//2����â
	return output;
}