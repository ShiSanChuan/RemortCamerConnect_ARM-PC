#include <iostream>
#include <thread>
#include <mutex>
#include "mytcp.h"
#include "mythread.h"
#include "detect.h"
//camer -s port
//camer -c addr port
using std::string;
int main(int argc, const char** argv)
{
	if(argc<3){std::cout<<"camer [option] {addr port} {port}:\t\ncamer -s port\t\ncamer -c addr port\t\n";}
	if(string(argv[1])=="-c"){
		cv::VideoCapture cap;
		cap.open(0);
		cv::Mat frame;
		// string command("oolea");
		string addr(argv[2]);
		int port=atoi(argv[3]);
		mytcp client(addr,port);
		client.connectserver();
		while(1){
			cap>>frame;
			int size=client.sendimage(frame);
		}
		return 0;
	}

	if(string(argv[1])=="-s"){
		cv::Mat image,gray;
		cv::Vec3f Circle;
		int port=atoi(argv[2]);
		mytcp server(port);
		server.newclient();
		// string command("hello");

		while(char(cv::waitKey(1)!='q')){
			int size=server.recvimage(image);
			cv::cvtColor(image, gray,cv::COLOR_RGB2GRAY);
			Circle=findCirclse(gray);
			cv::circle( image, cv::Point(Circle[0],Circle[1]), 3, cv::Scalar(0,255,0), -1, 8, 0 );
			cv::circle( image, cv::Point(Circle[0],Circle[1]), Circle[2], cv::Scalar(0,0,255), 3, 8, 0 );
			cv::imshow("pic", image);
		}
			cv::waitKey(0);
		return 0;
	}

    return 0;
}
