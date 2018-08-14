#include <iostream>
#include <unistd.h>
#include <thread>
#include <vector>
#include <map>

class mythread
{
private:
	std::vector<std::thread> threads;
	std::map<std::string,int> Map;
public:
	mythread();
	~mythread();
	int Add(const std::string name,void (*fun)(void));
	int Delete(const std::string name); 
};

mythread::mythread(){
	
}
mythread::~mythread(){

}
void back(int i){

}
int mythread::Add(const std::string name,void (*fun)(void)){
	threads.push_back(std::thread(fun));
	// std::cout<<">>>"<<threads.size()<<std::endl;
	Map.insert(std::pair<std::string,int>(name,threads.size()-1));
	threads[threads.size()-1].detach();
	return 0;
}
int mythread::Delete(const std::string name){
	// TerminateThread(threads[1]);
	return 0;
}

// int main(int argc, char const *argv[])
// {
// 	mythread m;
// 	m.Add("1",[]{std::cout<<"ffff"<<std::endl;});
// 	m.Add("2",[]{std::cout<<"ffff"<<std::endl;});
// 	m.Add("3",[]{std::cout<<"ffff"<<std::endl;});
// 	m.Add("4",[]{std::cout<<"ffff"<<std::endl;});
// 	sleep(3);
// 	return 0;
// }