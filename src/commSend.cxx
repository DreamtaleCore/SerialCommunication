/** @file UAV write commands to serial to control servos
  * @author DreamTale
  * @date Jul 30, 2016
  */
#include "commCtrl.h"
#include "../include/stdHeaders.h"
#include "ros/ros.h"

#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;
//==============================================
// the content of the data packet

// +------------+-----------+------------+
// | packetHead | main data | packetTail |
// +------------+-----------+------------+
//
// Now, the header is '!', and the tail is '#'
// FORCE
//==============================================

string dev = "/dev/ttyUSB1"; // set as default
ros::Publisher dataPub;
std_msgs::String pubData;

void chatterCallback(const std_msgs::String::ConstPtr& msgs)
{
    string cmd_data = (char*)msgs->data.c_str();

    ROS_INFO("I'll write [%s]\n", cmd_data.c_str());

    cmd_data = "!" + cmd_data + "#";
    commWrite(cmd_data, dev);

    cmd_data = "";
}

void setup()
{
  // Add the right path of serial_port_conf.yml in your device
  FileStorage fs("/home/yf/ws/src/drone/uav_ctrl_serial_comm/serial_port_conf.yml",
                 FileStorage::READ);
  fs["serialPortWrite"] >> dev;

  cout << endl << dev << " has been set as port to write." << endl;
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "serialWrite");
    ros::NodeHandle nh;
    ros::Rate loopRate(100);

    dataPub = nh.advertise<std_msgs::String>("uavWriteServo", 1000);


    setup();
    cout << "*Serial write node is setup successfully!!" << endl;
    cout << "_____________________________" << endl << endl;

    ros::Subscriber sub = nh.subscribe("uavCoreLogicServo", 1000, chatterCallback);

    while(nh.ok())
    {
      ros::spinOnce();
      loopRate.sleep();

      commWrite("!cmd_data#", dev.c_str());
      cout << "I have send data: " << "[cmd_data]" << endl << endl;
    }

    return 0;
}
