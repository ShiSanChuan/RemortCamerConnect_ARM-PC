#include <iostream>
#include <opencv2/opencv.hpp>

cv::Mat srcImage1,inpaintImage;
cv::Point previousPoint(-1,-1);

static void On_Mouse(int event,int x,int y,int flags,void*){
	if(event==cv::EVENT_LBUTTONUP||!(flags&cv::EVENT_FLAG_LBUTTON))
		previousPoint=cv::Point(-1,-1);
	else if(event==cv::EVENT_LBUTTONDOWN)
		previousPoint=cv::Point(x,y);
	else if(event==cv::EVENT_MOUSEMOVE&&(flags&cv::EVENT_FLAG_LBUTTON)){
		cv::Point pt(x,y);
		if(previousPoint.x<0)previousPoint=pt;
		cv::line(inpaintImage, previousPoint, pt, cv::Scalar(0,0,255),5,8,0);
		cv::line(srcImage1, previousPoint, pt, cv::Scalar(0,0,255),5,8,0);
		previousPoint=pt;
		cv::imshow("origin", srcImage1);
	}
}



int main(int argc, char const *argv[])
{
	cv::Mat srcImage=cv::imread("BingWallpaper-2018-06-13.jpg");
	cv::resize(srcImage, srcImage, cv::Size(800,480));
	srcImage1=srcImage.clone();
	inpaintImage=cv::Mat::zeros(srcImage1.size(),CV_8U);
	cv::imshow("origin", srcImage1);

	cv::setMouseCallback("origin", On_Mouse,0);

	while(1){
		char key=(char)cv::waitKey();
		if(key=='q')break;
		if(key=='2'){
			inpaintImage=cv::Scalar::all(0);
			srcImage.copyTo(srcImage1);
			cv::imshow("origin", srcImage1);
		}
		if(key=='1'||key==' '){
			cv::Mat inpaintedImage;
			cv::inpaint(srcImage1, inpaintImage, inpaintedImage, 3, cv::INPAINT_TELEA);
			cv::imshow("inpaint", inpaintedImage);
		}

	}


	return 0;
}