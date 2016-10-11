/**
 * @file HoughLines_Demo.cpp
 * @brief Demo code for Hough Transform
 * @author OpenCV team
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

/// Global variables

/** General variables */
Mat src, edges, dst;
Mat src_gray;
vector<Vec4i> p_lines;
/// Function Headers

void help()
{
  printf("\t NO IMAGE \n ");
  printf("\t Hough Transform to detect lines \n ");
  printf("\t---------------------------------\n ");
  printf(" Usage: ./HoughLines_Demo <image_name> \n");
}


/**
 * @function main
 */
int main( )
{
   /// Read the image
   Mat Structure1 = getStructuringElement(MORPH_RECT, Size(13,21));
   Mat Structure2 = getStructuringElement(MORPH_RECT, Size(11,13));
   Mat Structure3 = getStructuringElement(MORPH_ELLIPSE, Size(5,5));
   Mat Structure4 = getStructuringElement(MORPH_ELLIPSE, Size(15,19));
   Mat Structure5 = getStructuringElement(MORPH_RECT, Size(3,120));
   src = imread( "/home/lh/catkin_ws/src/SMART_ZED/data/pic/border2.JPG", CV_LOAD_IMAGE_COLOR);
   imshow("origin", src);
   cout << src.size() << endl;
   if( src.empty() )
     { help();
       return -1;
     }

   /// Pass the image to gray
   cvtColor( src, src_gray, COLOR_RGB2GRAY );
   imshow("gray", src_gray);
   threshold( src_gray, dst, 180, 255,THRESH_BINARY );
   // adaptiveThreshold( src_gray, dst, 255, CV_ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 15, -2);
   imshow("threshold", dst);

   // erode(dst, dst, Structure1, Point(-1, -1));
   dilate(dst, dst, Structure1, Point(-1, -1));
   // erode(dst, dst, Structure2, Point(-1, -1));
   // dilate(dst, dst, Structure2, Point(-1, -1));
   Canny( dst, edges, 50, 200, 3 );
   // erode(edges, edges, Structure3, Point(-1, -1));
   dilate(edges, edges, Structure4, Point(-1, -1));
   erode(edges, edges, Structure5, Point(-1, -1));
   imshow("threshold22", dst);
   imshow("threshold222", edges);
   waitKey(0);

   HoughLinesP( edges, p_lines, 1, CV_PI/180, 100, 100, 10 );
 	for( size_t i = 0; i < p_lines.size(); i++ )
     {
  	  cout<<"p_lines size"<<p_lines.size()<<endl;	
        // Vec4i line_vector = p_lines[i];
  	 }
   // cout << p_lines << endl;
}

/**
 * @function help
 * @brief Indications of how to run this program and why is it for
 */
