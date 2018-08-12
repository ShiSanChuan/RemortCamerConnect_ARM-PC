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

int main(int argc, char const *argv[])
{
	cv::Mat frame;
	cv::VideoCapture cap;
	cap.open(0);

	unsigned char *image_buffer=new unsigned char[image_size*4];
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

	while(1){
		cap>>frame;
		cv::resize(frame,frame,cv::Size(640,480));
		cv::cvtColor(frame, frame, cv::COLOR_RGB2GRAY);
		unsigned char *_image_buffer=frame.data;
		memcpy(image_buffer, _image_buffer, image_size*4);

		// recv(sock_cli,command_buffer,command,0);//using to command
		for(int i=0;i<4;i++){
	    	send(sock_cli,image_buffer+image_size*i,image_size,0);
			cv::waitKey(6);
		} 
	    
	}
	
	delete []image_buffer;
	delete []command_buffer;
	close(sock_cli);

	return 0;
}