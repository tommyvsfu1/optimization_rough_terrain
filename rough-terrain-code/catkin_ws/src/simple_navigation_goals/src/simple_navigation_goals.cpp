#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/Bool.h>
#include <cstdlib>
#include "huskyterrain/reach.h"
#include <iostream>
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

class navigation {
protected:
  ros::Publisher chatter ;
  ros::Publisher pub_ ; 
  ros::Publisher pub1_ ; 
  ros::ServiceServer service;
  geometry_msgs::Point des;
  std_msgs::Bool reach;
  std_msgs::Bool reach1;
  move_base_msgs::MoveBaseGoal goal;
  int discreteMode;
  MoveBaseClient ac;
  ros::Rate loop_rate;
  
public:
	const double patchSize = 0.9;
	ros::NodeHandle nh;
	bool askFuck(huskyterrain::reach::Request  &req,huskyterrain::reach::Response &res);
	navigation(): ac("move_base", true),loop_rate(1000) { 
		//this->service = nh.advertiseService("goal",&navigation::askFuck);
  		chatter = nh.advertise<geometry_msgs::Point>("destination",1); 
  		pub_ = nh.advertise<std_msgs::Bool>("reachgoal",1); 
  		pub1_ = nh.advertise<std_msgs::Bool>("reachgoal1",1); 
  		
  		//tell the action client that we want to spin a thread by default
  		
  		//wait for the action server to come up
  		while(!ac.waitForServer(ros::Duration(5.0))){
   		 ROS_INFO("Waiting for the move_base action server to come up");
  		}
  
  		//we'll send a goal to the robot to move 1 meter forward
  		goal.target_pose.header.frame_id = "odom";
  		goal.target_pose.header.stamp = ros::Time::now();
		
  		goal.target_pose.pose.position.x = 0.9;
  		goal.target_pose.pose.position.y = 0.45;
  		goal.target_pose.pose.orientation.w = 1.0;
		//const float patchSize = 0.9;
  		des.x = 0.0;
  		des.y = 0.45;
  		discreteMode = 1;	
	}
	void Goalreaching() {
  		while(1) {
  		ROS_INFO("Sending goal");
		nh.setParam("desx",des.x);
		nh.setParam("desy",des.y);
		nh.setParam("goal","going");
  		//reach.data = false;
  		std::cout << des.x << " " << des.y << std::endl; 
  		chatter.publish(des);
  		//pub1_.publish(reach);
  		loop_rate.sleep();
  		ac.sendGoal(goal);
  		ac.waitForResult();
  		if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
		 nh.setParam("goal","reaching");
		 ros::Duration(1.5).sleep(); // sleep for half a second
   		 reach1.data = true;  
   		 ROS_INFO("Hooray, the base moved 1 meter forward");
   		 if (goal.target_pose.pose.position.x == 0.0 && goal.target_pose.pose.position.y == 0.0) {
     		   discreteMode = -1;
			break;
    		 }
   		 else if (goal.target_pose.pose.position.x == 4.5){
				if (discreteMode != 3) {
    		    			discreteMode = 3;
					des.x += 0.9;
				}
				else 
			   		discreteMode = 2;
   		 		}
   		else if (fabs(goal.target_pose.pose.position.x - 0.0) < 0.01) {
				if (discreteMode != 3) {
    		    			discreteMode = 3;
					des.x -= 0.9;
				}
				else 
			   		discreteMode = 1;
   		 }  
   		 else {
			   discreteMode = discreteMode;
    		 }

    			if (discreteMode == 1) {
			   if (goal.target_pose.pose.position.y - des.y > 0.5) {
				des.y += 0.9;
			   }
			   else
			  	des.x = des.x + 0.9;
      			  goal.target_pose.pose.position.x = goal.target_pose.pose.position.x +patchSize;
			   // des.x = goal.target_pose.pose.position.x - patchSize;
       			 goal.target_pose.pose.orientation.w = 1.00;
			goal.target_pose.pose.orientation.z = 0.00;
	
    			}
    			else if (discreteMode == 2) {
			   if (goal.target_pose.pose.position.y - des.y > 0.5) {
				des.y += 0.9;
			   }
			   else 
			  	des.x = des.x - 0.9;
			goal.target_pose.pose.position.x= goal.target_pose.pose.position.x -patchSize;
			 // des.x = goal.target_pose.pose.position.x + patchSize;
        		goal.target_pose.pose.orientation.w = 0.00;
			goal.target_pose.pose.orientation.z = 1.00;
    			}
    			else if (discreteMode == 3){
			//des.y = des.y + 0.9;
        		goal.target_pose.pose.position.y = goal.target_pose.pose.position.y +patchSize;
			//des.y = goal.target_pose.pose.position.y - patchSize;
        		goal.target_pose.pose.orientation.w = 0.70; //90 degrees
			goal.target_pose.pose.orientation.z = 0.70;
    			}
    			else {
				break;
    			}
  		}
  		else {
    			ROS_INFO("The base failed to move forward 1 meter for some reason");
    			break;
  		}
 			//pub_.publish(reach);
			//loop_rate.sleep();
  		}


	}

};



int main(int argc, char** argv){
  ros::init(argc, argv, "simple_navigation_goals");
  navigation UAV;
  
  UAV.Goalreaching();
  return 0;
}
