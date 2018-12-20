#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <agv/targetAction.h>
#include <string>
#include <geometry_msgs/Pose.h>

int isObjectWaiting = 0;
std::string  objectWaiting = "";
std::string status = "idle";
ros::Publisher target_pub;

void storeObjCB(const std_msgs::StringConstPtr& obj);
void grabObj(actionlib::SimpleActionClient<agv::targetAction>& ac);
//void goHome(actionlib::SimpleActionClient<agv::targetAction>& ac);

void activeCB();
void doneCB(const actionlib::SimpleClientGoalState& state, const agv::targetResultConstPtr& result);
void feedbackCB(const agv::targetFeedbackConstPtr& feedback);

int main (int argc, char **argv)
{
  ros::init(argc, argv, "target_client");
  ros::NodeHandle node;
  ros::Subscriber sub = node.subscribe("storedObj", 1, &storeObjCB);
  target_pub = node.advertise<geometry_msgs::Pose>("set_target",3);
  ros::Rate rate(5);

  actionlib::SimpleActionClient<agv::targetAction> ac("targetAction", true);

  ROS_INFO("Waiting for action server to start.");
  ac.waitForServer(); 

  while(ros::ok()){
    rate.sleep();
    if((status == "idle")&&isObjectWaiting){
      grabObj(ac);
    }
    if((status == "idle")&&(!isObjectWaiting)){
      //goHome(ac);
    }
    ros::spinOnce();
  }
  return 0;
}

void grabObj(actionlib::SimpleActionClient<agv::targetAction>& ac){
  status = "running";
  geometry_msgs::Pose targetPoint;
  targetPoint.position.x = 0.0;
  targetPoint.position.y = 0.3;
  target_pub.publish(targetPoint);
  agv::targetGoal middle_point_goal;
  middle_point_goal.trigger.data = "target";
  middle_point_goal.distanceTolerence.data = 30.0;
  ac.sendGoalAndWait(middle_point_goal,ros::Duration(0, 0),ros::Duration(0, 0));
  ROS_INFO("Arrive mid point!");

  agv::targetGoal grap_goal;
  middle_point_goal.trigger.data = objectWaiting;
  middle_point_goal.distanceTolerence.data = 5.0;
  ac.sendGoalAndWait(middle_point_goal,ros::Duration(0, 0),ros::Duration(0, 0));
  ROS_INFO("Catch Object!");

  middle_point_goal.trigger.data = "goback";
  middle_point_goal.distanceTolerence.data = 5.0;
  ac.sendGoalAndWait(middle_point_goal,ros::Duration(0, 0),ros::Duration(0, 0));
  ROS_INFO("Go Backward!");


  targetPoint.position.x = 0.35;
  targetPoint.position.y = 0.5;
  target_pub.publish(targetPoint);
  middle_point_goal.trigger.data = "target";
  middle_point_goal.distanceTolerence.data = 30.0;
  ac.sendGoalAndWait(middle_point_goal,ros::Duration(0, 0),ros::Duration(0, 0));
  ROS_INFO("Arrive Mid Point");

  targetPoint.position.x = 0.35;
  targetPoint.position.y = 0.55;
  target_pub.publish(targetPoint);
  middle_point_goal.trigger.data = "target";
  middle_point_goal.distanceTolerence.data = 15.0;
  ac.sendGoalAndWait(middle_point_goal,ros::Duration(0, 0),ros::Duration(0, 0));
  ROS_INFO("Arrive final Point");

  middle_point_goal.trigger.data = "release";
  ac.sendGoalAndWait(middle_point_goal,ros::Duration(0, 0),ros::Duration(0, 0));
  ROS_INFO("Release");

  targetPoint.position.x = 0.20;
  targetPoint.position.y = 0.30;
  target_pub.publish(targetPoint);
  middle_point_goal.trigger.data = "target";
  middle_point_goal.distanceTolerence.data = 35.0;
  ac.sendGoalAndWait(middle_point_goal,ros::Duration(0, 0),ros::Duration(0, 0));
  ROS_INFO("Home");
  status = "idle";
}

void storeObjCB(const std_msgs::StringConstPtr& obj){
  if(obj->data == "None"){
    isObjectWaiting = 0;
  }
  else{
    isObjectWaiting = 1;
    objectWaiting = obj->data;
  }
}

void doneCB(const actionlib::SimpleClientGoalState& state, const agv::targetResultConstPtr& result){
  ROS_INFO("Finished in state [%s]", state.toString().c_str());
}
void activeCB(){
  ROS_INFO("Goal just went active");
}
void feedbackCB(const agv::targetFeedbackConstPtr& feedback){
  ROS_INFO("Got Feedback of %f", feedback->progress.data);
}