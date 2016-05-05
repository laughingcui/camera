#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <asm/types.h>
#include <linux/videodev2.h>

#include "VideoDevice.h"
#include "Common.h"

/**************************************************
    Function:   VideoDevice
    Description:    用于初始化摄像头的一些特性
    Input:
            devName:设备名称
    Output:
            无
    Return:
            无
    Others:
            无
********************************************************/
VideoDevice::VideoDevice(QString devName) {

    this->devName = devName;
    this->capFD = -1;
    this->buffers = NULL;
    this->nBuffers = 0;
    this->index = -1;

    //打开摄像头
    if (openDevice() == FALSE) {
        closeDevice();
        LOG_PRINT("openDevice failed!\n");
        goto error;
    }
    LOG_PRINT("openDevice success!\n");

    //初始化摄像头
    if (initDevice() == FALSE) {
        closeDevice();
        LOG_PRINT("initDevice failed!\n");
        goto error;
    }
    LOG_PRINT("initDevice success!\n");

    //开始摄像
    if (startCap() == FALSE) {
        //停止摄像
        stopCap();
        //关闭摄像头
        closeDevice();
        LOG_PRINT("startCap failed!\n");
        goto error;
    }
    LOG_PRINT("startCap success!\n");

    return ;

error:
    exit(-1);
    return ;
}
/**************************************************
    Function:  ~VideoDevice
    Description:   用于删除摄像头设备的一些配置
    Input:
            无
    Output:
            无
    Return:
            无
    Others:
            无
********************************************************/

VideoDevice::~VideoDevice(void) {

    if (stopCap() == FALSE){
        LOG_PRINT("stopCap failed!\n");
    }
    LOG_PRINT("stopCap success!\n");

    if (unInitDevice() == FALSE) {
         LOG_PRINT("unInitDevice failed!\n");
    }
    LOG_PRINT("unInitDevice success!\n");

    if (closeDevice() == FALSE){
         LOG_PRINT("closeDevice failed!\n");
    }
    LOG_PRINT("closeDevice success!\n");
}

/**************************************************
    Function:  openDevice
    Description:   打开摄像头
    Input:
            无
    Output:
            无
    Return:
            当capFD不为-1时，返回TRUE;
            如果为-1时，返回FALSE;
    Others:
            无
********************************************************/

int VideoDevice::openDevice(void) {

    capFD = open(FILE_VIDEO,O_RDWR);

    if (-1 == capFD) {
        LOG_PRINT("Error opening V4L interface\n");
        return FALSE;
    }
    return TRUE;
}

/**************************************************
    Function:  closeDevice
    Description:   关闭摄像头
    Input:
            无
    Output:
            无
    Return:
            当close的返回值不为-1时，返回TRUE;
            如果为-1时，返回FALSE;
    Others:
            无
********************************************************/

int VideoDevice::closeDevice(void) {

    if ( close(capFD) == FALSE) {
        LOG_PRINT("Error closing V4L interface\n");
        return FALSE;
    }
    return TRUE;
}

/**************************************************
    Function:  initDevice
    Description:   初始化摄像头
    Input:
            无
    Output:
            无
    Return:
            如果初始化成功，返回TRUE
    Others:
            无
********************************************************/

