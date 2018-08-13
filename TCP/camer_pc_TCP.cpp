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
#define image_size 65500 //pc can used is 320*240*4=640*480
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
	if(argc<2){std::cout<<"Usag: ./camer_pc_TCP port "<<std::endl;exit(0);}
	int portnumber=atoi(argv[1]);

	int sockfd,new_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t sin_size;
	unsigned char *image_buffer=new unsigned char[image_size*4];
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
	cv::Mat pregray,gray,grayblur;
	std::vector<cv::Point2f> points[2];
	cv::TermCriteria termcrit(cv::TermCriteria::COUNT|cv::TermCriteria::EPS,20,0.03);

	std::vector<uchar> data_decode;
	int size=0;
	bool flag=true;
	while(flag){
		//recv imag
		data_decode.clear();
		recv(new_fd,command_buffer,command,0);
		size=atoi((char*)command_buffer);
		int recv_size=recv(new_fd, image_buffer,size,0);
		// std::cout<<recv_size<<"\t"<<size<<std::endl;
		if(recv_size!=size||recv_size<10000)continue;

		for(int i=0;i<recv_size;i++)
			data_decode.push_back(image_buffer[i]);
		cv::Mat image=cv::imdecode(data_decode,CV_LOAD_IMAGE_COLOR);
		// use calcOptialFlowPyrLK
		cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);
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
		//find circles
		std::vector<cv::Vec3f> circles;
		cv::blur(gray, grayblur, cv::Size(3,3));
		cv::HoughCircles(grayblur, circles, CV_HOUGH_GRADIENT,2, grayblur.rows/4,CV_HOUGH_GRADIENT ,CV_HOUGH_GRADIENT,10,50);
		if(circles.size()){
			if(points[0].empty()){
				points[1].push_back(cv::Point2f(float(circles[0][0]),float(circles[0][1])));
			}else{
				
			}
			cv::Point center(cvRound(circles[0][0]), cvRound(circles[0][1]));
	        int radius = cvRound(circles[0][2]);
	        cv::circle( image, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
	        cv::circle( image, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
		}
		cv::imshow("demo", image);
		std::swap(points[1], points[0]);
		cv::swap(pregray, gray);
		switch(char(cv::waitKey(1))){
			case 'r':
			points[1].clear();
			points[0].clear();
			break;
			case 'q':
			flag=false;
			break;
		}
	}

	close(new_fd);
	close(sockfd);
	delete []image_buffer;
	delete []command_buffer;
	return 0;
}