#ifndef IMAGE_CAPTURE_HPP
#define IMAGE_CAPTURE_HPP

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/objdetect.hpp>
#include <opencv4/opencv2/imgproc.hpp>

#define DURATION 2
/*
    Camera device config
*/
#define DEVICE "/dev/video0"
#define W 1280
#define H 720
#define PIXEL_FORMAT V4L2_PIX_FMT_MJPEG
#define REQ_BUFFERS 1

void write_to_file(u_int8_t *data, int data_len);
void set_camera_format(int cam_fd, struct v4l2_format format);
void request_buffers(int cam_fd, struct v4l2_requestbuffers reqbufs);
void query_buffers_and_map(int cam_fd, struct v4l2_buffer buffer, void **buffers, int total_buffers);
void start_streaming(int cam_fd);
void capture_camera_stream(int cam_fd, struct v4l2_buffer buffer, void **buffers, cv::CascadeClassifier cv);


#endif