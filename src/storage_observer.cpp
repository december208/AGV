#include <ros/ros.h>
#include <string>
#include <std_msgs/String.h>
#include <tf2_ros/transform_listener.h>


ros::Publisher result_pub;
geometry_msgs::TransformStamped objTF;

double getRemainDist();

int main(int argc, char **argv){
    std::string objs[4]  = {"object11","object12","object13","object14"};
    ros::init(argc, argv, "storage_observer");
    ros::NodeHandle node;
    result_pub = node.advertise<std_msgs::String>("storedObj",1);
    tf2_ros::Buffer tfBuffer;
    tf2_ros::TransformListener tfListener(tfBuffer);
    double insideStorage = 140.0;
    ros::Rate rate(1.0);
    std_msgs::String findedObj;

    while(ros::ok()){
        int noObj = 1;
        rate.sleep();
        for(int i=0; i<4; i++){
            double dist2World = 0;
            try{
                objTF = tfBuffer.lookupTransform("world",objs[i],ros::Time(0));
            }
            catch(tf2::TransformException &ex){
                continue;
            }
            dist2World = getRemainDist();
            if(dist2World < insideStorage){
                noObj = 0;
                findedObj.data = objs[i];
                ROS_INFO("%s is inside the storage.",objs[i].c_str());
                result_pub.publish(findedObj);
            }
        }   
        if(noObj){
            findedObj.data = "None";
            result_pub.publish(findedObj);
            ROS_INFO("There is no object in storage.");
        }
    }
}

double getRemainDist(){
    double dx,dy;
    dx = objTF.transform.translation.x;
    dy = objTF.transform.translation.y;
    return sqrt(dx*dx+dy*dy)*1000.0;
}