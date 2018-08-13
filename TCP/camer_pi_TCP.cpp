#include <iostream>
#include <opencv2/opencv.hpp>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/shm.h>
#include <unistd.h>  //close() headfile 
#include <vector>

#define image_size 65500  
#define command    128

int main(int argc, char const *argv[])
{
	if(argc<3){std::cout<<"./camer_pi_TCP addr port"<<std::endl;exit(0);}
	std::string addr(argv[1]);
	int port=atoi(argv[2]);

	cv::Mat frame;
	cv::VideoCapture cap;
	cap.open(0);

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

	std::vector<uchar> data_encode;
	std::vector<int> quality;
	quality.push_back(CV_IMWRITE_JPEG_QUALITY);
	quality.push_back(50);
	while(1){
		cap>>frame;
		cv::resize(frame,frame,cv::Size(640,480));
		cv::imencode(".jpg", frame,data_encode,quality);
		sprintf((char*)command_buffer,"%d\t",(int)data_encode.size());
		send(sock_cli, command_buffer,command,0);
		for(int i=0;i<data_encode.size();i++)
			image_buffer[i]=data_encode[i];
		send(sock_cli,image_buffer,(int)data_encode.size(),0);
		std::cout<<data_encode.size()<<std::endl;
	}
	
	delete []image_buffer;
	delete []command_buffer;
	close(sock_cli);

	return 0;
}