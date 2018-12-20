#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <agv/targetAction.h>
#include <string>


int isObjectWaiting = 0;
std::string  objectWaiting = "";

void storeObjCB(const std_msgs::StringConstPtr& obj);
void activeCB();
void doneCB(const actionlib::SimpleClientGoalState& state, const agv::targetResultConstPtr& result);
void feedbackCB(const agv::targetActionFeedbackConstPtr& feedback);

int main (int argc, char **argv)
{
  ros::init(argc, argv, "target_client");
  ros::NodeHandle node;
  ros::Subscriber sub = node.subscribe("storedObj", 1, &storeObjCB);

  actionlib::SimpleActionClient<agv::targetAction> ac("targetAction", true);

  ROS_INFO("Waiting for action server to start.");
  ac.waitForServer();

  ROS_INFO("Action server started, sending goal.");
  // send a goal to the action
  agv::targetGoal goal;
  //goal.trigger.data = "release object";
  //goal.trigger.data = "object12";
  goal.trigger.data = "target";
  ac.sendGoal(goal,&doneCB,&activeCB,&feedbackCB);

  ros::spin();
  return 0;
}

void storeObjCB(const std_msgs::StringConstPtr& obj){
  if(obj->data == "None"){
    isObjectWaiting = 0;
  }
  else{
    isObjectWaiting = 0;
    objectWaiting = obj->data;
  }
}
void doneCB(const actionlib::SimpleClientGoalState& state, const agv::targetResultConstPtr& result){
  ROS_INFO("Finished in state [%s]", state.toString().c_str());
  return;
}
void activeCB(){
  ROS_INFO("Goal just went active");
  return;
}
void feedbackCB(const agv::targetActionFeedbackConstPtr& feedback){
  ROS_INFO("Got Feedback of %lu", feedback->feedback.progress.data);
  return;
}
