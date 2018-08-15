#ifndef _MYTHREAD_H
#define _MYTHREAD_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <vector>
#include <map>

class mythread
{
private:
	enum threadmethod{Detach,Join};
	std::vector<std::thread> threads;
	std::map<std::string,int> Map;
	// inline void rundetect(std::string & para,std::function<void (std::string&)> fun);
public:
	mythread(){std::cout<<std::thread::hardware_concurrency()<<" threads are supported"<<std::endl;}
	~mythread();
	int Add(const std::string name,std::function<void (std::string)>  fun);
	int Add(const std::string name,std::string & para,std::function<void (std::string&)> fun);
	int Add(const std::string name,cv::Mat & image,std::function<void (cv::Mat&)> fun);
	// int Delete(const std::string name); 
	void sleep(uint time);
	void usleep(uint time);
	void run(threadmethod method=Detach);
};

#endif