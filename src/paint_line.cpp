/**
 * @file paint_line.cpp
 * @detect the border(vertical) of each paints
 * @author JIN Linhao
 */

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

/// Global variables

/** General variables */
Mat src, edges, dst;
Mat src_gray;
vector<Vec4i> p_lines;
Mat Structure = getStructuringElement(MORPH_RECT, Size(13,21));
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
   src = imread( "/home/lh/catkin_ws/src/SMART_ZED/data/pic/border2.JPG", CV_LOAD_IMAGE_COLOR);
   cout << src.size() << endl;
   if( src.empty() )
     { help();
       return -1;
     }

   /// Pass the image to gray
   cvtColor( src, src_gray, COLOR_RGB2GRAY );

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
   line(src, Point(border_avg1, 0), Point(border_avg1, 480), Scalar(0,0,255), 1, CV_AA);

   imshow("origin", src);
   imshow("threshold", dst);
   imshow("threshold222", edges);
   waitKey(0);
}

/**
 * @function help
 * @brief Indications of how to run this program and why is it for
 */
