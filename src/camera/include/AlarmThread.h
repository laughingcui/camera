/***************************************************************
 *CopyRight (C), 2015-2016
 *
 *Filename: AlarmThread.h

 *Author: cuishiyang   Version:1.0   Date:2015.3.31

  Description:单独开启一个线程用于蜂鸣器报警

       public：
           alarmJudge():判断是否达到了报警的阈值，然后决定是否调用报警函数
           beep_on():开启报警
           beep_off)():关闭报警
 *
 *History:
 **************************************************************/
#ifndef ALARMTHREAD_H
#define ALARMTHREAD_H


#include <QThread>
#include <QTimer>
#include <iostream>

#include "beep.h"
#include "Common.h"

using namespace std;

class beepAlarm:public QThread {

    Q_OBJECT
public:
    beepAlarm(void);
    ~beepAlarm(void);

public:

    static void alarmJudge(int);
    static int beep_on();
    static int beep_off();

private:
    QTimer *timerbeep;



protected:
    void run();

};


#endif // ALARMTHREAD_H
