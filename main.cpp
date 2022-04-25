/* //**********************************************
220420_4 ������ ����
cutter handcream mandarin phonestand toothbrush(_d,b) .jpg
���Ϸ� ���� ����
toothbrush cutter phonestand  -45 �ߵ�.
mandarin �� �ȵ�. -20������ �ּ�.
handcream �Ͼ�κж����� ����
*/ //**********************************************

#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

Mat paint_b_2(Mat paint_input);
Mat brighter(Mat a, int b);
int gray_mean_2(Mat a);
Mat ad_bin(Mat input, int hm);

int main()
{
	Mat img_01 = imread("mandarin.jpg");	//�����̹���
	Mat img_01_re;							//�������
	Size size(400, 400);					//���� ũ��
	Mat img_01_g;							//���,(��� 245_?)
	Mat img_01_b;							//����
	resize(img_01, img_01_re, size);		//����������
	cvtColor(img_01_re, img_01_g, COLOR_BGR2GRAY);	//g �����
	//�Ͼ�κ��� ������ - �� ������ϰ� ������ ������ + �� ���ش�
	img_01_b = ad_bin(img_01_g, -20);		//g ����ȭ

	imshow("img_01", img_01);
	imshow("img_01_g", img_01_g);
	imshow("img_01_b", img_01_b);

	waitKey();
	return 0;	//����
}


Mat paint_b_2(Mat paint_input)
//���� ���̿� �°� ����ؼ� -1(0�Ǵ� �� ū����) �ϴ��Լ�. cols ����
{
	float cc = (float)255 / (float)paint_input.cols;	//���� ��ĭ�� scale ������
	cc = round(cc * 1000) / 1000;
	int count = 0;	// ����(cols)��ǥ ����
	// ���α��̸�ŭ �ݺ� 260������ ������ �ݿø�����..
	for (long double i = 0;i <= 260;i += cc) {
		i = round(i * 1000000) / 1000000;
		//		printf("%.9f ", i);
		for (int j = 0;j < paint_input.rows;j++) {	// ���α��̸�ŭ �ݺ�
			paint_input.at<uchar>(j, count) -= (int)round(i);
			//������ �Ʒ��� ���� �������鼭 ����(�Ͼ������ ���������ϱ�)
		}
		//		printf("%.0f ", (round(i)));
		count++;
		if (count == paint_input.cols) break;
	}
	//	printf("\n\n%d ", count);
	imshow("paint_b_2 output", paint_input);
	return paint_input;
}

Mat brighter(Mat a, int b)
{
	Mat aa = a.clone();
//	imshow("brighter_input", a);			//�����������������ֱ�
	for (int i = 0;i < aa.cols;i++) {		// ���α��̸�ŭ �ݺ�
		for (int j = 0;j < aa.rows;j++) {	// ���α��̸�ŭ �ݺ�
			if (aa.at<uchar>(j, i) > (255 - b))	//255���� ū���ڰ� �����ʰ�
				aa.at<uchar>(j, i) = 255;		//�ִ븦 255�� capping
			else if (aa.at<uchar>(j, i) < (0 - b)) //�ݴ�� b�� �����϶� 0���� �۾������ʰ�
				aa.at<uchar>(j, i) = 0;			//�ּҸ� 0�� capping
			else aa.at<uchar>(j, i) += b;
		}
	}
//	imshow("brighter_output", aa);		// ����Ѱ� �����ֱ�
	return aa;
}

int gray_mean_2(Mat a)
{
	//	imshow("gray_mean_input", a);
	int aa = 0;
	for (int i = 0;i < a.cols;i++) {		// ���α��̸�ŭ �ݺ�
		for (int j = 0;j < a.rows;j++) {	// ���α��̸�ŭ �ݺ�
			aa += a.at<uchar>(j, i);	//������ �Ʒ��� ���� �������鼭 ���Ѵ�.t
		}
	}
	aa /= (a.cols*a.rows);
//	printf("grayscale ����� %d\n", aa);
	return aa;
}

Mat ad_bin(Mat input, int hm)
//��ü ������ ����ȭ �ϱ�. how much
{
	Mat ad_bin_output;
	threshold(input, ad_bin_output, gray_mean_2(input) + hm, 255, THRESH_BINARY_INV);
	printf("%d+ (%d) �� ����ȭ", gray_mean_2(input), hm);
	return ad_bin_output;
}