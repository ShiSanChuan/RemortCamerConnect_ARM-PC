#include "detect.h"

static std::vector<cv::Point2f> points[2];
static cv::Mat pregray;
void calcOptialFlow(cv::Mat gray,cv::Point2f & point){
	if(!points[0].empty()){
			std::vector<uchar> status;
			std::vector<float> err;
			if(pregray.empty())gray.copyTo(pregray);
			cv::calcOpticalFlowPyrLK(pregray, gray, points[0], points[1],status,err,cv::Size(31,31));
			int k=0;
			for(int i=0;i<points[1].size();i++){
				if(!status[i])continue;
				points[1][k++]=points[1][i];
			}
			points[1].resize(k);
			point=points[1][0];

	}else{
		points[1].push_back(point);
	}
	// if(addremovept&&points[1].size()<20){
	// 	std::vector<cv::Point2f> tmp;
	// 	tmp.push_back(point);
	// 	cv::cornerSubPix(gray, tmp, cv::Size(10,10), cv::Size(-1,-1), termcrit);
	// 	points[1].push_back(tmp[0]);
	// 	addremovept=false;
	// }
	std::swap(points[1], points[0]);
	gray.copyTo(pregray);
}
