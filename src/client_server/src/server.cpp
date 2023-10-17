#include <cmath>
#include <string>
#include "ros/ros.h"
#include "my_service/QuadraticEquation.h"
#include "std_msgs/Float32MultiArray.h"

ros::Publisher publisher;

bool solution(my_service::QuadraticEquation::Request &req, my_service::QuadraticEquation::Response &res);

void reader(const std_msgs::Float32MultiArray &message);

int main(int argc, char **argv)
{
    ros::init(argc, argv, "solution_server");
    ros::NodeHandle n;

    ros::ServiceServer service = n.advertiseService("quadratic_equation", solution);

    publisher = n.advertise<std_msgs::Float32MultiArray>("quadratic_eq_solution", 1000);
    ros::Subscriber sub = n.subscribe("quadratic_eq_solution", 1000, reader);

    ros::spin();
    return 0;
}

bool solution(my_service::QuadraticEquation::Request &req, my_service::QuadraticEquation::Response &res)
{
    ROS_INFO("request: a = %f,b = %f, c = %f\n", req.coef_a, req.coef_b, req.coef_c);
    std_msgs::Float32MultiArray answer;

    /// If it linear (coefficient a near 0)
    /// It is necessary to check BEFORE division by coefficient a
    if (std::fabs(req.coef_a) < 0.000001)
    {
        /// If (a == 0) && (b == 0) - infinity of solutions
        if (std::fabs(req.coef_b - 0) > 0.000001)
        {
            /// returns data in service call
            res.roots.push_back(-req.coef_c / req.coef_b);

            /// pushes data in message for topic
            answer.data.push_back(-req.coef_c / req.coef_b);
        }

        publisher.publish(answer);

        return true;
    }

    /// If it is quadratic equation
    float deskriminant = req.coef_b * req.coef_b - 4 * req.coef_a * req.coef_c;

    if (std::fabs(deskriminant) < 0.000001)
    {
        res.roots.push_back(-req.coef_b / (2 * req.coef_a));
        answer.data.push_back(-req.coef_b / (2 * req.coef_a));
    }
    else if (deskriminant > 0)
    {
        res.roots.push_back((-req.coef_b - std::sqrt(deskriminant)) / (2 * req.coef_a));
        res.roots.push_back((-req.coef_b + std::sqrt(deskriminant)) / (2 * req.coef_a));
        answer.data.push_back((-req.coef_b - std::sqrt(deskriminant)) / (2 * req.coef_a));
        answer.data.push_back((-req.coef_b + std::sqrt(deskriminant)) / (2 * req.coef_a));
    }

    publisher.publish(answer);

    return true;
}

void reader(const std_msgs::Float32MultiArray &message)
{
    std_msgs::Float32MultiArray array = message;
    char str[50];
    int iterator = 1;
    int message_size = 0;

    for (float i : array.data)
    {
        message_size = sprintf(str + message_size, "x%d = %.5f\n", iterator++, i);
    }

    if (iterator == 1)
    {
        sprintf(str, "no solution");
    }
    ROS_INFO_STREAM(str);
}