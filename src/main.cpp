#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#define DURATION 2
/*
    Camera device config
*/
#define DEVICE "/dev/video0"
#define W 1280
#define H 720
#define PIXEL_FORMAT V4L2_PIX_FMT_MJPEG
#define REQ_BUFFERS 1

void write_to_file(u_int8_t *data, int data_len){
    const char *output_filename = "capture.jpg";
    FILE *fp = fopen(output_filename, "wb");
    if(!fp){
        perror("Error opening file!");
        exit(EXIT_FAILURE);
    }

    fwrite(data, 1, data_len, fp);
    fclose(fp);
}

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
    memset(&format, 0, sizeof(format));
    format.fmt.pix.width = W;
    format.fmt.pix.height = H;
    format.fmt.pix.pixelformat = PIXEL_FORMAT;
    format.fmt.pix.field = V4L2_FIELD_NONE;
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if((ioctl(cam_fd, VIDIOC_S_FMT, &format))==-1){
        perror("Error setting format for camera device!");
        exit(EXIT_FAILURE);
    }

    /* 
        Request kernel space buffers
    */
    struct v4l2_requestbuffers reqbufs;
    memset(&reqbufs, 0, sizeof(reqbufs));
    reqbufs.count = REQ_BUFFERS;
    reqbufs.memory = V4L2_MEMORY_MMAP;
    reqbufs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if((ioctl(cam_fd, VIDIOC_REQBUFS, &reqbufs))==-1){
        perror("Error requesting buffers!");
        exit(EXIT_FAILURE);
    }

    /* 
        Query the buffers and mapping
    */
    struct v4l2_buffer buffer;  // Stores metadata related to buffer

    // Pointer for each buffer (Driver written data)
    void *buffers[REQ_BUFFERS];

    for(int i=0; i<REQ_BUFFERS; i++){

        // Configure v4l2 buffer
        memset(&buffer, 0, sizeof(buffer));
        buffer.index = i;
        buffer.memory = V4L2_MEMORY_MMAP;
        buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        // Query: retrieve buffer metadata for user space mapping
        if((ioctl(cam_fd, VIDIOC_QUERYBUF, &buffer))==-1){
            perror("Error querying buffers!");
            exit(EXIT_FAILURE);
        }

        // Map buffer in kernel space to processes address space
        buffers[i] = mmap(NULL, buffer.length, PROT_READ | PROT_WRITE, MAP_SHARED, cam_fd, buffer.m.offset);
        if(buffers[i] == MAP_FAILED){
            perror("Error mapping memory!");
            exit(EXIT_FAILURE);
        }

        // Queue the buffer: Buffer is ready to be written to
        if((ioctl(cam_fd, VIDIOC_QBUF, &buffer))==-1){
            perror("Error queueing buffer!");
            exit(EXIT_FAILURE);
        }
    }

    /* 
        Start streaming
    */
    enum v4l2_buf_type type;
    memset(&type, 0, sizeof(type));
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if((ioctl(cam_fd, VIDIOC_STREAMON, &type))==-1){
        perror("Error streaming from camera!");
        exit(EXIT_FAILURE);
    }

    /* 
        Capture image : Dequeue -> process -> Queue
    */
    while(1){
        memset(&buffer, 0, sizeof(buffer));
        buffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buffer.memory = V4L2_MEMORY_MMAP;

        // Dequeue the buffer
        if((ioctl(cam_fd, VIDIOC_DQBUF, &buffer))==-1){
            perror("Error dequeing the buffer!");
            exit(EXIT_FAILURE);
        }

        // Process the buffer
        printf("\nCaptured image: %d bytes\n", buffer.length);

        write_to_file((u_int8_t*)buffers[buffer.index], buffer.length);

        // Queue the buffer again
        if((ioctl(cam_fd, VIDIOC_QBUF, &buffer))==-1){
            perror("Error queing buffer!");
            exit(EXIT_FAILURE);
        }

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