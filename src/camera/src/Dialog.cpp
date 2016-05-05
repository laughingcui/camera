#include "Dialog.h"
#include "ui_Dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog) {
    ui->setupUi(this);
    video = new CapThread();
    picOpt = new PicOpt();

    comPic = new CompPicThread();
    video->start();

    //通过信号将子线程capThread的QImage数据传给本线程
    connect(video,SIGNAL(sendImageData(const QImage &)),this,SLOT(showPicture(const QImage &)));
}

Dialog::~Dialog() {

    delete video;
    delete picOpt;
   // delete[] cpRgbBuffer ;
    delete comPic;
    delete ui;
}
/**************************************************
    Function:  showPicture
    Description:   获取CapThread子线程的QImage图像数据，将数据传给label->setPixmap
    Input:
            img的引用，获取子线程QImage的数据
    Output:
            无
    Return:
            无
    Others:
            无
********************************************************/
void Dialog::showPicture(const QImage &img) {

    ui->label->setPixmap(QPixmap::fromImage(img));
}

/**************************************************
    Function:  showPicture
    Description:   获取CapThread子线程的QImage图像数据，将数据传给label->setPixmap
    Input:
            img的引用，获取子线程QImage的数据
    Output:
            无
    Return:
            无
    Others:
            无
********************************************************/
void Dialog::on_pushButton_clicked() {

    static  bool pic_save_flag;
    pic_save_flag = !pic_save_flag;

    if(pic_save_flag) {

        ui->pushButton->setText(tr("stop photo"));
        picOpt->startSavePic();

    }
    else {

        ui->pushButton->setText(tr("start photo"));
        picOpt->stopSavePic();
    }
}
/*
void Dialog::on_label_linkActivated(const QString &link) {

}
*/
