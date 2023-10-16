#include "ros/ros.h"
#include "my_service/AddInts.h"
#include <iostream>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "add_ints_publisher");

    ros::NodeHandle n;
    ros::ServiceClient client = n.serviceClient<my_service::AddInts>("add_two_ints");
    my_service::AddInts srv;

    while (ros::ok())
    {
        int a, b, c;
        ROS_INFO_STREAM("input the first coeficient a:");
        std::cin >> a;
        ROS_INFO_STREAM("input the first coeficient b:");
        std::cin >> b;
        ROS_INFO_STREAM("input the first coeficient c:");
        std::cin >> c;

        srv.request.coef_a = a;
        srv.request.coef_b = b;
        srv.request.coef_c = c;

        if (client.call(srv))
        {
            ROS_INFO("Root(s) are: ");
            for (int32_t n : srv.response.roots)
            {
                ROS_INFO("%d", n);
            }
        }
        else
        {
            std::cout << "Failed to call service add_two_ints" << std::endl;
            return 1;
        }
    }

    return 0;
}