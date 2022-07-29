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
	VideoCapture cap(1);	//0�� �⺻ī�޶�(��ķ) 1����������..	1iIlL
//	VideoCapture cap(0);	//��Ʈ�� �����.
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
//		frame = imread("c_test05.png");	//��Ʈ�� �����.
//		flip(frame,frame,1);	//�¿����(�ſ���)
		imshow("frame", frame);
//		imshow("frame_b", b_b(frame, 20, 130, 80, 110, 255, 255));
		int kc = waitKey(50);	// 5ms refresh-rate
		if (kc == 27) { // ESC key
			cout << "esc ����" << endl;
			break;
		}
//############################################################################################
//		Mat oa = b_b(frame, 80, 170, 100, 100, 255, 205);	//	v1
		Mat oa = b_b(frame, 20, 130, 80, 110, 255, 255);	//	v2
		dilate(oa, oa, Mat(), Point(-1, -1), 5);
		vector<vector<Point>> contours;	//�ܰ��������� vector<vector<Point>> Ÿ����.
		findContours(oa, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);	//�ܰ��������� contours �� ����
		drawContours(oa, contours, -1, Scalar(100, 100, 100), 2);	//�ܰ��� �׸���
		int c_max = 0;	//contours_max
		if (contours.size() > 1)
//		if (0)
		{
			for (int i = 0; i < contours.size(); i++)	// i���ݺ�, i=contours.size �ܰ�������
			{
				cout << contours.size() << endl;	//contours.size = 2
				cout << "contour " << i << " �� ������ : " << contourArea(contours[i]) << endl;
				if (contourArea(contours[i]) > contourArea(contours[c_max])) c_max = i;
			}
			printf("\n���� ū contour�� %d �� �Դϴ�.\n", c_max);
		}
		RotatedRect a = minAreaRect(contours[c_max]);	//280������ �����ؼ� �׸� @�����ذ�
		cout << a.center << endl << a.size << endl << a.angle << endl;
		circle(oa, a.center, 10, Scalar(100, 100, 100), 2); //�߾ӿ� �� ǥ��$$$$$
		Point2f points[4];
		a.points(points);
		for (int i = 0; i < 4; i++)
			line(oa, points[i], points[(i + 1) % 4], Scalar(100, 100, 100), 2);
		
		imshow("oa", oa);
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
	output = ~output;
	return output;
}