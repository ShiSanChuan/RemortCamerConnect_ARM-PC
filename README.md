# RemortCamerConnect_ARM-PC
this is a project about how to transmit picture from ARM's camer to Ubuntu'PC, and take opencv's tool detect picture.some obstacle will be recode and give a way to fix.


#bulid 
on different platform can use this command,file_name and outname is easy to understand,"pkg-config --cflags --libs opencv" is used to link libopencv
```shell
g++ -std=c++11 {file_name} -o {outname} `pkg-config --cflags --libs opencv`
```

#How to use
you must first run `**pc_**.cpp`,and then run `**pi_**.cpp`



#some obstacle i have get
## - TCP&UDP send/recv or sendto/recvfrom buffer problem
