#include <cmath>
#include "ros/ros.h"
#include "my_service/AddInts.h"
#include "std_msgs/Float32MultiArray.h"

ros::Publisher publisher;

bool solution(my_service::AddInts::Request &req,
              my_service::AddInts::Response &res)
{
    ROS_INFO("request: a = %f,b = %f, c = %f", req.coef_a, req.coef_b, req.coef_c);
    std_msgs::Float32MultiArray solution;

    /// If it linear (coefficient a near 0)
    /// It is necessary to check BEFORE division by coefficient a
    if (std::fabs(req.coef_a) < 0.000001)
    {
        /// If (a == 0) && (b == 0) - infinity of solutions
        if (std::fabs(req.coef_b - 0) > 0.000001)
        {
            res.roots.push_back(-req.coef_c / req.coef_b);
            solution.data.push_back(-req.coef_c / req.coef_b);
        }
        publisher.publish(solution);
        return true;
    }

    /// If it is quadratic equation
    float deskriminant = req.coef_b * req.coef_b - 4 * req.coef_a * req.coef_c;

    if (std::fabs(deskriminant) < 0.000001)
    {
        res.roots.push_back(-req.coef_b / (2 * req.coef_a));
        solution.data.push_back(-req.coef_b / (2 * req.coef_a));
    }
    else if (deskriminant > 0)
    {
        res.roots.push_back((-req.coef_b - std::sqrt(deskriminant)) / (2 * req.coef_a));
        res.roots.push_back((-req.coef_b + std::sqrt(deskriminant)) / (2 * req.coef_a));
        solution.data.push_back((-req.coef_b - std::sqrt(deskriminant)) / (2 * req.coef_a));
        solution.data.push_back((-req.coef_b + std::sqrt(deskriminant)) / (2 * req.coef_a));
    }
    return true;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "solution_server");
    ros::NodeHandle n;
    ros::ServiceServer service = n.advertiseService("Solution", solution);
    publisher = n.advertise<std_msgs::Float32MultiArray>("Solution", 1000);

    // std_msgs::Message1 message;
    // pub.publish(message);
    ros::spin();
    return 0;
}