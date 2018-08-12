#include <iostream>
#include <opencv2/opencv.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/shm.h>
#include <unistd.h>  //close() headfile 
#include <netdb.h>
#include <sys/types.h>

#define portnumber 8080
#define image_size 320*240
#define command    128

int main(int argc, char const *argv[])
{
	cv::Mat frame;
	cv::VideoCapture cap;
	cap.open(0);

	int sockfd,len=0;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	unsigned char *image_buffer=new unsigned char[image_size];
	unsigned char *command_buffer=new unsigned char[command];

	if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1){
		std::cout<<"Socket error!"<<std::endl;
		exit(1);
	}else{std::cout<<"Socket is ok!"<<std::endl;}
	bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(portnumber);
    len=sizeof(server_addr);
    if(bind(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr))==-1)
    {
        std::cout<<"Bind erroe!"<<std::endl;
        exit(1);
    }else{std::cout<<"Bind is ok!"<<std::endl;}
    int _recv=recvfrom(sockfd,command_buffer,command,0,(struct sockaddr*)&client_addr,(socklen_t*)&len);

    while(1){
    	// if(_recv<0){std::cout<<"recv error!"<<std::endl;exit(1);}
    	// std::cout<<inet_ntoa(client_addr.sin_addr)<<std::endl;
    	cap>>frame;
    	cv::resize(frame,frame,cv::Size(320,240));
    	cv::cvtColor(frame,frame,cv::COLOR_RGB2GRAY);
    	unsigned char *_image_buffer=frame.data;
		memcpy(image_buffer, _image_buffer, image_size);
    	int _send=sendto(sockfd,image_buffer,65508,0,(struct sockaddr *)&client_addr,len);
    	if(_send<0){std::cout<<"send error!"<<std::endl;exit(1);}
    }

 //    sin_size=sizeof(struct sockaddr_in);
	// if((new_fd=accept(sockfd,(struct sockaddr *)(&client_addr),&sin_size))==-1)
	// {
	//  	std::cout<<"Accept error!"<<std::endl;
	//  	exit(1);
	// }else{std::cout<<"new client!"<<std::endl;}
	// inet_ntoa(client_addr.sin_addr);
	// while(1){
	// 	// iDataNum=recv(new_fd,image_buffer,sizeof(image_buffer),0);
	//  //    if(iDataNum<0){std::cout<<"Recv error!"<<std::endl;}
	// 	cap>>frame;
	// 	cv::cvtColor(frame, frame, cv::COLOR_RGB2GRAY);
	// 	unsigned char *_image_buffer=frame.data;
	// 	memcpy(image_buffer, _image_buffer, image_size*sizeof(unsigned char));
	//     send(new_fd,image_buffer,image_size*sizeof(unsigned char),0);
	//     // cv::Mat img(cv::Size(640,480),CV_8UC1,image_buffer);
	//     // cv::imwrite("pic.jpg", img);
	// }

	close(sockfd);
	delete []image_buffer;
	delete []command_buffer;
	return 0;
}