/*
220906 ROI �����ϴ� (ī�޶� ȭ�� �����ڸ� �ڸ���) ���α׷�.
�Լ� �Ѱ���(camera_edge_cut) ����߰�, �ؿ� �������� �Է� Mat�� frame �־ �����ϸ� �ȴ�.
���ѷ����̱� ������ esc Ű�� ������ �����ϰ�, "z x c v a s d f" ������ �Ķ���͸� �����Ѵ�.
�׽�Ʈ�Ҷ� �� ���α׷��� �Լ��κи� ���� �ٿ��ֱ� �ؼ� �ٷ� ����ϸ� �ɵ�.
�� Ȯ���ؼ� �����Ҷ��� �ؿ� 68�� ���ٸ� ���� ��.
ROI�� ���� �̹����� ������� error �߻��ϴϱ� �� �Ѿ���� �ؾ���.(�׽�Ʈ���̴ϱ�)
*/

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void camera_edge_cut(Mat i_input);

int main()
{
	cout << "Hello OpenCV " << CV_VERSION << endl;

	Mat frame = imread("c_test_frame.png");	//����� ĸóȭ��

	camera_edge_cut(frame);	//edge_cut �Լ� ������

	waitKey();
	return 0;
}

void camera_edge_cut(Mat i_input)
{
	int a = 115, b = 45, c = 390, d = 390;
	while (true) {
		imshow("i_input", i_input);
		int wait_time = 1000;
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