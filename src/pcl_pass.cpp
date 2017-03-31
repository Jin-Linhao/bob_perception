#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/passthrough.h>

int
main (int argc, char** argv)
{
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered_x (new pcl::PointCloud<pcl::PointXYZ>);
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered_y (new pcl::PointCloud<pcl::PointXYZ>);

  // Fill in the cloud data
  pcl::PCDReader reader;
  // Replace the path below with the path where you saved your file
  reader.read<pcl::PointXYZ> ("/home/lh/catkin_ws/src/bob_perception/data/rrc1_inliers.pcd", *cloud);

  std::cerr << "Cloud before filtering: " << std::endl;
  std::cerr << *cloud << std::endl;


  pcl::PassThrough<pcl::PointXYZ> pass_x;
  pass_x.setInputCloud (cloud);
  pass_x.setFilterFieldName ("x");
  pass_x.setFilterLimits (0.5, 20.0);
  //pass.setFilterLimitsNegative (true);
  pass_x.filter (*cloud_filtered_x);

  pcl::PassThrough<pcl::PointXYZ> pass_y;
  pass_y.setInputCloud (cloud_filtered_x);
  pass_y.setFilterFieldName ("y");
  pass_y.setFilterLimits (-10, 6.0);
  //pass.setFilterLimitsNegative (true);
  pass_y.filter (*cloud_filtered_y);


  std::cerr << "Cloud finish filtering: " << std::endl;
  // for (size_t i = 0; i < cloud_filtered->points.size (); ++i)
  //   std::cerr << "    " << cloud_filtered->points[i].x << " " 
  //                       << cloud_filtered->points[i].y << " " 
  //                       << cloud_filtered->points[i].z << std::endl;


  pcl::PCDWriter writer;
  writer.write<pcl::PointXYZ> ("/home/lh/catkin_ws/src/bob_perception/data/rrc1_pass.pcd", *cloud_filtered_y, false);
   return (0);
}