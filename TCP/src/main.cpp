#include <iostream>
#include <thread>
#include <mutex>
#include "mytcp.h"
#include "mythread.h"
#include "detect.h"

std::mutex M;
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
		std::vector<cv::Point2f> calaFlow;
		cv::Point2f detect_point(.0,.0);
		// mythread T;
		// T.Add("camer",image,[](cv::Mat &image){
		// 	while(1){
		// 		M.lock();
		// 		int size=server.recvimage(image);
		// 		M.unlock();
		// 	}
		// });
		// T.Add("imagtack", image, [](cv::Mat &image){
		// 	while(1){
		// 		M.lock();
		// 		cv::cvtColor(image, gray,cv::COLOR_RGB2GRAY);
		// 		M.unlock();
		// 	}
		// });
		// T.Add("findcircle", gray, [](cv::Mat &gray){
		// 	if(gray.size())
		// 		Circle=findCirclse(gray);
		// });

		while(char(cv::waitKey(1)!='q')){
			int size=server.recvimage(image);
			cv::cvtColor(image, gray,cv::COLOR_RGB2GRAY);
			Circle=findCirclse(gray);
			if(Circle[0]!=-1){
				cv::circle( image, cv::Point(Circle[0],Circle[1]), 3, cv::Scalar(0,255,0), -1, 8, 0 );
				cv::circle( image, cv::Point(Circle[0],Circle[1]), Circle[2], cv::Scalar(0,0,255), 3, 8, 0 );
			}
			if(detect_point.x==.0&&detect_point.y==.0){//计算多次出现的圆形，建立一个跟踪点
				calaFlow.push_back(cv::Point2f((float)Circle[0],(float)Circle[1]));
				if(calaFlow.size()>3){
					cv::Point2f mean(.0,.0);
					for(int i=0;i<calaFlow.size();i++){
						mean.x+=calaFlow[i].x;
						mean.y+=calaFlow[i].y;
					}
					mean.x=mean.x/calaFlow.size();
					mean.y=mean.y/calaFlow.size();
					cv::Point2f cov(.0,.0);
					for(int i=0;i<calaFlow.size();i++){
						float x=calaFlow[i].x-mean.x;
						float y=calaFlow[i].y-mean.y;
						cov.x+=x*x;
						cov.y+=y*y;
					}
					cov.x=cov.x/calaFlow.size();
					cov.y=cov.y/calaFlow.size();
					// std::cout<<"x: "<<mean.x<<"\ty: "<<mean.y<<std::endl;
					// std::cout<<"cov: "<<cov.x<<"\t"<<cov.y<<std::endl;
					if(cov.x<5&&cov.y<5){
						detect_point.x=mean.x;
						detect_point.y=mean.y;
						calcOptialFlow(gray,detect_point);
					}
					calaFlow.clear();
				}
			}else {
				calcOptialFlow(gray,detect_point);
				std::cout<<"X: "<<detect_point.x<<"\tY: "<<detect_point.y<<std::endl;
				cv::circle(image, detect_point, 3, cv::Scalar(0,255,0));
			
			}
			cv::imshow("pic", image);
		}
			cv::waitKey(0);
		return 0;
	}

    return 0;
}
