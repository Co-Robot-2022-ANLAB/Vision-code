//

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void camera_edge_cut(void);
void camera_in();
Mat b_b(Mat input, int hh, int ss, int vv, int hhh, int sss, int vvv);

int main()
{
	cout << "Hello OpenCV " << CV_VERSION << endl;
	
	camera_edge_cut();	//edge_cut �Լ� ������

//	camera_in();

	waitKey();
	return 0;
}

void camera_in()
{
	VideoCapture cap(0);	//0�� �⺻ī�޶�(��ķ) 1����������..	1iIlL
//	VideoCapture cap(0);	//��Ʈ�� �����.	//220902
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
//		frame = imread("c_test_frame.png");	//��Ʈ�� �����. //220902
//		flip(frame,frame,1);	//�¿����(�ſ���)
		imshow("frame", frame);
//		imshow("frame_b", b_b(frame, 20, 130, 80, 110, 255, 255));
		int kc = waitKey(100);	// 1ms refresh-rate
		if (kc == 27) { // ESC key
			cout << "esc ����" << endl;
			break;
		}
//############################################################################################
//		Mat oa = b_b(frame, 80, 170, 100, 100, 255, 205);	//	v1
		Mat oa = b_b(frame, 20, 130, 80, 110, 255, 255);	//	v2
//		imshow("oa", oa);
		dilate(oa, oa, Mat(), Point(-1, -1), 5);
//		imshow("oa_dilate", oa);
		vector<vector<Point>> contours;	//�ܰ��������� vector<vector<Point>> Ÿ����.
		findContours(oa, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);	//�ܰ��������� contours �� ����
//		drawContours(oa, contours, -1, Scalar(100, 100, 100), 2);	//�ܰ��� �׸���
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
//		circle(oa, a.center, 10, Scalar(100, 100, 100), 2); //�߾ӿ� �� ǥ��$$$$$
		Point2f points[4];
		a.points(points);
		cvtColor(oa, oa, COLOR_GRAY2BGR, 0);
		for (int i = 0; i < 4; i++)
			line(oa, points[i], points[(i + 1) % 4], Scalar(0, 255, 0), 2);
		drawContours(oa, contours, -1, Scalar(0, 0, 255), 2);	//�ܰ��� �׸���
		circle(oa, a.center, 10, Scalar(255, 0, 0), 2); //�߾ӿ� �� ǥ��$$$$$
		imshow("oa_f", oa);
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


void camera_edge_cut(void)
{
	//
	VideoCapture cap(0);	//0�� �⺻ī�޶�(��ķ) 1����������..	1iIlL
//	VideoCapture cap(0);	//��Ʈ�� �����.	//220902
	if (!cap.isOpened()) {	//����ó���ڵ�
		cerr << "Camera open failed!" << endl;
		return;
	}
	//
	Mat i_input;
	int a = 115, b = 45, c = 390, d = 390;
	while (true) {
		
		cap >> i_input;

		imshow("i_input", i_input);
		int wait_time = 1;
		int kc = waitKey(wait_time);	//refresh-rate
		if (kc == 27) {					//ESC key
			cout << "esc ����" << endl;
			break;
		}
		else if (kc == 'z') {
			a += 1;
		}
		else if (kc == 'x') {
			a -= 1;
		}
		else if (kc == 'c') {
			b += 1;
		}
		else if (kc == 'v') {
			b -= 1;
		}
		else if (kc == 'a') {
			c += 1;
		}
		else if (kc == 's') {
			c -= 1;
		}
		else if (kc == 'd') {
			d += 1;
		}
		else if (kc == 'f') {
			d -= 1;
		}
		printf("%d ms �������� �������Դϴ�.\n", wait_time);

		//���� �����Ҷ� �̰� ���ٸ� ���� �ȴ�.
		Mat i_output = i_input(Rect(a, b, c, d));

		imshow("frame_edge_cut", i_output);
		printf("a : %d\nb : %d\nc : %d\nd : %d\n", a, b, c, d);
	}
	destroyAllWindows();
	return;
}
