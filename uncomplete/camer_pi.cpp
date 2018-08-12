#include <iostream>
#include <opencv2/opencv.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/shm.h>
#include <unistd.h>  //close() headfile 

#define image_size 320*240
#define command    128
const std::string addr="127.0.0.1";
const int port=8080;
cv::Point2f point;
bool addremovept=false; 

static void On_Mouse(int event,int x,int y,int flags,void*){
	if(event==cv::EVENT_LBUTTONDOWN){
		point=cv::Point2f((float)x,(float)y);
		addremovept=true;
	}
}

int main(int argc, char const *argv[])
{
	unsigned char *image_buffer=new unsigned char[image_size];
	unsigned char *command_buffer=new unsigned char[command];
	int sock_cli;
	sock_cli=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in servaddr;
	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(port);
	servaddr.sin_addr.s_addr=inet_addr(addr.data());
	if(connect(sock_cli,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
		perror("connect");
		return false;
	}
	cv::namedWindow("demo",cv::WINDOW_AUTOSIZE);
	cv::setMouseCallback("demo", On_Mouse,0);
	cv::Mat preimage;
	std::vector<cv::Point2f> points[2];
	cv::TermCriteria termcrit(cv::TermCriteria::COUNT|cv::TermCriteria::EPS,20,0.03);
	
	while(char(cv::waitKey(1))!='q'){
		send(sock_cli,command_buffer,command,0);
		recv(sock_cli,image_buffer,image_size,0);
		cv::Mat image=cv::Mat(cv::Size(320,240),CV_8UC1,image_buffer);
		cv::resize(image, image, cv::Size(640,480));
		// cv::imshow("pic", image);
		if(!points[0].empty()){
			std::vector<uchar> status;
			std::vector<float> err;
			if(preimage.empty())image.copyTo(preimage);
			cv::calcOpticalFlowPyrLK(preimage, image, points[0], points[1],status,err,cv::Size(31,31));
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
			cv::cornerSubPix(image, tmp, cv::Size(10,10), cv::Size(-1,-1), termcrit);
			points[1].push_back(tmp[0]);
			addremovept=false;
		}
		cv::imshow("demo", image);
		std::swap(points[1], points[0]);
		cv::swap(preimage, image);
		switch(char(cv::waitKey(1))){
			case 'r':
			points[1].clear();
			points[0].clear();
			break;
		}
	}
	delete []image_buffer;
	delete []command_buffer;
	close(sock_cli);

	return 0;
}
