/*******************************************************
Copyright (C) , 2015-2016

Author: cuishiyang   Version: 1.0   Date: 2015.3.18

Description: 图片保存的API
        public:
            //setPicValue:接受CapThread线程传递过来的rgbBuffer
        private:
            savePic(const char *outStr); 从全局变量globRgbBuffer中获取图片信息，保存图片，格式指定为jpg格式
            savePicTime(void);定时存储图片
            timeToStr:将时间转换为字符串类型
            QImageToIplImage:将QImage转换为IplImage

History:


  *****************************************************/

#ifndef PICOPT_H
#define PICOPT_H

#include <QImage>
#include <highgui.h>
#include <QTimer>
#include <opencv.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <cv.h>

class PicOpt : QObject{
    Q_OBJECT
public:
    PicOpt();//构造函数
    ~PicOpt();
    void startSavePic(void);
    void stopSavePic(void);
private:

    unsigned char * imgBuf;
    QImage *frame;
    QTimer *saveTimer;//定时存储图片

private:
    void savePic(const char *outStr);//save picture
    void timeToStr(char *outStr);
    IplImage *QImageToIplImage(const QImage * qImage);

private slots:
    void savePicTime(void);
};

#endif // PICOPT_H
