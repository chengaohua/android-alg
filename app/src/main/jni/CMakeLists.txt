project(scrfdncnn)

cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

include_directories(${CMAKE_SOURCE_DIR}/../jniLibs/include)
link_directories(${CMAKE_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})

find_library(log-lib log)
find_library( jnigraphics-lib jnigraphics )

message("------------>${CMAKE_SOURCE_DIR}")

set(OPENCV_LIBS opencv_core opencv_highgui opencv_imgproc opencv_photo opencv_video opencv_features2d)

add_library(scrfdncnn SHARED scrfdncnn.cpp scrfd.cpp ndkcamera.cpp)


target_link_libraries(scrfdncnn ncnn ${OPENCV_LIBS} camera2ndk mediandk  jnigraphics
        z
        ${log-lib}  android)
