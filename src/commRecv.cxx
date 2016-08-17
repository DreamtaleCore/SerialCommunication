/**
  * Read data from the point serial commmunicate port
  * @file UAV core logic node
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
string dev = "/dev/ttyUSB0";
std_msgs::String pubData;

void setup()
{
  // Add the right path of serial_port_conf.yml in your device
  FileStorage fs("/home/yf/ws/src/drone/uav_ctrl_serial_comm/serial_port_conf.yml",
                 FileStorage::READ);
  fs["serialPortRead"] >> dev;

  cout << endl << dev << " has been set as port to read." << endl;
}

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "serialRead");
    ros::NodeHandle nh;

    ros::Publisher publisher;
    publisher = nh.advertise<std_msgs::String>("uavGroundCar", 10);

    setup();
    cout << "*Serial read node is setup!!" << endl;
    cout << "_____________________________" << endl << endl;
    sleep(1);

    string data;
    ros::Rate loopRate(100);

    string logicLine;
    bool recordFlag = false;
    while(nh.ok())
    {
        ros::spinOnce();
        loopRate.sleep();

        int ret = commRead(data, dev.c_str());
        if(ret == -1)
        {
            cout << "Assert the serial port failed!" << endl;
            break;
        }
        for(int i = 0; i < data.length(); i++)
        {
          if(data[i] == '!')
            recordFlag = true;
          else if(data[i] == '#')
          {
            recordFlag = false;
            if(logicLine.length() > 0)
            {
              pubData.data = logicLine;
              ROS_INFO("I've read the data: [%s]\n", logicLine.c_str());
              publisher.publish(pubData);
              logicLine = "";
            }
          }
          else if(recordFlag == true)
            logicLine += data[i];
        }
    }

    return 0;
}
