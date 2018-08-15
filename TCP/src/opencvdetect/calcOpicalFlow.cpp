#include "detect.h"

std::vector<cv::Point2f> Points;
static cv::Mat pregray;
std::vector<cv::Point2f> calcOptialFlow(cv::Mat &gray){
	if(!points[0].empty()){
			std::vector<uchar> status;
			std::vector<float> err;
			if(pregray.empty())gray.copyTo(pregray);
			cv::calcOpticalFlowPyrLK(pregray, gray, points[0], points[1],status,err,cv::Size(31,31));
			int k=0;
			for(int i=0;i<points[1].size();i++){
				if(addremovept&&cv::norm(point-points[1][i])<=5){
					addremovept=false;
					continue;
				}
				if(!status[i])continue;
				points[1][k++]=points[1][i];
				cv::circle(image, points[1][i],3,cv::Scalar(0,255,0));
			}
			points[1].resize(k);
		}
		if(addremovept&&points[1].size()<20){
			std::vector<cv::Point2f> tmp;
			tmp.push_back(point);
			cv::cornerSubPix(gray, tmp, cv::Size(10,10), cv::Size(-1,-1), termcrit);
			points[1].push_back(tmp[0]);
			addremovept=false;
	}
}
