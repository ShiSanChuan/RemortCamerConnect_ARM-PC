#include <iostream>
#include <unistd.h>
#include <thread>
#include <vector>


class mythread
{
private:
	std::vector<std::thread> threads;
public:
	mythread();
	~mythread(){}
	int Add(void (*fun)(void));
	int Delete(); 
};

mythread::mythread(){
	
}
void back(int i){

}
int mythread::Add(void (*fun)(void)){
	threads.push_back(std::thread(fun));
	std::cout<<">>>"<<threads.size()<<std::endl;
	threads[threads.size()-1].detach();
	return 0;
}
int mythread::Delete(){
	return 0;
}

int main(int argc, char const *argv[])
{
	mythread m;
	m.Add([]{std::cout<<"ffff"<<std::endl;});
	m.Add([]{std::cout<<"ffff"<<std::endl;});
	m.Add([]{std::cout<<"ffff"<<std::endl;});
	m.Add([]{std::cout<<"ffff"<<std::endl;});
	sleep(10);
	return 0;
}