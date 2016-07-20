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

class Depth_viewer
{ 
 public:                                                      //public functions
	Depth_viewer()                                         //declare a constructor, same name as the class name.

		{
		 min_range_ = 0.0;
		 max_range_ = 1000; 
		}                             //member function declaration
	void depth_callback(const sensor_msgs::ImageConstPtr&);

    // void segmentation(Mat, Mat);
    // void laplacian(Mat);


    double min_range_,  max_range_; 
    Mat depth_img_msg;
};

 

void Depth_viewer::depth_callback(const sensor_msgs::ImageConstPtr& image)         //?????
{
    // convert to cv image
    cv_bridge::CvImagePtr bridge;
    try
    {
        bridge = cv_bridge::toCvCopy(image, "32FC1");
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Failed to transform depth image.");
        cout<<"error in subscribing image"<<endl;
        return;
    }

    // convert to something visible
    Mat depth_img_msg(bridge->image.rows, bridge->image.cols, CV_8UC1);
    for(int i = 0; i < bridge->image.rows; i++)
    {
        float* Di = bridge->image.ptr<float>(i);//get ith row of original image, index Di
        char* Ii = depth_img_msg.ptr<char>(i);//get ith row of img, index Ii
        for(int j = 0; j < bridge->image.cols; j++)
        {   
            Ii[j] = (char) (255*((Di[j]-min_range_)/(max_range_-min_range_))); //normalize and copy Di to Ii
            Ii[j] = (char) (255 - Ii[j]);
            // cout << (float)Ii[j]<<endl;
        }   
    }
    // cout << "the depth image :"<< depth_img_msg << endl;
    // cout << depth_img_msg.size() << endl;
    imshow("image_proc", depth_img_msg);
    Mat dst1, dst2;
    // cvAdaptiveThreshold( depth_img_msg, dst, 255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY,13, 1.0 );
    adaptiveThreshold(depth_img_msg, dst1, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, -2);
    imshow("adaptive thresholding", dst1);

    threshold(depth_img_msg, dst2, 100, 255, 0);
    imshow("thresholding", dst2);
    waitKey(1);
    // imshow("xxxxxx:::", bridge->image);
    //display
}



int main( int argc, char* argv[] )
{
    ros::init( argc, argv, "depth_viewer_node" );
    ros::NodeHandle n;
    ros::NodeHandle nh("~");

    Depth_viewer depth_viewer;
    
    ros::Subscriber depth_sub = n.subscribe("/o3d3xx/camera/depth", 3, &Depth_viewer::depth_callback, &depth_viewer);

    ros::spin();
}

