#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <agv/targetAction.h>

int main (int argc, char **argv)
{
  ros::init(argc, argv, "target_client");

  // create the action client
  // true causes the client to spin its own thread
  actionlib::SimpleActionClient<agv::targetAction> ac("targetAction", true);

  ROS_INFO("Waiting for action server to start.");
  // wait for the action server to start
  ac.waitForServer(); //will wait for infinite time

  ROS_INFO("Action server started, sending goal.");
  // send a goal to the action
  agv::targetGoal goal;
  goal.trigger.data = "Go Go!";
  ac.sendGoal(goal);

  ros::spin();
  return 0;
}