#include <iostream>
#include <opencv2/opencv.hpp>

static int corner=1;
cv::Mat image,gray;
void On_Track(int ,void*){
	std::vector<cv::Point2f> corners;
	cv::Mat clone=image.clone();
	cv::goodFeaturesToTrack(gray,corners,corner?corner:1, 0.01,10);
	for(int i=0;i<corners.size();i++){
		cv::circle(clone, corners[i], 4, cv::Scalar(0,0,255));
	}
	cv::imshow("goodFeatures", clone);
}

// int main(int argc, char const *argv[])
// {
// 	image=cv::imread("BingWallpaper-2018-06-13.jpg");
// 	cv::resize(image, image, cv::Size(800,480));
// 	cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
// 	cv::namedWindow("goodFeatures",cv::WINDOW_AUTOSIZE);
// 	cv::createTrackbar("Features", "goodFeatures", &corner,500,On_Track);
// 	cv::imshow("goodFeatures",image);
// 	On_Track(0,0);
// 	cv::waitKey(0);
// 	return 0;
// }