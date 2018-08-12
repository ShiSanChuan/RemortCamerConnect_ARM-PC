#include <iostream>
#include <opencv2/opencv.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/shm.h>
#include <unistd.h>  //close() headfile 
#include <netdb.h>
#include <sys/types.h>

// #define portnumber 8080
#define image_size 320*240 //pc can used is 320*240*4=640*480
#define command    128

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
	if(argc<1){std::cout<<"./camer_pc_TCP port";exit(0);}
	int portnumber=atoi(argv[1]);

	int sockfd,new_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t sin_size;
	unsigned char *image_buffer=new unsigned char[image_size*16];
	unsigned char *command_buffer=new unsigned char[command];

	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
		std::cout<<"Socket error!"<<std::endl;
		exit(1);
	}else{std::cout<<"Socket is ok!"<<std::endl;}
	bzero(&server_addr,sizeof(struct sockaddr_in));
	bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(portnumber);
    if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
    {
        std::cout<<"Bind erroe!"<<std::endl;
        exit(1);
    }else{std::cout<<"Bind is ok!"<<std::endl;}
    if(listen(sockfd,2)==-1)
    {
        std::cout<<"Listen error!"<<std::endl;
        exit(1);
    }else {std::cout<<"Listen is ok!"<<std::endl;}
    sin_size=sizeof(struct sockaddr_in);
	if((new_fd=accept(sockfd,(struct sockaddr *)(&client_addr),&sin_size))==-1)
	{
	 	std::cout<<"Accept error!"<<std::endl;
	 	exit(1);
	}else{std::cout<<"new client!"<<std::endl;}
	inet_ntoa(client_addr.sin_addr);

	cv::namedWindow("demo",cv::WINDOW_AUTOSIZE);
	cv::setMouseCallback("demo", On_Mouse,0);
	cv::Mat preimage;
	std::vector<cv::Point2f> points[2];
	cv::TermCriteria termcrit(cv::TermCriteria::COUNT|cv::TermCriteria::EPS,20,0.03);
	
	while(char(cv::waitKey(1))!='q'){
		// send(new_fd,command_buffer,command,0);
		for(int i=0;i<4;i++){
			recv(new_fd,image_buffer+image_size*i,image_size,0);
			// cv::waitKey(6);
		}

		cv::Mat image=cv::Mat(cv::Size(640,480),CV_8UC1,image_buffer);
		// cv::resize(image, image, cv::Size(640,480));
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

	close(new_fd);
	close(sockfd);
	delete []image_buffer;
	delete []command_buffer;
	return 0;
}