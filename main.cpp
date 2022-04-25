#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;
//�������� ��������
int main()
{
	Mat img1 = imread("driver01.jpg");	//�����̹�������
	Mat img_re;							//��������̹���
	Mat img_gray;						//����̹���
	Mat img_bin;						//����ȭ�̹���

	Size img_size(500, 500);	//����� ���� ũ��

	resize(img1, img_re, img_size);		//����������
	cvtColor(img_re, img_gray, COLOR_BGR2GRAY);	//�������
	threshold(img_gray, img_bin, 220, 255, THRESH_BINARY_INV);	//����ȭ

	//********************************************** ��â
	Mat dst4;
	dilate(img_bin, dst4, Mat(), Point(-1, -1), 4);		//4�� ��â
	//**********************************************
	//Mat img_contour;		//�ܰ����̹���
	vector<vector<Point>> contours;	//�ܰ��������� vector<vector<Point>> Ÿ����.
	findContours(dst4, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);	//�ܰ��������� contours �� ����
//	findContours(img_bin, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);	//�ܰ��������� contours �� ���� @�׽�Ʈ
	drawContours(dst4, contours, -1, Scalar(100, 100, 100), 2);	//�ܰ��� �׸���
	RotatedRect a = minAreaRect(contours[0]);	//280������ �����ؼ� �׸� @�����ذ�
	cout << a.center << endl << a.size << endl << a.angle << endl;	
	//235.127 , 239.846 �߽���ǥ 526.452 * 67.0615 ũ�� -41.0091 ����
	//**********************************************

	circle(dst4, a.center, 10, Scalar(100, 100, 100), 2); //�߾ӿ� �� ǥ��
	Point2f points[4];
	a.points(points);
	for (int i = 0;i < 4;i++)
		line(dst4, points[i], points[(i + 1) % 4], Scalar(100, 100, 100), 2);
	//line(dst4, Point(235, 239), Point(235, 500), Scalar(50, 50, 50));	//�� �׸���
	imshow("bin", img_bin);		//���� �����ֱ�(����ȭ)
//	imshow("dilate4", img_re);	//4����â
	imshow("dilate4", dst4);	//4����â

	waitKey();	//Ű�Է¹ޱ�

	return 0;	//����
}

/*
220407
�ϴ� �簢�������� �ߴµ� �� ����̶� �װŸ� �ذ��ؾ��Ѵ�. ���� Matrix�� ���� grayscale���������� �����ϰ� ���� �߰��� �׸� �߾ӿ�
�ܰ��� �簢���� �÷��� �����ٰ� �׸� �� �ֵ��� �����ؾ���
*/