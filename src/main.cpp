#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>

#include "../include/image_capture.hpp"
#include "../include/quiz.hpp"

const char *QUIZ_FILENAME = "assets/test.txt";
const char *ANS_FILENAME = "answers.txt";
const int NUM_QUESTIONS = 7;

struct cap_thread_args{
    int camfd;
    struct v4l2_buffer buffer;
    void **buffers;
    cv::CascadeClassifier classifier;
};

struct cap_thread_args *args;

void *capture_thread_routine(void *args){
    struct cap_thread_args *th_args;
    th_args = (struct cap_thread_args*)args;

    while(1){
        /* 
            Capture images continuously
        */
        capture_camera_stream(th_args->camfd, th_args->buffer, th_args->buffers, th_args->classifier);
        sleep(DURATION);
    }
}

int main(){

    int cam_fd;
    pthread_t tid;

    char answers_buffer[1024];

    args = (struct cap_thread_args*)malloc(sizeof(struct cap_thread_args));

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
        Display quiz questions
    */
    display_quiz(QUIZ_FILENAME, NUM_QUESTIONS);

    /* 
        Start streaming
    */
    start_streaming(cam_fd);

    args->camfd = cam_fd;
    args->buffer = buffer;
    args->buffers = buffers;
    args->classifier = face_cascade;
    pthread_create(&tid, NULL, capture_thread_routine, (void *)args);

    // Main thread loop
    while(1){
        if(fgets(answers_buffer, sizeof(answers_buffer), stdin)==NULL){
            perror("Error reading input!");
            exit(EXIT_FAILURE);
        }
        sleep(1);
        break;
    }

    /*
        Write answers to file
    */
    write_to_file((uint8_t *)answers_buffer, strlen(answers_buffer), ANS_FILENAME);

    /*
        Cleanup
    */
    for(int i=0; i<REQ_BUFFERS; i++){
        munmap(buffers[i], buffer.length);
    }
    close(cam_fd);

    return 0;
}