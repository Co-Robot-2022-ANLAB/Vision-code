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

        int fontFace = FONT_HERSHEY_DUPLEX;

        //######################### 글자 세기기 ####################################

        // 세이프티 존 센서 인식
        putText(img, "Human_Side", Point(20, 50), fontFace, 1, Scalar(255, 51, 51));
        putText(img, "Robot_Side", Point(20, 100), fontFace, 1, Scalar(204, 255, 0));

        // 공구 좌표 및 각도 인식
        putText(img, "Now_Tool_center(relative distance from robot)", Point(20, 150), fontFace, 1, Scalar(204, 255, 0));
        putText(img, "Now_Tool_angle", Point(20, 300), fontFace, 1, Scalar(204, 255, 0));

        // plc 가동중 인식
        putText(img, "PLC_operation", Point(20, 350), fontFace, 1, Scalar(204, 255, 0));
        putText(img, "raspberry_operation", Point(20, 400), fontFace, 1, Scalar(204, 255, 0));

        // 각 문자열 사이즈 정보 입력
        Size Human_text_size = getTextSize("Human_Side", fontFace, 1,1, 0);
        Size Robot_text_size = getTextSize("Robot_Side", fontFace, 1,1, 0);
        Size Tool_center_text_size = getTextSize("Now_Tool_center(relative distance from robot", fontFace, 1,1, 0);
        Size Tool_angle_text_size = getTextSize("Now_Tool_angle", fontFace, 1,1, 0);
        Size PLC_operation_text_size = getTextSize("PLC_operation", fontFace, 1,1, 0);
        Size raspberry_text_size = getTextSize("raspberry_operation", fontFace, 1,1, 0);

        if(human_flag == true)
        {
            putText(img, "Human_In", Point(Human_text_size.width + 40, 50), fontFace, 1, Scalar(30, 30, 30));
        }
        else if(human_flag == false)
        {
            putText(img, "Human_OUT", Point(Human_text_size.width + 40, 50), fontFace, 1, Scalar(30, 30, 30));
        }

        if(robot_flag == true)
        {
            putText(img, "Robot_In", Point(Robot_text_size.width + 40, 100), fontFace, 1, Scalar(30, 30, 30));
        }
        else if(robot_flag == false)
        {
            putText(img, "Robot_OUT", Point(Robot_text_size.width + 40, 100), fontFace, 1, Scalar(30, 30, 30));
        }


        string tool_center_x = "tool center : " + to_string(tool_center[0]) + " mm";
        string tool_center_y = "tool center : " + to_string(tool_center[1]) + " mm";
        string tool_angl_st = "tool angle :  " + to_string(tool_angle) + " degree";
        putText(img, tool_center_x, Point(20, 200), fontFace, 1, Scalar(30, 30, 30));
        putText(img, tool_center_y, Point(20, 250), fontFace, 1, Scalar(30, 30, 30));
        putText(img, tool_angl_st, Point(Tool_angle_text_size.width + 40, 300), fontFace, 1, Scalar(30, 30, 30));

        if(plc_start_flag == true)
        {
            putText(img, "PLC START", Point(PLC_operation_text_size.width + 40, 350), fontFace, 1, Scalar(30, 30, 30));
            cout << "4444" << endl;
        }
        else if(plc_start_flag == false)
        {
            putText(img, "PLC STOP", Point(PLC_operation_text_size.width + 40, 350), fontFace, 1, Scalar(30, 30, 30));
            
        }
        
        if(plc_program_down_flag == true)
        {
            putText(img, "raspberry4 is down", Point(raspberry_text_size.width + 40, 400), fontFace, 1, Scalar(30, 30, 30));
        }
        else if(plc_program_down_flag == false)
        {
            putText(img, "raspberry4 is operating", Point(raspberry_text_size.width + 40, 400), fontFace, 1, Scalar(30, 30, 30));
        }
        
        // 비상신호
        if(safe_flag == true)
        {
            Point warning_point1(50, 130);
            
            // putText(img, "WARNING", warning_point1, fontFace, 3, Scalar(255, 255, 255), 3);
            Size warning_text_size = getTextSize("WARNING", fontFace, 5, 5, 0);

            Point warning_point2(50, img.rows - 130);
            Point warning_point3(img.cols - warning_text_size.width - 50, img.rows - 130 - warning_text_size.height);
            Point warning_point4(img.cols - warning_text_size.width - 50, 130 + warning_text_size.height);
            
            rectangle(img, Point(50, 110 - warning_text_size.height), Point(warning_text_size.width+50, warning_text_size.height+ 30), Scalar(0, 0, 255), -1);
            putText(img, "WARNING", warning_point1, fontFace, 5, Scalar(255, 255, 255), 5);
            
            rectangle(img, Point(50, img.rows - 150 - warning_text_size.height), Point(warning_text_size.width+50, img.rows - 110), Scalar(0, 0, 255), -1);
            putText(img, "WARNING", warning_point2, fontFace, 5, Scalar(255, 255, 255), 5);

            rectangle(img, Point(img.cols - warning_text_size.width - 50, img.rows - 150 - warning_text_size.height - warning_text_size.height), Point(img.cols - 50, img.rows - 110 - warning_text_size.height), Scalar(0, 0, 255), -1);
            putText(img, "WARNING", warning_point3, fontFace, 5, Scalar(255, 255, 255), 5);

            rectangle(img, Point(img.cols - warning_text_size.width - 50, 110), Point(img.cols - 50, 160 + warning_text_size.height), Scalar(0, 0, 255), -1);
            putText(img, "WARNING", warning_point4, fontFace, 5, Scalar(255, 255, 255), 5);

            Point org((img.cols - warning_text_size.width) / 2, (img.rows + warning_text_size.height) / 2);

            rectangle(img, org + Point(-10, +20), org + Point(warning_text_size.width+10, -warning_text_size.height-20), Scalar(0, 0, 255), -1);
            putText(img, "WARNING", org, fontFace, 5, Scalar(255, 255, 255), 5);
        }
       

        imshow("status_screen", img);

        // setMouseCallback("status_screen", on_mouse);

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