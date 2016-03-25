#include <ros/ros.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Twist.h>


ros::Publisher cmd_vel_pub;
ros::Subscriber irValue_sub;

void irValueCallback(const geometry_msgs::Vector3 &vector3)
{
  //omega_left = vector3.x;
  //omega_right  = vector3.y;

  geometry_msgs::TwistPtr cmd(new geometry_msgs::Twist());
  cmd->linear.x = -0.02;
  cmd->angular.z = 0.0005 * (vector3.x - vector3.y);
  cmd_vel_pub.publish(cmd);
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "mybot_autodocking");

  ros::NodeHandle nh;
  cmd_vel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 50);
  irValue_sub = nh.subscribe("/irValue", 10, irValueCallback);

  ros::spin(); // stay here...
  return 0;
}
