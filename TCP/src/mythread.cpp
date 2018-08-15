#include "mythread.h"
// inline void mythread::rundetect(std::string & para,std::function<void (std::string&)> fun){
// 	while(1)
// 		fun(para);//undo
// }
mythread::~mythread(){
	for(int i=0;i<threads.size();i++)
		if(threads[i].joinable())threads[i].join();
}

int mythread::Add(const std::string name,std::function<void (std::string)> fun){
	threads.push_back(std::thread(fun,name));
	Map.insert(std::pair<std::string,int>(name,threads.size()-1));
	return 0;
}
int mythread::Add(const std::string name,std::string & para,std::function<void (std::string&)> fun){
	threads.push_back(std::thread(fun,std::ref(para)));
	Map.insert(std::pair<std::string,int>(name,threads.size()-1));
	return 0;
}
int mythread::Add(const std::string name,cv::Mat & image,std::function<void (cv::Mat&)> fun){
	threads.push_back(std::thread(fun,std::ref(image)));
	Map.insert(std::pair<std::string,int>(name,threads.size()-1));
	return 0;
}
void mythread::sleep(uint time){
	std::this_thread::sleep_for(std::chrono::seconds(time));
}
void mythread::usleep(uint time){
	std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void mythread::run(threadmethod method){
	for(auto & t: threads)
		if(t.joinable()){
			if(method==Detach)t.detach();
			else t.join();
		}
}
// int main(int argc, char const *argv[])
// {
// 	mythread m;
// 	std::string buff("hello");
// 	std::cout<<std::this_thread::get_id()<<std::endl;
// 	m.Add("1",buff,[](std::string &str){
// 									str+="1";
// 									std::cout<<std::this_thread::get_id()<<"\t"<<str<<std::endl;
// 									std::this_thread::sleep_for(std::chrono::milliseconds(2000));
// 									std::cout<<std::this_thread::get_id()<<"\t"<<str<<std::endl;
// 								});
// 	m.Add("2",buff,[](std::string &str){str+="2";std::cout<<std::this_thread::get_id()<<"\t"<<str<<std::endl;});
// 	m.Add("3",buff,[](std::string &str){str+="3";std::cout<<std::this_thread::get_id()<<"\t"<<str<<std::endl;});
// 	m.Add("4",[](std::string str){str+="4";std::cout<<std::this_thread::get_id()<<"\t"<<str<<std::endl;});
// 	m.run();
// 	sleep(3);
// 	std::cout<<std::this_thread::get_id()<<std::endl;
// 	return 0;
// }