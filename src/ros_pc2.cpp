#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>


#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <boost/foreach.hpp>
#include <sensor_msgs/PointCloud2.h>
#include <fstream>
using namespace std;
using namespace cv;



typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

class Pointcloud_to_image
{
  public:
    //function header
    void pointcloud_callback(const sensor_msgs::PointCloud2ConstPtr&);
    // void help();

    //ros header
    ros::Publisher  pub;
    ros::Subscriber sub; 
};

void Pointcloud_to_image::pointcloud_callback(const sensor_msgs::PointCloud2ConstPtr& msg)
{
    pcl::PCLPointCloud2 pcl_pc2;
    pcl_conversions::toPCL(*msg,pcl_pc2);
    pcl::PointCloud<pcl::PointXYZ>::Ptr pcl_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::fromPCLPointCloud2(pcl_pc2,*pcl_cloud);
    //do stuff with temp_cloud here
}



int main( int argc, char* argv[] )
{
    ros::init( argc, argv, "pointcloud2iamge_node" );
    ros::NodeHandle n;
    ros::NodeHandle nh("~");

    Pointcloud_to_image pointcloud2image;
    std::string topic = nh.resolveName("point_cloud");
    uint32_t queue_size = 1;
    // pointcloud2image.pub = n.advertise<std_msgs::Int32MultiArray>("hough_line",1);
    // pointcloud2image.sub = n.subscribe<PointCloud>("/camera/cloud/point_cloud", 100, 
    //                                                 &Pointcloud_to_image::pointcloud_callback, &pointcloud2image);  
    ros::Subscriber sub = n.subscribe <sensor_msgs::PointCloud2> (topic, queue_size, &Pointcloud_to_image::pointcloud_callback, &pointcloud2image);
    // ros::Subscriber sub = nh.subscribe(topic, queue_size, callback);
    ros::spin();
}
