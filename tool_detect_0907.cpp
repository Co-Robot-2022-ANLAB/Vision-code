#include "opencv2/opencv.hpp"
#include <iostream>
#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float64.h>

#define CAM_NUM 0

using namespace cv;
using namespace std;

void camera_in();
Mat b_b(Mat input, int hh, int ss, int vv, int hhh, int sss, int vvv);
void mouse_callback_2(int event, int x, int y, int flags, void *param);
void on_mouse(int event, int x, int y, int flags, void *);

ros::Publisher tool_center_pub;
ros::Publisher tool_angle_pub;

int H, S, V;
Point ptOld1;
Mat img_color;

int main(int argc, char** argv)
{

    // ROS 설정 ###############################################
    ros::init(argc, argv, "tool_detect");

    ros::NodeHandle nh;
    tool_center_pub = nh.advertise<std_msgs::Float32MultiArray>("camera/tool_center", 100);
    tool_angle_pub = nh.advertise<std_msgs::Float64>("camera/tool_angle", 100);

    ros::Rate loop_rate(50);
    // //##########################################################


	cout << "Hello OpenCV " << CV_VERSION << endl;
	
    VideoCapture cap(CAM_NUM);	//0번 기본카메라(웹캠) 1번은다음거..	1iIlL

	cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

	if (!cap.isOpened()) {	//예외처리코드
		cerr << "Camera open failed!" << endl;
		return 0;
	}

	Mat frame;
	while (ros::ok()) 
    {
		cap >> frame;	//@@@@@@@@@@@@@@@@
		

//############################################################################################
//		frame = imread("c_test07.png");	//노트북 시험용. //220902
//		flip(frame,frame,1);	//좌우반전(거울모드)
		imshow("frame", frame);
//		imshow("frame_b", b_b(frame, 20, 130, 80, 110, 255, 255));
		int kc = waitKey(1);	// 1ms refresh-rate
		if (kc == 27) 
        { // ESC key
			cout << "esc 감지" << endl;
			break;
		}

		img_color = frame.clone();

		setMouseCallback("frame", mouse_callback_2);
		setMouseCallback("frame", on_mouse);


//############################################################################################
		// Mat oa = b_b(frame, 80, 170, 100, 100, 255, 205);	//	v1
		Mat oa = b_b(frame, 20, 130, 80, 110, 255, 255);	//	v2
//		imshow("oa", oa);
		dilate(oa, oa, Mat(), Point(-1, -1), 5);
//		imshow("oa_dilate", oa);
		vector<vector<Point>> contours;	//외곽선정보는 vector<vector<Point>> 타입임.
		findContours(oa, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);	//외곽선정보를 contours 에 저장
//		drawContours(oa, contours, -1, Scalar(100, 100, 100), 2);	//외곽선 그리기
		int c_max = 0;	//contours_max
		if (contours.size() > 1)
//		if (0)
		{
			for (int i = 0; i < contours.size(); i++)	// i번반복, i=contours.size 외곽선개수
			{
				cout << contours.size() << endl;	//contours.size = 2
				cout << "contour " << i << " 의 면적은 : " << contourArea(contours[i]) << endl;
				if (contourArea(contours[i]) > contourArea(contours[c_max])) c_max = i;
			}
			printf("\n제일 큰 contour는 %d 번 입니다.\n", c_max);
		
			RotatedRect a = minAreaRect(contours[c_max]);	//280페이지 참조해서 그림 @문제해결

			//################ROS Publish###################
			std_msgs::Float32MultiArray tool_center;
			std_msgs::Float64 tool_angle;

			float real_x = (640-a.center.x) * 1.25;
			float real_y = (480-a.center.y) * 1.25;

			tool_center.data.clear();
			tool_center.data.push_back(real_x);
			tool_center.data.push_back(real_y);

			tool_angle.data = a.angle;

			tool_center_pub.publish(tool_center);
			tool_angle_pub.publish(tool_angle);
			//##############################################

			cout << a.center << endl << a.size << endl << a.angle << endl;
	//		circle(oa, a.center, 10, Scalar(100, 100, 100), 2); //중앙에 원 표시$$$$$
			Point2f points[4];
			a.points(points);
			cvtColor(oa, oa, COLOR_GRAY2BGR, 0);
			for (int i = 0; i < 4; i++)
				line(oa, points[i], points[(i + 1) % 4], Scalar(0, 255, 0), 2);
			drawContours(oa, contours, -1, Scalar(0, 0, 255), 2);	//외곽선 그리기
			circle(oa, a.center, 10, Scalar(255, 0, 0), 2); //중앙에 원 표시$$$$$
			imshow("tool", oa);
		}
		else
		{
			imshow("tool", oa);
			
		}

		waitKey(1);

        ros::spinOnce();
        loop_rate.sleep();

	}

	waitKey(1);
	return 0;
}

Mat b_b(Mat input, int hh, int ss, int vv, int hhh, int sss, int vvv)
{
	Mat output, mid;
	cvtColor(input, mid, COLOR_BGR2HSV);
	inRange(mid, Scalar(hh, ss, vv), Scalar(hhh, sss, vvv), output);
//	erode(output, output, Mat(), Point(-1, -1), 2);		//2번침식
//	dilate(output, output, Mat(), Point(-1, -1), 2);	//2번팽창
	output = ~output;
	return output;
}

void mouse_callback_2(int event, int x, int y, int flags, void *param)
{
	if (event == EVENT_RBUTTONDOWN)
	{
		Vec3b color_pixel = img_color.at<Vec3b>(y, x);

		Mat hsv_color = Mat(1, 1, CV_8UC3, color_pixel);


		H = hsv_color.at<Vec3b>(0, 0)[0];
		S = hsv_color.at<Vec3b>(0, 0)[1];
		V = hsv_color.at<Vec3b>(0, 0)[2];

		cout << "right H= " << H << endl;
		cout << "right S= " << S << endl;
		cout << "right V = " << V << "\n"
			 << endl;

	}
}

void on_mouse(int event, int x, int y, int flags, void *)
{
	switch (event)
	{
	case EVENT_LBUTTONDOWN:
		ptOld1 = Point(x, y);
		cout << "EVENT_LBUTTONDOWN: " << x << ", " << y << endl;
		break;
	case EVENT_LBUTTONUP:
		cout << "EVENT_LBUTTONUP: " << x << ", " << y << endl;
		break;
	}
}
