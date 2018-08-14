#include <opencv2/opencv.hpp>
#include <iostream>

cv::Mat srcImage,grayImage;
int nThresh=80,nThresh_max=255;
cv::RNG rng(12345);
cv::Mat cannyMat;
std::vector<std::vector<cv::Point> > Contours;
std::vector<cv::Vec4i> Hierarchy;

void on_ThreshChange(int ,void*){
	// use findContour to find line
	cv::Canny(grayImage, cannyMat, nThresh, nThresh*2,3);
	cv::findContours(cannyMat, Contours, Hierarchy, cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE,cv::Point(0,0));
	cv::Mat drawing=cv::Mat::zeros(cannyMat.size(),CV_8UC3);
	for(int i=0;i<Contours.size();i++)
		cv::drawContours(drawing, Contours, i, cv::Scalar(0,0,255));
	cv::imshow("withcanny",drawing);
	// find circles
	// std::vector<cv::Vec3f> circles;
	// cv::HoughCircles(grayImage, circles, CV_HOUGH_GRADIENT,2,grayImage.rows/4, 200, 100 );
	// for(int i=0;i<circles.size();i++){
	// 	cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
 //         int radius = cvRound(circles[i][2]);
 //         // draw the circle center
 //         cv::circle( grayImage, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
 //         // draw the circle outline
 //         cv::circle( grayImage, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
	// }
	// cv::imshow("withcanny", grayImage);
}


int main(int argc, char const *argv[])
{
	srcImage=cv::imread("BingWallpaper-2018-06-13.jpg",1);
	cv::resize(srcImage, srcImage, cv::Size(800,480));
	cv::cvtColor(srcImage, grayImage, cv::COLOR_BGR2GRAY);
	cv::blur(grayImage, grayImage, cv::Size(3,3));
	cv::namedWindow("origin",cv::WINDOW_AUTOSIZE);
	cv::imshow("origin", grayImage);

	cv::createTrackbar("canny", "origin", &nThresh, nThresh_max,on_ThreshChange);
	on_ThreshChange(0,0);
	cv::waitKey(0);
	return 0;
}