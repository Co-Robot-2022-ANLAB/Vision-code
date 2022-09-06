/*
220906 ROI 설정하는 (카메라 화면 가장자리 자르는) 프로그램.
함수 한가지(camera_edge_cut) 사용했고, 밑에 써있지만 입력 Mat에 frame 넣어서 실행하면 된다.
무한루프이기 때문에 esc 키를 눌러서 종료하고, "z x c v a s d f" 가지고 파라미터를 조정한다.
테스트할때 본 프로그램에 함수부분만 복사 붙여넣기 해서 바로 사용하면 될듯.
값 확정해서 적용할때는 밑에 68번 한줄만 쓰면 됨.
ROI가 원본 이미지를 벗어날때는 error 발생하니까 안 넘어가도록 해야함.(테스트용이니까)
*/

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

void camera_edge_cut(Mat i_input);

int main()
{
	cout << "Hello OpenCV " << CV_VERSION << endl;

	Mat frame = imread("c_test_frame.png");	//시험용 캡처화면

	camera_edge_cut(frame);	//edge_cut 함수 실행명령

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
			cout << "esc 감지" << endl;
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
		printf("%d ms 간격으로 실행중입니다.\n", wait_time);

		//실제 적용할때 이거 한줄만 쓰면 된다.
		Mat i_output = i_input(Rect(a, b, c, d));

		imshow("frame_edge_cut", i_output);
		printf("a : %d\nb : %d\nc : %d\nd : %d\n", a, b, c, d);
	}
	destroyAllWindows();
	return;
}