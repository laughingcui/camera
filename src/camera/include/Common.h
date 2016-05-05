/*****************************************************************
  Copyright (C), 2015-2016

  File name : Common.h

  Author: cuishiyang     Version:1.0     Date:2015.3.18

  Description:
           定一些数据格式

  ********************************************************************/

#ifndef COMMON_H
#define COMMON_H

#include <cstdlib>
#include <stdio.h>
#include <cstring>

#define IMG_WIDTH 640 //图片像素宽
#define IMG_HEIGHT 480 //图片像素高


#define DEBUG

#ifdef DEBUG
#define LOG_PRINT(format, args...)  printf( "[%s@%s,%d] " format , __FUNCTION__, __FILE__, __LINE__, ##args );
#else
#define LOG_PRINT(format, args...)
#endif

//#pragma  pack(1) //单字节对齐

typedef struct TagBitMapFileHeader {

    unsigned short int bfType;
    unsigned long int  bfSize;
    unsigned long int  bfReserved;
    unsigned long int  bfOffBits;

}BITMAPFILEHEADER;


typedef struct TagBitMapInfoHeader {

    unsigned long int  biSize;
    unsigned long int  biWidth;
    unsigned long int  biHeight;
    unsigned short int biPlanes;
    unsigned short int biBitCount;
    unsigned long int  biCompression;
    unsigned long int  biSizeImage;
    unsigned long int  biXPelsPerMeter;
    unsigned long int  biYPelsPerMeter;
    unsigned long int  biClrUsed;
    unsigned long int  biClrImportant;

}BITMAPINFOHEADER;

typedef struct TagRgbQuad {

     unsigned char rgbBlue;
     unsigned char rgbGreen;
     unsigned char rgbRed;
     unsigned char rgbReserved;

}RGBQUAD;

//声明一个globRgbBuffer全局指针，用于将capThread采集的globRgbBuffer内容传递给PicOpt用于保存图片
extern unsigned char *globRgbBuffer;

#endif // COMMON_H
