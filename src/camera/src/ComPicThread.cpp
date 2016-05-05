#include "CompPicThread.h"
#include "AlarmThread.h"

/**************************************************
    Function:   CompPicThread
    Description:  CompPicThread的构造函数，用于初始化CompPicThread，分配globPicArray的数组元素内存空间
    Input:
            无
    Output:
            无
    Return:
            无
    Others:
            无
********************************************************/
CompPicThread::CompPicThread(void):QThread() {

    arrFlag = 0;

    //动态的为内存分配PIC_SIZE份缓冲资源
    for (int i=0; i<PIC_SIZE ; i++) {
        globPicArray[i] = new unsigned char[IMG_WIDTH*IMG_HEIGHT*3];
    }

    //睡眠1秒用于等待CapThread线程摄像头初始化完毕
    sleep(1);
    timerComp = new QTimer(this);
    connect(timerComp,SIGNAL(timeout()),this,SLOT(compPicTimer()));
    timerComp->start(1000);
    LOG_PRINT("glocPicArray new success!\n");
}

/**************************************************
    Function:   ~CompPicThread
    Description:  ~CompPicThread的析构函数，删除globPicArray的数组元素内存空间
    Input:
            无
    Output:
            无
    Return:
            无
    Others:
            无
********************************************************/
CompPicThread::~CompPicThread(void) {

    //动态的删除内存分配的PIC_SIZE份缓冲资源
    for (int i=0; i<PIC_SIZE ; i++) {
       delete globPicArray[i];
    }
    delete timerComp;
    LOG_PRINT("glocPicArray delete success!\n");
}

/**************************************************
    Function:   compPicTimer
    Description:   定时 用于比较两张图片不同
    Input:
            无
    Output:
            无
    Return:
            无
    Others:
            无
********************************************************/
void CompPicThread::compPicTimer(void) {

    //当globPicArray元素没有填充满的时候就填充
    if ( arrFlag < PIC_SIZE ) {

        memcpy(globPicArray[arrFlag], globRgbBuffer, IMG_WIDTH*IMG_HEIGHT*3);
        arrFlag++;
        LOG_PRINT("正在采集图片............\n");
    }
    else {
        //临时将globPicArray第一个元素的指针存储起来
        unsigned char * temp = globPicArray[0];

        //每次将数据向后偏移，最后的那个元素偏移到第一个
        for (int i=PIC_SIZE-1; i>0; i--) {
            globPicArray[i-1] = globPicArray[i];
        }

        //数组第一个元素传给最后个元素
        globPicArray[PIC_SIZE-1] = temp;

        //将新的图片传给最后一个元素
        memcpy(globPicArray[PIC_SIZE-1], globRgbBuffer, IMG_WIDTH*IMG_HEIGHT*3);

        //用于计算像素差值
        int count = compPic(globPicArray[0],globRgbBuffer);
        LOG_PRINT("count = %d\n", count);

        beepAlarm::alarmJudge(count);
    }


}




/**************************************************
    Function:   compPic
    Description:   用于计算标准图片和当前图片的像素差值，将差值的数量返回
    Input:
            standPic,compPic
    Output:
            无
    Return:
            count
    Others:
            无
********************************************************/
int CompPicThread::compPic(const unsigned char *standPic, const unsigned char *compPic) {
    //用于记录不同的像素
    int count = 0;
   //int res=0;
    for (int i=0; i < IMG_HEIGHT*IMG_WIDTH*3; i++) {
        /*
        res=((int)(standPic[i])-48);
        cout<<"res"<<i<<"="<<res<<endl;
        cout<<"\n"<<endl;
        */
        //如果两个图片某个像素值大于设定的阔值，让count++

        if ( abs( standPic[i]-compPic[i] ) > COMP_VALUE ) {

            count++;
        }

    }
    return count;
}

void CompPicThread::run() {

    this->exec();
}














