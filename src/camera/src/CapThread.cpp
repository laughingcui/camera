#include <QByteArray>
#include <QBuffer>
#include <QImage>
#include <QDebug>
#include <iostream>

#include "CapThread.h"


using namespace std;
//globRgbBuffer全局指针，用于将capThread采集的globRgbBuffer内容传递给PicOpt用于保存图片
unsigned char* globRgbBuffer = new unsigned char[IMG_WIDTH*IMG_HEIGHT*3];

/**************************************************
    Function:   CapThread
    Description:  CapThread的构造函数，用于初始化CapThread，配置bmp，分配内存空间
    Input:
            无
    Output:
            无
    Return:
            无
    Others:
            无
********************************************************/
CapThread::CapThread():QThread() {
    //Set BITMAPINFOHEADER
    bi.biSize = 40;
    bi.biWidth = IMG_WIDTH;
    bi.biHeight = IMG_HEIGHT;
    bi.biPlanes = 1;
    bi.biBitCount = 24;
    bi.biCompression = 0;
    bi.biSizeImage = IMG_WIDTH*IMG_HEIGHT*3;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    //Set BITMAPFILEHEADER
    bf.bfType = 0x4d42;
    bf.bfSize = 54 + bi.biSizeImage;
    bf.bfReserved = 0;
    bf.bfOffBits = 54;


    vd = new VideoDevice(tr("/dev/video4"));
    frameImg = new QImage(globRgbBuffer,IMG_WIDTH,IMG_HEIGHT,QImage::Format_RGB888);

    timerImg= new QTimer(this);
    //定时刷新图片
    connect(timerImg,SIGNAL(timeout()),this,SLOT(flushPic()));
    timerImg->start(50);
}

/**************************************************
    Function:   ~CapThread
    Description:  CapThread的析构函数，释放内存空间
    Input:
            无
    Output:
            无
    Return:
            无
    Others:
            无
********************************************************/
CapThread::~CapThread() {
    this->quit();
    //quit_flag = true;
    this->wait();
    delete[] globRgbBuffer;

    delete frameImg;
    delete timerImg;

}

/**************************************************
    Function:   flushPic
    Descriptions: 定时器的槽函数，用于加载图片，将图片的内容通过sendImageData发送给
                   UI线程
    Input:
            无
    Output:
            无
    Return:
            无
    Others:
            无
********************************************************/
void CapThread::flushPic(void) {

     vd->getFrame(&yuvBuffer, &len);
    convertYuvToRgbBuffer();

    frameImg->loadFromData(globRgbBuffer,IMG_WIDTH * IMG_HEIGHT * 3);

    vd->ungetFrame();
    emit sendImageData(*frameImg);//向主线程发送一帧图像内容

}

/**************************************************
    Function:   convertYuvToglobRgbBuffer
    Description:  将YUV格式的图片转换为RGB格式的图片并存入globRgbBuffer中。
    Input:
            无
    Output:
            无
    Return:
            图片转换成功返回0
    Others:
            无
********************************************************/
int CapThread::convertYuvToRgbBuffer(void) {

    unsigned long in, out = 0;
    int y0, u, y1, v;
    int r, g, b,gray;

    for (in = 0; in < IMG_WIDTH * IMG_HEIGHT * 2; in += 4) {
        //在yuvBuffer提取yuv色值
        y0 = yuvBuffer[in + 0];
        u  = yuvBuffer[in + 1];
        y1 = yuvBuffer[in + 2];
        v  = yuvBuffer[in + 3];

        //通过下面算法将YUV转换为RGB
        r = y0 + (1.370705 * (v-128));
        g = y0 - (0.698001 * (v-128)) - (0.337633 * (u-128));
        b = y0 + (1.732446 * (u-128));
        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        if (r < 0) r = 0;
        if (g < 0) g = 0;
        if (b < 0) b = 0;

        //灰度转换
        gray = (r*30+b*59+b*11)/100;

        //将转换好的值存储在globRgbBuffer
        globRgbBuffer[out++] = gray;
        globRgbBuffer[out++] = gray;
        globRgbBuffer[out++] = gray;

        r = y1 + (1.370705 * (v-128));
        g = y1 - (0.698001 * (v-128)) - (0.337633 * (u-128));
        b = y1 + (1.732446 * (u-128));

        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        if (r < 0) r = 0;
        if (g < 0) g = 0;
        if (b < 0) b = 0;
        //灰度转换
        gray = (r*30+b*59+b*11)/100;
        globRgbBuffer[out++] = gray;
        globRgbBuffer[out++] = gray;
        globRgbBuffer[out++] = gray;
    }
    return 0;
}

/**************************************************
    Function:   run
    Description:  重写父类QThread的run函数，用于线程的运行。
    Input:
            无
    Output:
            无
    Return:
            无
    Others:
            无
********************************************************/
void CapThread::run(){

        this->exec();
}

