// Include Libraries.
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <ros/ros.h>
#include <stdio.h>
#include <sstream>
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float64.h>

using namespace cv;
using namespace std;

ros::Subscriber Human_sub;
ros::Subscriber Robot_sub;
ros::Subscriber Safe_sub;
ros::Subscriber Tool_center_sub;
ros::Subscriber Tool_angle_sub;
ros::Subscriber Hand_num_sub;
ros::Subscriber Plc_start_sub;
ros::Subscriber Plc_program_down;

bool human_flag = false;
bool robot_flag = false;
bool safe_flag = false;
float tool_angle = 0.0;
float tool_center[2] = {0.0, 0.0};
int hand_num = 0;
bool plc_start_flag = false;
bool plc_program_down_flag = false;

Point pt01d;
Mat img;
Mat img2;

//############# opencv 콜백함수 ############################
void on_mouse(int event, int x, int y, int flags, void*);
//#########################################################

//#########################ros 콜백함수##########################
void Human_sub_callback(const std_msgs::Bool::ConstPtr& msgs)
{
    human_flag = msgs -> data;
}

void Robot_sub_callback(const std_msgs::Bool::ConstPtr& msgs)
{
    robot_flag = msgs -> data;
}

void Safe_sub_callback(const std_msgs::Bool::ConstPtr& msgs)
{
    safe_flag = msgs -> data;
}

void Tool_angle_sub_callback(const std_msgs::Float64::ConstPtr& msgs)
{
    tool_angle = msgs -> data;
}

void Tool_center_sub_callback(const std_msgs::Float32MultiArray::ConstPtr& array)
{
    int i = 0;
	
	for(std::vector<float>::const_iterator it = array->data.begin(); it != array->data.end(); ++it)
	{
		tool_center[i] = *it;
		i++;
	}

	return;
}

void Hand_num_sub_callback(const std_msgs::Int32::ConstPtr& msgs)
{
    hand_num = msgs -> data;
}

void Plc_start_sub_callback(const std_msgs::Bool::ConstPtr& msgs)
{
    plc_start_flag = msgs -> data;
}

