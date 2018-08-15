# RemortCamerConnect_ARM-PC
this is a project about how to transmit picture from ARM's camer to Ubuntu'PC, and take opencv's tool detect picture.some obstacle will be recode and give a way to fix.

<!-- <img src=""> -->


# bulid 
on different platform can use this command,file_name and outname is easy to understand,"pkg-config --cflags --libs opencv" is used to link libopencv
~~~
```shell
g++ -std=c++11 {file_name} -o {outname} `pkg-config --cflags --libs opencv`
```
~~~

```shell
cd RemortCamerConnect_ARM-PC/TCP/
cmake .
cd build/src
./camer -s 8080
```

# How to use
~~you must first run `**pc_**.cpp`,and then run `**pi_**.cpp`~~
- Server : ./camer -s 8080
- Client : ./camer -c 127.0.0.1 8080

# Some obstacle i have meet
- TCP&UDP send/recv or sendto/recvfrom buffer problem
	TCP&UDP buffer have 2^16 byte size(about 65536), so send a picture(640x480)is out this rang,so we have two way to solve this problem,using circulatory transmission or set buffer in linux kernel enviorment:
	kernel: `/proc/sys/net/core/rmem_default`,`/proc/sys/net/core/wmem_default`,`/proc/sys/net/core/rmem_max`,`/proc/sys/net/core/wmem_max`;
	recv buffer: `cat /proc/sys/net/ipv4/tcp_rmem`,three parameter is min,default,max(bit);
	send buffer: `cat /proc/sys/net/ipv4/tcp_wmem`,three parameter is min,dafault,max(bit);
	so you can adjust mem:
	```shell
	sysctl -w net.core.rmem_max= 8388608   <-size you want set
	...		<- wmem_max,wmem_min,wmem_default etc.
	sysctl -w net.ipv4.tcp_mem='8388608 8388608 8388608'  <-min,default,max
	```
	you can get network flow by this command:
	```shell
	ifconfig
	sudo tcpdump -ni eth0
	```

- How to transmit opencv's Mat into *unsigned char?
	when i get a image with Mat's format `cv::Mat image=imread("pic.jpg",0)`,i want to translate Mat's format into *unsigned char:
	```cpp
	unsigned char* image_buffer=image.data;
	memcpy(buffer,image_buffer,image_size);
	```
	translate *unsigned char into Mat's format:
	```cpp
	cv::Mat image=cv::Mat(cv::Size(wide,high),CV_8UC1,buffer);
	cv::imshow("pic",image);
	```
	it seem have a better choise in translate picture by use imencode and imdecode
	```
	//imag encode
	std::vector<int> quality;
	quality.push_back(CV_IMWRITE_JPEG_QUALITY);
	quality.push_back(50);
	cv::imencode(".jpg",Mat imag,std::vector<uchar>& send_buffer,quality);
	//imag decode
	cv:: Mat image=cv::imdecode(vector<uchar> & recv_buffer,CV_LOAD_IMAGE_COLOR);
	```

- How to use thread liberate in C++?
	```cpp
	void func(void*);
	thread threads;
	threads=thread(func,void*);
	threads.detach();//unblock or block:threads.join();
	...
	```
	if we want to creat a class to manage thread easily:
	```cpp
	int mythread::Add(void (*fun)(void)){
	threads.push_back(std::thread(fun));
	std::cout<<">>>"<<threads.size()<<std::endl;
	threads[threads.size()-1].detach();
	return 0;
	}
	...
	mythread m;
	m.Add([]{std::cout<<"ffff"<<std::endl;});
	```
	mutilthread have a comment parameter by used std::ref():
	```
	std::thread(fun,std::ref(str));
	```
	if we have better choise to use thread ,we can repackage thread lib into a easy used class.
- CMakeLists.txt enable opencvlib and threadlib
	```make
	FIND_PACKAGE( OpenCV REQUIRED ) 
	FIND_PACKAGE ( Threads REQUIRED )
	TARGET_LINK_LIBRARIES(camer ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
	```