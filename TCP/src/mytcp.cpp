#include "mytcp.h"

static const int image_size=65500;
static const int command=128;

mytcp::mytcp(const std::string &_addr,const int & _port){
	addr=_addr;
	port=_port;
	new_fd=-1;
	image_buffer=new unsigned char[image_size];
	command_buffer=new unsigned char[command];
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(port);
	server_addr.sin_addr.s_addr=inet_addr(addr.data());
}
mytcp::mytcp(const int & _port){
	port=_port;
	new_fd=-1;
	image_buffer=new unsigned char[image_size];
	command_buffer=new unsigned char[command];

	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1){
		std::cout<<"Socket error!"<<std::endl;
		exit(1);
	}else{std::cout<<"Socket is ok!"<<std::endl;}
	bzero(&server_addr,sizeof(struct sockaddr_in));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(port);
}
bool mytcp::connectserver(void){
	if(connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0){
		perror("connect");
		return false;
	}
	return true;
}
bool mytcp::newclient(void){
	if(addr.size()!=0){std::cout<<"newclient is used by server!"<<std::endl;exit(1);}
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
	return true;
}
int mytcp::recvcommand(std::string & buffer){
	int fd;
	if(new_fd==-1)fd=sockfd;// client recv
	else fd=new_fd;
	memset(command_buffer,0x0,command);
	int size=recv(fd,command_buffer,command,0);
	buffer=std::string((char*)command_buffer);
	return size;
}
int mytcp::recvbuffer(std::string & buffer){
	int fd;
	if(new_fd==-1)fd=sockfd;//client recv
	else fd=new_fd;
	int size=recv(fd, image_buffer,image_size,0);
	buffer=std::string((char*)image_buffer);
	return size;
}
int mytcp::recvimage(cv::Mat & image){
	if(new_fd==-1)return new_fd;//only used in server
	std::vector<uchar> data_decode;
	int size=recv(new_fd, image_buffer,image_size,0);
	if(size<10000)return -1;
	for(int i=0;i<size;i++)
			data_decode.push_back(image_buffer[i]);
	image=cv::imdecode(data_decode,CV_LOAD_IMAGE_COLOR);
	return size;
}
int mytcp::sendcommand(const std::string & buffer){
	int fd;
	if(new_fd!=-1)fd=new_fd;
	else fd=sockfd;
	// memcpy(command_buffer,buffer.data(),buffer.size()*sizeof(uchar));
	// command_buffer[buffer.size()]=0x0;
	int size=send(fd, buffer.data(),buffer.size(),0);
	return size;
}
int mytcp::sendbuffer(const std::string & buffer){
	int fd;
	if(new_fd!=-1)fd=new_fd;
	else fd=new_fd;
	int size=send(fd, buffer.data(),buffer.size(),0);
	return size;
}
int mytcp::sendimage(const cv::Mat &image){
	if(new_fd!=-1)return -1;//only used in client
	std::vector<int> quality;
	quality.push_back(CV_IMWRITE_JPEG_QUALITY);
	quality.push_back(50);
	std::vector<uchar> data_encode;
	cv::imencode(".jpg", image,data_encode,quality);
	int i=0;
	for(i=0;i<data_encode.size();i++)
			image_buffer[i]=data_encode[i];
	int size=send(sockfd,image_buffer,i,0);
	return size;
}

// int main(int argc, const char** argv)
// {
//     mytcp server(8080);
//     mytcp client("127.0.0.1",8080);
//     return 0;
// }
