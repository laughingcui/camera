/*******************************************************
Copyright (C), 2015-2016

File name: CompPicThread.h

Author: cuishiyang  Version:1.0   Date:2015.3.18

Description:单独开启一个线程对摄像头采集的图像进行比较操作


        protected:
            void run();线程执行函数

        private:
             int compPic(const unsigned char *standPic, const unsigned char *compPic);
             用于比较两张图片不同, standPic:是要比较的图片,compPic:是被比较的图片
        private slots:
            compPicTimer(void):定时动态用于比较两张图片不同

History:
********************************************************/

#ifndef COMPPICTHREAD_H
#define COMPPICTHREAD_H

#include <QThread>
#include <QTimer>
#include <iostream>

#include "Common.h"
using namespace std;

#define COMP_VALUE 50 // 比较两张图片某个像素的临界阔值
#define PIC_SIZE 3//保存N张图片在缓冲队列


class CompPicThread : public QThread {
    Q_OBJECT
public:
    CompPicThread(void);
    ~CompPicThread(void);

protected:
    void run();

private:
    int compPic(const unsigned char *standPic, const unsigned char *compPic);//用于比较两张图片不同


private:

    QTimer *timerComp;

    //声明一个globPicArray全局指针数组，用于存储指定秒数保存图片，并把图片缓存指向该指针数组的每一个元素
   unsigned char* globPicArray[PIC_SIZE];
   int arrFlag; //用于标记globPicArray元素是否满了

private slots:
  void compPicTimer(void);
};

#endif // COMPPICTHREAD_H

















