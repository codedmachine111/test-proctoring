#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/image_capture.hpp"

int main(){

    int cam_fd;
    
    /* 
        Open the camera device
    */
    if((cam_fd = open(DEVICE, O_RDWR, 0))==-1){
        perror("Error opening camera!");
        exit(EXIT_FAILURE);
    }

    /* 
        Set format for camera device
    */
    struct v4l2_format format;
    set_camera_format(cam_fd, format);

    /* 
        Request kernel space buffers
    */
    struct v4l2_requestbuffers reqbufs;
    request_buffers(cam_fd, reqbufs);

    /* 
        Query the buffers and mapping
    */
    struct v4l2_buffer buffer;  // Stores metadata related to buffer
    void *buffers[REQ_BUFFERS]; // Pointer for each buffer (Driver written data)
    query_buffers_and_map(cam_fd, buffer, buffers, REQ_BUFFERS);

    /*
        Load face cascade classfier from OpenCV
    */
    cv::CascadeClassifier face_cascade;
    std::string face_cascade_name = "assets/haarcascade_frontalface_alt.xml";
    if(!face_cascade.load(face_cascade_name)){
        perror("Error loading face cascade!");
        exit(EXIT_FAILURE);
    }

    /* 
        Start streaming
    */
    start_streaming(cam_fd);

    /* 
        Capture images continuously
    */
    while(1){
        capture_camera_stream(cam_fd, buffer, buffers, face_cascade);
        sleep(DURATION);
    }

    /*
        Cleanup
    */
    for(int i=0; i<REQ_BUFFERS; i++){
        munmap(buffers[i], buffer.length);
    }
    close(cam_fd);

    return 0;
}