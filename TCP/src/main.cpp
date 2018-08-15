#include <iostream>
#include <thread>
#include <mutex>
#include "mytcp.h"
#include "mythread.h"
//camer -s port
//camer -c addr port
using std::string;
int main(int argc, const char** argv)
{
	if(argc<3){std::cout<<"camer [option] {addr port} {port}:\t\ncamer -s port\t\ncamer -c addr port\t\n";}

	if(string(argv[1])=="-s"){
		cv::Mat image;
		int port=atoi(argv[2]);
		mytcp server(port);
		server.newclient();
		string command("hello");
		int i=0;
		while(((char)cv::waitKey(1))!='q'){
			int size=server.sendcommand(command);
			server.recvimage(image);
			cv::imshow("pic", image);
		}
		return 0;
	}
	if(string(argv[1])=="-c"){
		cv::VideoCapture cap;
		cap.open(0);
		cv::Mat frame;
		string command;
		string addr(argv[2]);
		int port=atoi(argv[3]);
		mytcp client(addr,port);
		client.connectserver();
		while(1){
			cap>>frame;
			int size=client.recvcommand(command);
			std::cout<<command<<std::endl;
			client.sendimage(frame);
		}
		return 0;
	}
    return 0;
}
