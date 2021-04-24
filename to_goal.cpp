//program to move the turtle to a goal position

//Headers

#include "ros/ros.h"
//Header for publish
#include "geometry_msgs/Twist.h"
//Header to subscribe
#include "turtlesim/Pose.h"
#include <sstream>




// Named publisher, subscriber and mesagges
ros::Publisher vel_publisher;
ros::Subscriber pose_subscriber;
turtlesim::Pose turtlesim_pose;
geometry_msgs::Twist vel_msg;

using namespace std;

//function main initialization
void poseCallback(const turtlesim::Pose::ConstPtr & pose_message);
void move_goal(turtlesim::Pose goal_pose,double distance_tolerance);
double distance_goal(double x1, double x2, double y1, double y2);
//main initial
int main(int argc, char **argv){

//Initialitate node
ros::init(argc,argv,"to_goal");
ros::NodeHandle n;

// connection of nodes
vel_publisher = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",10);
pose_subscriber = n.subscribe("/turtle1/pose",10,poseCallback);
// loop frequency
ros::Rate loop_rate(10);

double x_pose,y_pose,theta_pose;


cout<<"Enter desired x pose: ";
cin>>x_pose;
cout<<"Enter desired y pose: ";
cin>>y_pose;
cout<<"Enter desired theta pose: ";
cin>>theta_pose;

//run the program
turtlesim::Pose goal_pose;
goal_pose.x=x_pose;
goal_pose.y=y_pose;
goal_pose.theta = theta_pose;

//double distance_tolerance = 0.1;
move_goal(goal_pose,0.01);
loop_rate.sleep();
return 0;
//loop the callback
ros::spin();


}
// function to move the turtle to the goal pose
void move_goal(turtlesim::Pose goal_pose,double distance_tolerance){
  geometry_msgs::Twist  vel_msg;

//initializate the loop frequency
  ros::Rate loop_rate(10);
    do { //linear velocities
         vel_msg.linear.x = 1.5*distance_goal(goal_pose.x,turtlesim_pose.x,goal_pose.y,turtlesim_pose.y);
         vel_msg.linear.y = 0;
         vel_msg.linear.z = 0;
        //angular velocities
          vel_msg.angular.x = 0;
          vel_msg.angular.y = 0;
          vel_msg.angular.z = 4*(atan2(goal_pose.y - turtlesim_pose.y, goal_pose.x - turtlesim_pose.x)-turtlesim_pose.theta);
          //publish the message in topic
          vel_publisher.publish(vel_msg);
      //loop frequency
         ros::spinOnce();
        loop_rate.sleep(); 
      }while(distance_goal(goal_pose.x,turtlesim_pose.x,goal_pose.y,turtlesim_pose.y) > distance_tolerance);
         vel_msg.linear.x = 0;
         vel_msg.angular.z = 0;
         vel_publisher.publish(vel_msg);
     }
       

//function distance_goal
double distance_goal(double x1, double x2, double y1, double y2){
  return sqrt(pow((x2-x1),2)+pow((y2-y1),2));
} 

void poseCallback(const turtlesim::Pose::ConstPtr & pose_message){
   turtlesim_pose.x = pose_message -> x;
   turtlesim_pose.y = pose_message -> y;
   turtlesim_pose.theta = pose_message -> theta;
}

