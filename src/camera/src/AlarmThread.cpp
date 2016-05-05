#include "AlarmThread.h""
#include "CompPicThread.h"

beepAlarm::beepAlarm(void):QThread() {

    timerbeep=new QTimer(this);
    connect(timerbeep,SIGNAL(timeout()),this,SLOT(alarmJudge()));
    timerbeep->start(1000);
}

beepAlarm::~beepAlarm(void) {

    this->quit();
    this->wait();
}

void beepAlarm::alarmJudge(int num) {

    if (num > 100000) {

        beepAlarm::beep_on();
    }
    else {

        beepAlarm::beep_off();
    }
}

int beepAlarm::beep_on() {

    int res = 0;
    res = initBeep();

    if (res < 0) {
        printf("open failed\n");
        return 0;
    }

    printf("open success\n");

    startBeep();
    closeBeep();
}

int beepAlarm::beep_off(){

    int res = 0;
    res = initBeep();

    if (res < 0) {
        printf("open failed\n");
        return 0;
    }

    printf("open success\n");

    stopBeep();
    closeBeep();
}


void beepAlarm::run() {

    this->exec();
}
