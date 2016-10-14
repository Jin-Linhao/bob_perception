/**
 * @file paint_line.cpp
 * @detect the border(vertical) of each paints
 * @author JIN Linhao
 */
#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <opencv2/opencv.hpp>
#include <sensor_msgs/Image.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <iomanip>
#include <std_msgs/Int32MultiArray.h>


using namespace std;
using namespace cv;


/// Global variables

/** General variables */

class Paint_border
{ 
 public:                                                      //public functions
  Paint_border()                                         //declare a constructor, same name as the class name.

    {
      // Mat src, edges, dst;
      // Mat src_gray;
      // vector<Vec4i> p_lines;
      // Mat Structure = getStructuringElement(MORPH_RECT, Size(13,21));
    }                             //member function declaration

      Mat src, edges, dst;
      Mat src_gray;
      vector<Vec4i> line_points;
      Mat Structure = getStructuringElement(MORPH_RECT, Size(13,21));

  void help();
  void paint_border_callback(const sensor_msgs::ImageConstPtr&);

    // void segmentation(Mat, Mat);
    // void laplacian(Mat);
  ros::Subscriber paint_sub;
  ros::Publisher paint_pub;
};




void Paint_border::help()
{
  printf("\t NO IMAGE \n ");
  printf("\t Hough Transform to detect lines \n ");
  printf("\t---------------------------------\n ");
  printf(" Usage: ./HoughLines_Demo <image_name> \n");
}


/**
 * @function main
 */
void Paint_border::paint_border_callback(const sensor_msgs::ImageConstPtr& image)
{
  cv_bridge::CvImagePtr bridge;
  try
  {
      bridge = cv_bridge::toCvCopy(image, sensor_msgs::image_encodings::BGR8);
  }
  catch (cv_bridge::Exception& e)
  {
      ROS_ERROR("Failed to transform rgb image.");
      cout<<"error in subscribing image"<<endl;
      return;
  }


  if( bridge->image.empty() )
   { help();
     return;
   }

  /// Pass the image to gray
  cvtColor( bridge->image, src_gray, COLOR_RGB2GRAY );

  threshold( src_gray, dst, 180, 255,THRESH_BINARY );
  // adaptiveThreshold( src_gray, dst, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
  dilate(dst, dst, Structure, Point(-1, -1));

  Canny( dst, edges, 50, 200, 3 );


  Mat nonZeroCoordinates;
  int sum_nonzero = 0; float border_avg = 0;

  findNonZero(edges, nonZeroCoordinates);
  for (int i = 0; i < nonZeroCoordinates.total(); i++ ) 
  {
      // cout << "Zero#" << i << ": " << nonZeroCoordinates.at<Point>(i).x << ", " << nonZeroCoordinates.at<Point>(i).y << endl;
      sum_nonzero = sum_nonzero + nonZeroCoordinates.at<Point>(i).x;
  }

  int border_avg1 = int (sum_nonzero/nonZeroCoordinates.total() + 0.5);
  // cout << border_avg1 <<endl;

  line(edges, Point(border_avg1, 0), Point(border_avg1, 480), Scalar(0,0,255), 1, CV_AA);
  line(bridge->image, Point(border_avg1, 0), Point(border_avg1, 480), Scalar(0,0,255), 1, CV_AA);

  imshow("origin", bridge->image);
  imshow("threshold", dst);
  imshow("threshold222", edges);
  waitKey(1);

  std_msgs::Int32MultiArray line_points_array;
  line_points_array.data.push_back(border_avg1);
  line_points_array.data.push_back(0);
  line_points_array.data.push_back(border_avg1);
  line_points_array.data.push_back(0);

  paint_pub.publish(line_points_array);
}

/**
 * @function help
 * @brief Indications of how to run this program and why is it for
 */
int main( int argc, char* argv[] )
{
    ros::init( argc, argv, "paint_border_node" );
    ros::NodeHandle n;
    ros::NodeHandle nh("~");

    Paint_border paint_border;

    paint_border.paint_pub = n.advertise<std_msgs::Int32MultiArray>("paint_line",100);
    paint_border.paint_sub = n.subscribe("/camera/color/image_raw", 3, &Paint_border::paint_border_callback, &paint_border);

    ros::spin();
}
