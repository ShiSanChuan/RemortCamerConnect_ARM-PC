#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>

int main(int argc, char const *argv[])
{
	cv::Mat image=cv::imread("book.png");
	cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
	// std::cout<<image<<std::endl;
	cv::imshow("origin", image);

	std::vector<cv::KeyPoint> train_keypoint;
	cv::Mat trainDescriptor;
	cv::SurfFeatureDetector featureDetector(15);
	featureDetector.detect(image, train_keypoint);
	cv::SurfDescriptorExtractor featureExtractor;
	featureDetector.compute(image, train_keypoint, trainDescriptor);

	cv::FlannBasedMatcher matcher;
	std::vector<cv::Mat> train_desc(1,trainDescriptor);
	matcher.add(train_desc);
	matcher.train();

	cv::VideoCapture cap(0);
	cv::Mat frame;
	while(char(cv::waitKey(1))!='q'){
		cap>>frame;
		cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
		// cv::blur(frame, frame, cv::Size(3,3));
		std::vector<cv::KeyPoint> test_keypoint;
		cv::Mat testDescriptor;
		featureDetector.detect(frame, test_keypoint);
		featureDetector.compute(frame, test_keypoint, testDescriptor);

		std::vector<std::vector<cv::DMatch> > matches;
		matcher.knnMatch(testDescriptor, matches, 2);

		std::vector<cv::DMatch> goodmatch;
		for(int i=0;i<matches.size();i++){
			if(matches[i][0].distance<0.6*matches[i][1].distance)
				goodmatch.push_back(matches[i][0]);
		}
		cv::Mat dstimage;
		cv::drawMatches(frame, test_keypoint, image, train_keypoint, goodmatch, dstimage);
		if(goodmatch.size()>10){
				std::vector<cv::Point2f> obj;
				std::vector<cv::Point2f> scene;
				for(int i=0;i<goodmatch.size();i++){
					obj.push_back(train_keypoint[goodmatch[i].queryIdx].pt);
					scene.push_back(test_keypoint[goodmatch[i].trainIdx].pt);
				}
		
				cv::Mat H=cv::findHomography(obj, scene);
				std::vector<cv::Point2f> obj_corner;
				std::vector<cv::Point2f> scene_corner;
				obj_corner[0]=cv::Point2f(0,0);
				obj_corner[1]=cv::Point2f(image.cols,0);
				obj_corner[2]=cv::Point2f(image.cols,image.rows);
				obj_corner[3]=cv::Point2f(0,image.rows);
				// cv::perspectiveTransform(obj_corner, scene_corner, H);//error
		
				// cv::line(dstimage, scene_corner[0]+cv::Point2f(static_cast<float>(image.cols),0),
				// 	scene_corner[1]+cv::Point2f(static_cast<float>(image.cols),0),cv::Scalar(-1));
				// cv::line(dstimage, scene_corner[1]+cv::Point2f(static_cast<float>(image.cols),0),
				// 	scene_corner[2]+cv::Point2f(static_cast<float>(image.cols),0),cv::Scalar(-1));
				// cv::line(dstimage, scene_corner[2]+cv::Point2f(static_cast<float>(image.cols),0),
				// 	scene_corner[3]+cv::Point2f(static_cast<float>(image.cols),0),cv::Scalar(-1));
				// cv::line(dstimage, scene_corner[3]+cv::Point2f(static_cast<float>(image.cols),0),
				// 	scene_corner[0]+cv::Point2f(static_cast<float>(image.cols),0),cv::Scalar(-1));
		}
		cv::imshow("match", dstimage);
	}
	return 0;
}