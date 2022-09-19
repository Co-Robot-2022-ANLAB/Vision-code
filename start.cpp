#include <iostream>
#include <ros/ros.h>
#include <wiringPi.h>
#include <std_msgs/Bool.h>


using namespace std;

int start_pin = 15;

bool plc_mode = false;

bool plc_program = true;

int main(int argc, char **argv)
{

    ros::init(argc, argv, "PLC_signal");
    ros::NodeHandle nh;

    ros::Publisher start_pub = nh.advertise<std_msgs::Bool>("plc_start", 100);
    ros::Publisher program_pub = nh.advertise<std_msgs::Bool>("program_down", 100);

    ros::Rate loop_rate(10);

    if(wiringPiSetup() == -1) exit(1);

    pinMode(start_pin, OUTPUT);
    digitalWrite(start_pin, 0);

    string comand;

    int start_level = 0;



    // rpi::init();

   
    while (ros::ok())
    {

        std_msgs::Bool plc;
        std_msgs::Bool program;

        

        plc.data = plc_mode;
        start_pub.publish(plc);

        program.data = plc_program;
        program_pub.publish(program);

        loop_rate.sleep();

        if(start_level == 0)
        {
            cout << "시작하시겠습니까? YES(Y) / NO(N)" << endl;
            cin >> comand;

            cout << comand << "를 고르셨습니다." << endl;

            if(comand == "Y")
            {
                cout << "PLC를 시작하겠습니다." << endl;

                digitalWrite(start_pin, 1);

                plc_mode = true;

                start_level++;
            }
            else if(comand == "N")
            {
                // cout << "하지마 인마" << endl;
                cout << "프로그램을 끄도록 하겠습니다." << endl;

                digitalWrite(start_pin, 0);

                plc_program = false;

                program.data = plc_program;
                program_pub.publish(program);

                ros::spinOnce();

                loop_rate.sleep();

                break;
            }
            else
            {
                cout << "다시 고르시오" << endl;
            }
        }
        else
        {
            cout << "PLC 작동중" << endl;

            plc_mode = true;
        }

        plc.data = plc_mode;
        start_pub.publish(plc);

        program.data = plc_program;
        program_pub.publish(program);

        ros::spinOnce();

        loop_rate.sleep();
    }


    return 0;
}