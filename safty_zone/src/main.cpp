#include <iostream>
#include <ros/ros.h>
#include <std_msgs/Bool.h>

#include "/home/ubuntu/catkin_ws/src/safty_zone/include/rpi.h"

using namespace std;
using namespace rpi;

ros::Publisher Human_pub;
ros::Publisher Robot_pub;
ros::Publisher Safe_pub;

int main(int argc, char **argv)
{

    ros::init(argc, argv, "safty_zone_result");
    ros::NodeHandle nh;

    Human_pub = nh.advertise<std_msgs::Bool>("human_zone", 100);
    Robot_pub = nh.advertise<std_msgs::Bool>("robot_zone", 100);
    Safe_pub = nh.advertise<std_msgs::Bool>("safe_zone", 100);

    ros::Rate loop_rate(10);

    rpi::init();

    // 1초에 한 번씩 값 입력.
    while (ros::ok())
    {
        std_msgs::Bool Human;
        std_msgs::Bool Robot;
        std_msgs::Bool Safe;

        // cout << "444444" << endl;
        printf("Human Side: %d\n", rpi::get_from_human());
        printf("Robot Side: %d\n", rpi::get_from_robot());
        printf("Not Safe: %d\n\n", rpi::is_not_safe());

        if(get_from_human() == true)
        {
            Human.data = true;
        }
        else
        {
            Human.data = false;
        }
        
        if(get_from_robot() == true)
        {
            Robot.data = true;
        }
        else
        {
            Robot.data = false;
        }

        if(is_not_safe() == true)
        {
            Safe.data = true;
        }
        else
        {
            Safe.data = false;
        }

        Human_pub.publish(Human);
        Robot_pub.publish(Robot);
        Safe_pub.publish(Safe);
        

        ros::spinOnce();

        loop_rate.sleep();
    }

    return 0;
}