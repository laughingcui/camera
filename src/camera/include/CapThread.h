/************************************************************
Copyright (C), 2015-2016

File name: CapThread.h

Author: cuishiyang   Version:1.0   Date:2015.3.18

Description:单独开启一个线程对摄像头的操作，采集摄像头数据通过sndImgData函数
            发送给主线程

        signals:
            sendImageData(const QImage &img):向主线程发送一帧图像内容
            //sendRgbValue(const unsigned char &rgbBuffer):向其他线程发送一帧图像内容(rgb格式)
        private:
            convertYuvToRgbBuffer():将Yuv图像转换为RGB格式并存储到buffer缓冲区中
        private slots:
            flushPic(void):定时器动态扫描图片调用该函数
            sendRgbBuffer(void):定时器每隔一秒将rgb数据发送给其他线程
History:


**************************************************************/

#ifndef CAPTHREAD_H
#define CAPTHREAD_H

#include <QImage>
#include <QTimer>
#include <QThread>

#include "VideoDevice.h"
#include "Common.h"

class CapThread : public QThread {
    Q_OBJECT

public:
    explicit CapThread();
    ~CapThread();

protected:
    void run();

signals:
    void sendImageData(const QImage &img);//向主线程发送一帧图像内容

private:

    QImage *frameImg;//用于捕获一帧图像
    QTimer *timerImg;//用于定时扫描摄像头设备,从摄像头每秒钟采集20帧
    size_t len;//yuvBuffer缓冲区内容字节长度

    VideoDevice *vd;//摄像头对象
    unsigned char *yuvBuffer;//yuv缓冲区
    BITMAPFILEHEADER   bf;//BMP图像格式
    BITMAPINFOHEADER   bi;

private:
    int convertYuvToRgbBuffer(void);

private slots:
    void flushPic(void);
};

#endif // CAPTHREAD_H
