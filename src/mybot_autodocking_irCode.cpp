#include <ros/ros.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Twist.h>


ros::Publisher cmd_vel_pub;
ros::Subscriber irValue_sub;

unsigned int foundDockingStation = 0;
geometry_msgs::Vector3 vec3;

void irValueCallback(const geometry_msgs::Vector3 &vector3)
{
  //omega_left = vector3.x;
  //omega_right  = vector3.y;

  
  vec3.x = vector3.x;
  vec3.y = vector3.y;

  if (foundDockingStation) {
    geometry_msgs::TwistPtr cmd(new geometry_msgs::Twist());
    cmd->linear.x = -0.08;
    cmd->angular.z = 0.1 * (vector3.x - vector3.y);
    if (vector3.x || vector3.y)
      cmd_vel_pub.publish(cmd);
  }
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "mybot_autodocking_irCode");

  ros::NodeHandle nh;
  cmd_vel_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 50);
  irValue_sub = nh.subscribe("/irCode", 10, irValueCallback);

  vec3.x = 0; //initiallized with some big values
  vec3.y = 0;

  ros::Rate r(10);

  while (ros::ok())
  {
    ros::spinOnce(); 

    printf("x=%f, y=%f\n", vec3.x, vec3.y);
    if (!foundDockingStation && vec3.x == 0.0 && vec3.y == 0.0) {
      geometry_msgs::TwistPtr cmd(new geometry_msgs::Twist());
      cmd->angular.z = 0.1; // const rotate until 
      cmd->linear.x = 0.0;
      cmd_vel_pub.publish(cmd);
    } else {
      foundDockingStation = 1;
    }
      
    r.sleep();
  }
  return 0;
}