void Plc_program_callback(const std_msgs::Bool::ConstPtr& msgs)
{
    plc_program_down_flag = msgs -> data;
}
//################################################################################3

 int main(int argc, char **argv)
 {
    ros::init(argc, argv, "status_display");
	ros::NodeHandle nh;

    Human_sub = nh.subscribe("/human_zone", 100, Human_sub_callback);
    Robot_sub = nh.subscribe("/robot_zone", 100, Robot_sub_callback);
    Safe_sub = nh.subscribe("/safe_zone", 100, Safe_sub_callback);
    Tool_center_sub = nh.subscribe("/camera/tool_center", 100, Tool_center_sub_callback);
    Tool_angle_sub = nh.subscribe("/camera/tool_angle", 100, Tool_angle_sub_callback);
    Hand_num_sub = nh.subscribe("/status_hand", 100, Hand_num_sub_callback);
    Plc_start_sub = nh.subscribe("/plc_start", 100, Plc_start_sub_callback);
    Plc_program_down = nh.subscribe("/plc_down", 100, Plc_program_callback);

    ros::Rate loop_rate(50);

    while(ros::ok())
    {
        Mat display(960, 1280, CV_8UC3, Scalar(255, 255, 255));

        img = display.clone();
        Mat display2(960, 1000, CV_8UC3, Scalar(255, 255, 255));
        img2 = display2.clone();

        int fontFace = FONT_HERSHEY_DUPLEX;

        //######################### 글자 세기기 ####################################

        // 세이프티 존 센서 인식
        putText(img, "Human_Side", Point(100, 80), fontFace, 2, Scalar(255, 51, 51), 3);
        putText(img, "Robot_Side", Point(710, 80), fontFace, 2, Scalar(204, 255, 0), 3);

        Mat human_img = imread("/home/park-kyeong-geun/img/human.jpg");
        Mat Robot_img = imread("/home/park-kyeong-geun/img/robot_arm.jpg");

        resize(human_img, human_img, Size(283,283));
        resize(Robot_img, Robot_img, Size(283,283));

        Mat human_img_cropped = human_img(Rect(74, 44, 122, 198));
        Mat robot_img_cropped = Robot_img(Rect(30, 50, 206, 192));

        // imshow("human", human_img_cropped);
        // imshow("robot", robot_img_cropped);

        Mat human_roi = img(Rect(205,125,human_img_cropped.cols,human_img_cropped.rows));
        Mat robot_arm_roi = img(Rect(780, 120, robot_img_cropped.cols, robot_img_cropped.rows));


        if(human_flag == true)
        {
            addWeighted(human_roi, 0, human_img_cropped, 1, 0, human_roi);
            putText(img, "Human_In", Point(120, 403), fontFace, 2, Scalar(30, 30, 30), 2);
        }
        else if(human_flag == false)
        {
            putText(img, "Human_OUT", Point(100, 204), fontFace, 2, Scalar(30, 30, 30), 2);
        }
        
        if(robot_flag == true)
        {
            addWeighted(robot_arm_roi, 0, robot_img_cropped, 1, 0, robot_arm_roi);
            putText(img, "Robot_In", Point(730, 403), fontFace, 2, Scalar(30, 30, 30), 2);
        }
        else if (robot_flag == false)
        {
            putText(img, "Robot_OUT", Point(710, 204), fontFace, 2, Scalar(30, 30, 30), 2);
        }
        // bitwise_and(robot_arm_roi, Robot_img, robot_arm_roi);
        // bitwise_and(human_roi, human_img, human_roi);

        // 공구 좌표 및 각도 인식
        putText(img2, "Tool_Center", Point(370, 50), fontFace, 1.5, Scalar(0, 0, 0) , 2);
        putText(img2, "(relative distance from robot)", Point(270, 100), fontFace, 1, Scalar(0, 0, 0));

        rectangle(img2, Rect(40, 150, 360, 360), Scalar(0, 255, 0), 5);
        
        putText(img2, "robot-arm", Point(460, 335), fontFace, 1, Scalar(255, 0, 0), 1.5);

        float center_point[2];

        center_point[0] = 40 + cvRound(tool_center[0] * 0.72);

        if(center_point[0] < 40) center_point[0] = 40;
        else if(center_point[0] > 400) center_point[0] = 400;

        center_point[1] = 150 + cvRound(tool_center[1] * 0.72);

        if(center_point[1] < 150) center_point[1] = 150;
        else if(center_point[1] > 510) center_point[1] = 510;

        //x선
        line(img2, Point(440, 330), Point(center_point[0], 330), Scalar(0, 0, 255), 3);
        putText(img2, "X", Point(cvRound((440 - center_point[0])/2)+ center_point[0] - 10, 325), fontFace, 1, Scalar(0, 0, 255), 1);

        //y선
        line(img2, Point(center_point[0], center_point[1]), Point(center_point[0], 330), Scalar(0,0,255), 3);
        if(center_point[1] < 330)
        {
            putText(img2, "Y", Point(center_point[0] - 25, cvRound((330-center_point[1])/2) + center_point[1]), fontFace, 1, Scalar(0, 0, 255), 1);
        }
        else if(center_point[1] == 330)
        {
            putText(img2, "Y", Point(center_point[0] - 10, center_point[1]- 25), fontFace, 1, Scalar(0, 0, 255), 1);
        }
        else if(center_point[1] > 330)
        {
            putText(img2, "Y", Point(center_point[0] - 25, cvRound((center_point[1] - 330)/2) + 330), fontFace, 1, Scalar(0, 0, 255), 1);
        }

        circle(img2, Point(center_point[0], center_point[1]), 12, Scalar(0, 0, 0), -1); //공구 원
        circle(img2, Point(440, 330), 10, Scalar(255, 0, 0), -1); //로봇팔 원

        string tool_center_x = to_string(500 - tool_center[0] + 40) + " mm";
        string tool_center_y;
        if(tool_center[1] < 250) tool_center_y = to_string(250 + tool_center[1]) + " mm";
        else tool_center_y = to_string(tool_center[1]-250) + " mm";


        putText(img2, "tool_center_x", Point(440, 190), fontFace, 1.5, Scalar(30, 30, 30), 2);
        putText(img2, tool_center_x, Point(445, 273), fontFace, 1.5, Scalar(30, 30, 30), 2);

        putText(img2, "tool_center_y", Point(440, 425), fontFace, 1.5, Scalar(30, 30, 30), 2);
        putText(img2, tool_center_y, Point(445, 505), fontFace, 1.5, Scalar(30, 30, 30), 2);

        putText(img2, "Tool_Angle", Point(60, 620), fontFace, 1.5, Scalar(30, 30, 30), 2);

        Point2f center(170, 760), pts[4];
        Size2f size(50, 200);
        int deg = cvRound(tool_angle);
        RotatedRect rot_rect(center, size, deg);

        Rect bound_rect = rot_rect.boundingRect();
        rot_rect.points(pts);

        for(int i = 0; i < 4; i++)
        {
            line(img2, pts[i], pts[(i+1) % 4], Scalar(0, 0, 0), 2);
        }

        string tool_angl_st = to_string(tool_angle) + " degree";

        putText(img2, tool_angl_st, Point(15, 940), fontFace, 1.5, Scalar(30, 30, 30), 2);

        //손가락 갯수
        putText(img2, "Hand_Number", Point(505, 620), fontFace, 1.5, Scalar(30, 30, 30), 2);

        string hand_num_str = to_string(hand_num);

        putText(img2, hand_num_str, Point(505, 720), fontFace, 1.5, Scalar(30, 30, 30), 2);         

//######################################################################################################################

        //  가로줄
        line(img, Point(0, 450), Point(img.cols, 450), Scalar(0, 0, 0), 3);

//######################################################################################################################

        // plc 가동중 인식
        putText(img, "PLC_operation", Point(70, 540), fontFace, 2, Scalar(204, 255, 0), 2);
        putText(img, "Raspberry-Pi4", Point(705, 540), fontFace, 2, Scalar(204, 255, 0), 2);

        if(plc_start_flag == false)
        {
            circle(img, Point(285, 745), 142, Scalar(255, 0, 0), -1);
            circle(img, Point(285, 745), 142, Scalar(0, 0, 0), 2);
            putText(img, "STOP", Point(210, 765), fontFace, 2, Scalar(255, 255, 255), 2);
        }
        else if(plc_start_flag == true)
        {
            circle(img, Point(285, 745), 142, Scalar(0, 0, 255), -1);
            circle(img, Point(285, 745), 142, Scalar(0, 0, 0), 2);
            putText(img, "START", Point(195, 765), fontFace, 2, Scalar(255, 255, 255), 2);
        }

        if(plc_program_down_flag == false)
        {
            circle(img, Point(935, 745), 142, Scalar(255, 0, 0), -1);
            circle(img, Point(935, 745), 142, Scalar(0, 0, 0), 2);
            putText(img, "STOP", Point(860, 765), fontFace, 2, Scalar(255, 255, 255), 2);
        }
        else if(plc_program_down_flag == true)
        {
            circle(img, Point(935, 745), 142, Scalar(0, 0, 255), -1);
            circle(img, Point(935, 745), 142, Scalar(0, 0, 0), 2);
            putText(img, "START", Point(845, 765), fontFace, 2, Scalar(255, 255, 255), 2);
        }

        // 각 문자열 사이즈 정보 입력
        Size Human_text_size = getTextSize("Human_Side", fontFace, 1,1, 0);
        Size Robot_text_size = getTextSize("Robot_Side", fontFace, 1,1, 0);
        Size Tool_center_text_size = getTextSize("Now_Tool_center(relative distance from robot", fontFace, 1,1, 0);
        Size Tool_angle_text_size = getTextSize("Now_Tool_angle", fontFace, 1,1, 0);
        Size PLC_operation_text_size = getTextSize("PLC_operation", fontFace, 1,1, 0);
        Size raspberry_text_size = getTextSize("raspberry_operation", fontFace, 1,1, 0);
        
        // 비상신호
        if(safe_flag == true)
        {
            // Point warning_point1(50, 130);
            
            // // putText(img, "WARNING", warning_point1, fontFace, 3, Scalar(255, 255, 255), 3);
            // Size warning_text_size = getTextSize("WARNING", fontFace, 5, 5, 0);

            // Point warning_point2(50, img.rows - 130);
            // Point warning_point3(img.cols - warning_text_size.width - 50, img.rows - 130 - warning_text_size.height);
            // Point warning_point4(img.cols - warning_text_size.width - 50, 130 + warning_text_size.height);
            
            // rectangle(img, Point(50, 110 - warning_text_size.height), Point(warning_text_size.width+50, warning_text_size.height+ 30), Scalar(0, 0, 255), -1);
            // putText(img, "WARNING", warning_point1, fontFace, 5, Scalar(255, 255, 255), 5);
            
            // rectangle(img, Point(50, img.rows - 150 - warning_text_size.height), Point(warning_text_size.width+50, img.rows - 110), Scalar(0, 0, 255), -1);
            // putText(img, "WARNING", warning_point2, fontFace, 5, Scalar(255, 255, 255), 5);

            // rectangle(img, Point(img.cols - warning_text_size.width - 50, img.rows - 150 - warning_text_size.height - warning_text_size.height), Point(img.cols - 50, img.rows - 110 - warning_text_size.height), Scalar(0, 0, 255), -1);
            // putText(img, "WARNING", warning_point3, fontFace, 5, Scalar(255, 255, 255), 5);

            // rectangle(img, Point(img.cols - warning_text_size.width - 50, 110), Point(img.cols - 50, 160 + warning_text_size.height), Scalar(0, 0, 255), -1);
            // putText(img, "WARNING", warning_point4, fontFace, 5, Scalar(255, 255, 255), 5);

            // Point org((img.cols - warning_text_size.width) / 2, (img.rows + warning_text_size.height) / 2);

            // rectangle(img, org + Point(-10, +20), org + Point(warning_text_size.width+10, -warning_text_size.height-20), Scalar(0, 0, 255), -1);
            // putText(img, "WARNING", org, fontFace, 5, Scalar(255, 255, 255), 5);

            const String warn_txt = "WARNING";
            double fontScale = 6;
            int thickness = 7;

            Size size_warn_txt = getTextSize(warn_txt, fontFace, fontScale, thickness, 0);
            Size sizeimg1 = img.size();
            Size sizeimg2 = img2.size();

            Point org1((sizeimg1.width - size_warn_txt.width) / 2, (sizeimg1.height + size_warn_txt.height) / 2);
            Point org2((sizeimg2.width - size_warn_txt.width) / 2, (sizeimg2.height + size_warn_txt.height) / 2);

            
            rectangle(img, org1 + Point(-10,10), org1 + Point(size_warn_txt.width + 10, -size_warn_txt.height - 10), Scalar(0, 0, 255), -1);
            putText(img, warn_txt, org1, fontFace, fontScale, Scalar(255,255,255), thickness);

            rectangle(img2, org2 + Point(-10,10), org2 + Point(size_warn_txt.width + 10, -size_warn_txt.height - 10), Scalar(0, 0, 255), -1);
            putText(img2, warn_txt, org2, fontFace, fontScale, Scalar(255,255,255), thickness);
        }
       

        imshow("status_screen", img);
        imshow("tool_screen", img2);

        setMouseCallback("status_screen", on_mouse);
        setMouseCallback("tool_screen", on_mouse);

        ros::spinOnce();
        loop_rate.sleep();
        
        waitKey(1);

    }
    
 }

 void on_mouse(int event, int x, int y, int flags, void*)
 {
    switch(event)
    {
        case EVENT_LBUTTONDOWN:
            pt01d = Point(x,y);
            cout << "x : " << x << " , " << "y : " << y << endl; 
        break;
        case EVENT_MOUSEMOVE:
            if (flags & EVENT_FLAG_LBUTTON)
            {
                line(img, pt01d, Point(x, y), Scalar(0, 0, 255), 10);
                imshow("status_screen", img);
                pt01d = Point(x, y);

                waitKey();
            }
            break;
        
        default:
            break;
    }
 }
