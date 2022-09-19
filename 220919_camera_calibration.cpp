/*
@@@@@	���⼭ �����ؾ��Ұ�
1. �������� ����
2. �Լ� calibration
@@@@@	������
CAM_NUM , A B C D ���� �������ְ� �����ϸ��
escŰ�� �����ϸ� A B C D �� calibration �� �� �����.
*/

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

//�������� ���� , ABCD�� �ڿ��������μ���
int CAM_NUM = 0;
int A = 130, B = 40, C = 384, D = 380;

//�Լ� ����
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
	if (!cap.isOpened()) {	//����ó��
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
//		c_frame = imread("c_test_frame_3.png");	//�׽�Ʈ��!!!
		imshow("c_frame", c_frame);
		int wait_time = 3000;
		int kc = waitKey(wait_time);	//refresh-rate
		if (kc == 27) {					//ESC key
			cout << "esc ����" << endl;
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
		//����ó��
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