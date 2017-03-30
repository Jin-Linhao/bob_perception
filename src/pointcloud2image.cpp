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

#include <tf/transform_listener.h>
#include <laser_geometry/laser_geometry.h>


using namespace std;
using namespace cv;



typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

class Pointcloud_to_image
{
  public:
    //function header
    void pointcloud_callback( const sensor_msgs::PointCloud2ConstPtr& );
    // void help();

    //ros header
    ros::Publisher  pub;
    ros::Subscriber sub; 
};

void Pointcloud_to_image::pointcloud_callback( const sensor_msgs::PointCloud2ConstPtr& msg )
{
    printf ("Cloud: width = %d, height = %d\n", msg->width, msg->height);
    // BOOST_FOREACH (const pcl::PointXYZ& pt, msg->points)
    // printf ("\t(%f, %f, %f)\n", pt.x, pt.y, pt.z);

}






class My_Filter {
     public:
        My_Filter();
        void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan);
     private:
        ros::NodeHandle node_;
        // laser_geometry::LaserProjection projector_;
        // tf::TransformListener tfListener_;

        ros::Publisher point_cloud_publisher_;
        ros::Subscriber scan_sub_;
};

My_Filter::My_Filter(){
        scan_sub_ = node_.subscribe<sensor_msgs::LaserScan> ("/scan", 100, &My_Filter::scanCallback, this);
        point_cloud_publisher_ = node_.advertise<sensor_msgs::PointCloud2> ("/cloud", 100, false);
        // tfListener_.setExtrapolationLimit(ros::Duration(0.1));
}


void My_Filter::scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan){
    sensor_msgs::PointCloud2 cloud;
    // projector_.transformLaserScanToPointCloud("base_link", *scan, cloud, tfListener_);
    point_cloud_publisher_.publish(cloud);
}




// int main( int argc, char* argv[] )
// {
//     ros::init( argc, argv, "pointcloud2image_node" );
//     ros::NodeHandle n;
//     ros::NodeHandle nh("~");

//     Pointcloud_to_image pointcloud2image;

//     ros::Subscriber sub = n.subscribe("/camera/depth/points", 100, &Pointcloud_to_image::pointcloud_callback, &pointcloud2image);

//     ros::spin();
// }

int main(int argc, char** argv)
{
    ros::init(argc, argv, "my_filter");
    // ros::NodeHandle n;

    My_Filter filter;

    // ros::Subscriber sub = n.subscribe("/scan", 1, &My_Filter::scanCallback, &filter);

    ros::spin();

    return 0;
}