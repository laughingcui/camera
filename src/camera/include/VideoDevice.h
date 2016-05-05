/********************************************************
  Copyright (C), 2015-2016

  File name:    VideoDevice.h

  Author: cuishiyang     Version: 1.0    Data:2015.3.18

  Description: 摄像头底层操作
               public：
                    getFrame(void):用于获取一帧数据存到yuvBufPointer中，
                    ungetFrame(void):用于释放缓存空间
               private:
                    int initDevice(void):初始化摄像头设备
                    int openDevice(void):打开摄像头设备
                    int initMmap(void):初始化mmap内存，用于获取内核空间的摄像头数据
                    int startCap(void):开始摄像
                    int stopCap(void):停止摄像
                    int unInitDevice(void):将初始化的摄像头参数去除
                    int closeDevice(void):关闭摄像头

   History:


********************************************************/

#ifndef VIDEODEVICE_H
#define VIDEODEVICE_H

#include <QObject>
#include <stdlib.h>
#include <QString>

//设备号
#define FILE_VIDEO 	"/dev/video4"

typedef struct {
    void * start;//图像缓存头指针
    size_t length;//图像缓存长度
}Buffer;

class VideoDevice : public QObject {
    Q_OBJECT
public:
    VideoDevice(QString devName);
    ~VideoDevice(void);

public:
    int getFrame(unsigned char** yuvBufPointer, size_t *len);
    int ungetFrame(void);

private:

    QString devName;//设备名
    int capFD;//设备的描述符
    Buffer *buffers;//用于存储从摄像头读取的图像信息
    unsigned int nBuffers;//用于遍历数组
    int index;//用于存储摄像头帧队列的图像数量

    int initDevice(void);
    int openDevice(void);
    int initMmap(void);
    int startCap(void);
    int stopCap(void);
    int unInitDevice(void);
    int closeDevice(void);

};
#endif // VIDEODEVICE_H












