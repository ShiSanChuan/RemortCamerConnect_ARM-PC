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
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	uchar * image_buffer;
	uchar * command_buffer;
	socklen_t sin_size;
public:
	mytcp(const std::string &_addr,const int & _port);
	mytcp(const int & _port);
	int recvcommand(std::string & buffer);
	int recvbuffer(std::string & buffer);
	int recvimage(cv::Mat & image);
	int sendcommand(const std::string & buffer);
	int sendbuffer(const std::string & buffer);
	int sendimage(const cv::Mat & image);
	bool newclient(void);
	bool connectserver(void);
	~mytcp(){
		delete []image_buffer;
		delete []command_buffer;
		if(sockfd>0)close(sockfd);
		if(new_fd>0)close(new_fd);
	}
};