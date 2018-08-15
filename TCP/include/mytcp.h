#ifndef _MYTCP_H
#define _MYTCP_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/shm.h>
#include <unistd.h>  //close() headfile 
#include <netdb.h>
#include <sys/types.h>
#include <string>
#include <opencv2/opencv.hpp>

class mytcp
{
private:
	std::string addr;
	int port;
	int sockfd,new_fd;
	int image_size;
	int command;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	uchar * image_buffer;
	uchar * command_buffer;
	socklen_t sin_size;
public:
	mytcp(const std::string &_addr,const int & _port,int _command=128,int _image_size=65500);
	mytcp(const int & _port,int _command=128,int _image_size=65500);
	int recvcommand(std::string & buffer){return recvcommand(buffer,command);}
	int recvcommand(std::string & buffer,int _size);
	int recvbuffer(std::string & buffer){return recvbuffer(buffer,image_size);}
	int recvbuffer(std::string & buffer,int _size);
	int recvimage(cv::Mat & image){return recvimage(image,image_size);}
	int recvimage(cv::Mat & image,int _size);
	int sendcommand(const std::string & buffer){return sendcommand(buffer,command);}
	int sendcommand(const std::string & buffer,int _size);
	int sendbuffer(const std::string & buffer){return sendbuffer(buffer,image_size);}
	int sendbuffer(const std::string & buffer,int _size);
	int sendimage(const cv::Mat & image){return sendimage(image,image_size);}
	int sendimage(const cv::Mat & image,int _size);
	int GetCommendSize(void){return command;}
	int GetImageSize(void){return image_size;}
	bool newclient(void);
	bool connectserver(void);
	~mytcp(){
		delete []image_buffer;
		delete []command_buffer;
		if(sockfd>0)close(sockfd);
		if(new_fd>0)close(new_fd);
	}
};

#endif