int VideoDevice::initDevice(void) {

    v4l2_capability cap;//摄像头基本参数
    v4l2_format fmt;//摄像头格式
    v4l2_streamparm setfps;//摄像头采集数据流

    //获取摄像头的基本信息
    if(ioctl(capFD, VIDIOC_QUERYCAP, &cap) == -1) {

       // LOG_PRINT("Error opening device %s: unable to query device.\n", FILE_VIDEO);
        LOG_PRINT("Error opening device %s\n", FILE_VIDEO);
        return FALSE;
    }
    else {

        LOG_PRINT("driver:\t\t%s\n",cap.driver);
        LOG_PRINT("card:\t\t%s\n",cap.card);
        LOG_PRINT("bus_info:\t%s\n",cap.bus_info);
        LOG_PRINT("version:\t%d\n",cap.version);
        LOG_PRINT("capabili ties:\t%x\n",cap.capabilities);

        if ((cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == V4L2_CAP_VIDEO_CAPTURE) {
            LOG_PRINT("Device %s: supports capture.\n",FILE_VIDEO);
        }

        if ((cap.capabilities & V4L2_CAP_STREAMING) == V4L2_CAP_STREAMING) {
            LOG_PRINT("Device %s: supports streaming.\n",FILE_VIDEO);
        }
    }

    //设置 fmt
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    //设置获取图像的格式，这个罗技摄像头支持YUYV格式的，所以采用这种格式
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    //获取图像的高
    fmt.fmt.pix.height = IMG_HEIGHT;
    //获取图像的宽
    fmt.fmt.pix.width = IMG_WIDTH;
    //设置字段为V4L2_FIELD_INTERLACED
    fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
    //设置VIDIOC_S_FMT,将上面的参数传递给内核空间
    if(ioctl(capFD, VIDIOC_S_FMT, &fmt) == -1) {
            LOG_PRINT("Unable to set format\n");
            return FALSE;
    }
    //VIDIOC_G_FMT，获取摄像头格式信息
    if(ioctl(capFD, VIDIOC_G_FMT, &fmt) == -1) {
            LOG_PRINT("Unable to get format\n");
            return FALSE;
    }

    //set fps
    setfps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    setfps.parm.capture.timeperframe.numerator = 1;
    setfps.parm.capture.timeperframe.denominator = 10;
    // VIDIOC_S_PARM，设置摄像头的编号
    if(ioctl(capFD, VIDIOC_S_PARM, &setfps) == -1) {
        LOG_PRINT("Unable to set frame rate\n");
        return FALSE;
    }
    else {
        LOG_PRINT("set fps OK!\n");
    }

    if(ioctl(capFD, VIDIOC_G_PARM, &setfps) == -1) {

        LOG_PRINT("Unable to get frame rate\n");
        return FALSE;
    }
    else {

        LOG_PRINT("get fps OK!\n");
        LOG_PRINT("timeperframe.numerator:\t%d\n",setfps.parm.capture.timeperframe.numerator);
        LOG_PRINT("timeperframe.denominator:\t%d\n",setfps.parm.capture.timeperframe.denominator);

    }

    //mmap,初始化内存映射mmap
    if(initMmap() == FALSE ) {

        LOG_PRINT("cannot mmap!\n");
        return FALSE;

    }

    return TRUE;
}

/**************************************************
    Function:  initMmap
    Description:   初始化摄像头采集图像的存储空间
    Input:
            无
    Output:
            无
    Return:
            如果初始化成功，返回TRUE
    Others:
            无
********************************************************/
int VideoDevice::initMmap(void) {

    v4l2_requestbuffers req;

    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;

    //为摄像头请求内存映射，图像缓冲队列中设置图像为4帧
    if (ioctl(capFD, VIDIOC_REQBUFS, &req) == -1) {
        return FALSE;
    }

    if (req.count < 2) {
        return FALSE;
    }

    buffers = (Buffer*)calloc(req.count, sizeof(*buffers));

    if(!buffers) {

        return FALSE;
    }

    //为buffers数组元素的成员的start分配内存映射
    for (nBuffers = 0; nBuffers < req.count; ++nBuffers) {

        v4l2_buffer buf;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = nBuffers;

        if (ioctl(capFD, VIDIOC_QUERYBUF, &buf) == -1) {

            return FALSE;
        }

        buffers[nBuffers].length = buf.length;
        buffers[nBuffers].start =mmap(NULL, // start anywhere// allocate RAM*4
                                      buf.length,
                                      PROT_READ | PROT_WRITE,
                                      MAP_SHARED,
                                      capFD, buf.m.offset);

        if (MAP_FAILED == buffers[nBuffers].start) {

            return FALSE;
        }
    }
    return TRUE;

}

/**************************************************
    Function:  startCap
    Description:   开始采集图像
    Input:
            无
    Output:
            无
    Return:
            如果初始化成功，返回TRUE
    Others:
            无
********************************************************/
int VideoDevice::startCap(void) {
    unsigned int i;
    for (i = 0; i < nBuffers; ++i) {

        v4l2_buffer buf;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory =V4L2_MEMORY_MMAP;
        buf.index = i;

        if (-1 == ioctl(capFD, VIDIOC_QBUF, &buf)) {

            return FALSE;
        }
    }

    v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (-1 == ioctl(capFD, VIDIOC_STREAMON, &type)) {
        return FALSE;
    }
    return TRUE;
}

/**************************************************
    Function:  stopCap
    Description:   停止采集图像
    Input:
            无
    Output:
            无
    Return:
            如果初始化成功，返回TRUE
    Others:
            无
********************************************************/
int VideoDevice::stopCap(void) {

    v4l2_buf_type type;
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    if (ioctl(capFD, VIDIOC_STREAMOFF, &type) == -1) {

        return FALSE;
    }
    return TRUE;
}

/**************************************************
    Function:  unInitDevice
    Description:   删除初始化摄像头的配置
    Input:
            无
    Output:
            无
    Return:
            如果初始化成功，返回TRUE
    Others:
            无
********************************************************/

int VideoDevice::unInitDevice(void) {

    unsigned int i;

    for (i = 0; i < nBuffers; ++i) {

        if (-1 == munmap(buffers[i].start, buffers[i].length)) {

            LOG_PRINT("munmap error\n");
            return FALSE;
        }

    }

    delete buffers;
    return TRUE;
}

/**************************************************
    Function:  getFrame
    Description:  分配图像缓冲区，获取一帧的图像缓冲内容
    Input:
            无
    Output:
            yuvBuffer:指针指向采集图像的缓冲区，
            len:图像缓冲区的大小
    Return:
            如果初始化成功，返回TRUE
    Others:
            无
********************************************************/
int VideoDevice::getFrame(unsigned char ** yuvBuffer, size_t* len) {

    v4l2_buffer queue_buf;

    queue_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    queue_buf.memory = V4L2_MEMORY_MMAP;

    if (ioctl(capFD, VIDIOC_DQBUF, &queue_buf) == -1) {
        return FALSE;
    }

    *yuvBuffer = (unsigned char *)buffers[queue_buf.index].start;
    *len = buffers[queue_buf.index].length;
    index = queue_buf.index;

    return TRUE;

}

/**************************************************
    Function:  ungetFrame
    Description:  释放图像缓冲区
    Input:
            无
    Output:
             无
    Return:
            如果初始化成功，返回TRUE
    Others:
            无
********************************************************/
int VideoDevice::ungetFrame(void) {

    if (index != -1) {

        v4l2_buffer queue_buf;
        queue_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        queue_buf.memory = V4L2_MEMORY_MMAP;
        queue_buf.index = index;

        if (ioctl(capFD, VIDIOC_QBUF, &queue_buf) == -1) {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}
