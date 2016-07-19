#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>


#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/range_image/range_image.h>
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
    int range_image_converter();
    // void help();

    //parameters
    pcl::PointCloud<pcl::PointXYZ>::Ptr pcl_cloud;


    //ros header
    ros::Publisher  pub;
    ros::Subscriber sub; 
};

void Pointcloud_to_image::pointcloud_callback(const sensor_msgs::PointCloud2ConstPtr& msg)
{
    pcl::PCLPointCloud2 pcl_pc2;
    pcl_conversions::toPCL(*msg, pcl_pc2);
    // pcl::PointCloud<pcl::PointXYZ>::Ptr pcl_cloud(new pcl::PointCloud<pcl::PointXYZ>);
    pcl::fromPCLPointCloud2(pcl_pc2,*pcl_cloud);
    //do stuff with temp_cloud here
    Pointcloud_to_image::range_image_converter();
}


int Pointcloud_to_image::range_image_converter() 
{
  // pcl::PointCloud<pcl::PointXYZ> pointCloud;
  
  // Generate the data
  for (float y=-0.5f; y<=0.5f; y+=0.01f) {
    for (float z=-0.5f; z<=0.5f; z+=0.01f) {
      pcl::PointXYZ point;
      point.x = 2.0f - y;
      point.y = y;
      point.z = z;
      pcl_cloud->points.push_back(point);
    }
}
  pcl_cloud->width = (uint32_t) pcl_cloud->points.size();
  pcl_cloud->height = 1;
  
  // We now want to create a range image from the above point cloud, with a 1deg angular resolution
  float angularResolution = (float) (  1.0f * (M_PI/180.0f));  //   1.0 degree in radians
  float maxAngleWidth     = (float) (360.0f * (M_PI/180.0f));  // 360.0 degree in radians
  float maxAngleHeight    = (float) (180.0f * (M_PI/180.0f));  // 180.0 degree in radians
  Eigen::Affine3f sensorPose = (Eigen::Affine3f)Eigen::Translation3f(0.0f, 0.0f, 0.0f);
  pcl::RangeImage::CoordinateFrame coordinate_frame = pcl::RangeImage::CAMERA_FRAME;
  float noiseLevel=0.00;
  float minRange = 0.0f;
  int borderSize = 1;
  
  pcl::RangeImage rangeImage;
  rangeImage.createFromPointCloud(*pcl_cloud, angularResolution, maxAngleWidth, maxAngleHeight,
                                  sensorPose, coordinate_frame, noiseLevel, minRange, borderSize);
  
  std::cout << rangeImage << "\n";
}




int main( int argc, char* argv[] )
{
    ros::init( argc, argv, "pointcloud2iamge_node" );
    ros::NodeHandle n;
    ros::NodeHandle nh("~");

    Pointcloud_to_image pointcloud2image;
    std::string topic = nh.resolveName("/o3d3xx/camera/cloud");
    uint32_t queue_size = 1;
    // pointcloud2image.pub = n.advertise<std_msgs::Int32MultiArray>("hough_line",1);
    // pointcloud2image.sub = n.subscribe<PointCloud>("/camera/cloud/point_cloud", 100, 
    //                                                 &Pointcloud_to_image::pointcloud_callback, &pointcloud2image);  
    ros::Subscriber sub = n.subscribe <sensor_msgs::PointCloud2> (topic, queue_size, &Pointcloud_to_image::pointcloud_callback, &pointcloud2image);
    // ros::Subscriber sub = nh.subscribe(topic, queue_size, callback);
    ros::spin();
}
