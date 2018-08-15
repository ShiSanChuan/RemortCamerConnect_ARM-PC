#ifndef _DETECT_H
#define _DETECT_H

#include <opencv2/opencv.hpp>
#include <iostream>

cv::Vec3f findCirclse(cv::Mat &gray);
void calcOptialFlow(cv::Mat gray,cv::Point2f & point);




#endif