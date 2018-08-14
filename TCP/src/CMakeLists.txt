FIND_PACKAGE( OpenCV REQUIRED ) 
FIND_PACKAGE ( Threads REQUIRED )

aux_source_directory(. DIR_SRCS)
add_executable(camer ${DIR_SRCS})

TARGET_LINK_LIBRARIES(camer ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})