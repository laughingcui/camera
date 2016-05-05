#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "Common.h"
#include "PicOpt.h"

/*******************************************************************************
  Function: PicOpt
  Description: PicOpt构造函数,为frame和rgbBuffer动态分配内存空间
  Input:
        无
  Output:
        无
  Return:
        无
  Other:
        无

  ****************************************************************************/
PicOpt::PicOpt() {

    frame = new QImage(globRgbBuffer,IMG_WIDTH,IMG_HEIGHT,QImage::Format_RGB888);
    saveTimer = new QTimer(this);
    connect(saveTimer,SIGNAL(timeout()),this,SLOT(savePicTime()));
}
/*******************************************************************************
  Function: ~PicOpt
  Description: PicOpt析构函数,为frame和rgbBuffer释放分配内存空间
  Input:
        无
  Output:
        无
  Return:
        无
  Other:
        无

  ****************************************************************************/
PicOpt::~PicOpt() {
  //  delete[] rgbBuffer;
    delete frame;

}
/*******************************************************************************
  Function: savePic
  Description: 从全局变量globRgbBuffer中获取图片信息，保存图片，格式指定为jpg格式
  Input:
        filename
  Output:
        无
  Return:
        无
  Other:
        无
  ****************************************************************************/
void PicOpt::savePic(const char *filename) {

    char strTmp[40] = {};
    sprintf(strTmp, "./picture/%s.jpg",filename);

    //从全局变量中获取RgbBuffer的内容
    frame->loadFromData(globRgbBuffer,IMG_WIDTH * IMG_HEIGHT * 3);

    IplImage* src = QImageToIplImage(frame);//QImage 转成IplImage

    if(access("picture", F_OK) == -1) {

        LOG_PRINT("picture is not exist!!!\n");
        system("mkdir picture");
    }
    LOG_PRINT("save %s \n", strTmp);
    cvSaveImage(strTmp,src);

    //释放src分配的内存空间
    cvReleaseImage(&src);
   // src = NULL;
}

/*******************************************************************************
  Function: savePicTime
  Description: 定时保存图片
  Input:
        无
  Output:
        无
  Return:
        无
  Other:
        无
  ****************************************************************************/
void PicOpt::savePicTime(void) {


    //this->rgbBuffer = &rgbBuffer;
    char timeStr[50] = {};
    this->timeToStr(timeStr);
    LOG_PRINT("localtime=%s\n", timeStr);
    this->savePic(timeStr);
}

/*******************************************************************************
  Function: timeToStr
  Description: 将时间转换为字符串
  Input:
        无
  Output:
        outStr
  Return:
        无
  Other:
        无
  ****************************************************************************/
void PicOpt::timeToStr(char * outStr) {

    struct tm* local_time;
    time_t rawtime;

    time(&rawtime);

    local_time = localtime(&rawtime);
    local_time->tm_mon+=1;
    local_time->tm_year+=1900;

    sprintf(outStr, "%d%02d%02d%02d%02d%02d",
            local_time->tm_year,local_time->tm_mon,
            local_time->tm_mday, local_time->tm_hour,
           local_time->tm_min,local_time->tm_sec);
}

/*******************************************************************************
  Function: QImageToIplImage
  Description: 将QImage转换为IplImage,并返回IplImage
  Input:
       qImage
  Output:
        无
  Return:
        IplImageBuffer;
  Other:
        无
  ****************************************************************************/
IplImage* PicOpt::QImageToIplImage(const QImage * qImage) {

    int width = qImage->width();
    int height = qImage->height();
    CvSize Size;
    Size.height = height;
    Size.width = width;

    IplImage *IplImageBuffer = cvCreateImage(Size, IPL_DEPTH_8U, 3);    //记着释放内存

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {

            QRgb rgb = qImage->pixel(x, y);
            cvSet2D(IplImageBuffer, y, x, CV_RGB(qRed(rgb), qGreen(rgb), qBlue(rgb)));
        }
    }
    return IplImageBuffer;
}

/*******************************************************************************
  Function: startSavePic
  Description: 开始保存图片
  Input:
       无
  Output:
        无
  Return:
        无
  Other:
        无
  ****************************************************************************/
void PicOpt::startSavePic(void) {

    saveTimer->start(1000);
}

/*******************************************************************************
  Function: stopSavePic
  Description: 停止保存图片
  Input:
       无
  Output:
        无
  Return:
        无
  Other:
        无
  ****************************************************************************/
void PicOpt::stopSavePic(void) {

    saveTimer->stop();
}
