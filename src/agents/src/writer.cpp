#include <ros/ros.h>
#include <my_message/Message1.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>

typedef std::pair< std::string, int > TextNum;

int main(int argc, char**argv)
{
    ros::init(argc, argv, "writer");
    
    ROS_INFO_STREAM("Writer is ready.\n");
    
    ros::NodeHandle n;
    ros::Publisher pub = n.advertise<my_message::Message1>("Name", 10);
    
    std::vector< TextNum > messageList;
    messageList.push_back(TextNum("Hello", 11));
    messageList.push_back(TextNum("World", 12));
    messageList.push_back(TextNum("This", 17));
    messageList.push_back(TextNum("Is", 14));
    messageList.push_back(TextNum("Test.", 15));
    messageList.push_back(TextNum("Stop", -1));
    sleep(1);
    
    ros::Rate loop_rate(1);
    for (int i = 0; i < messageList.size(); i++)
    {
        my_message::Message1 message;
        message.text = messageList[i].first;
        message.number = messageList[i].second;
        pub.publish(message);
        
        ROS_INFO("%s, %d", messageList[i].first.c_str(), messageList[i].second);
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    ROS_INFO_STREAM("Publishing is finished!\n");
    return 0;
}