#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "Common.h"
#include "CapThread.h"
#include "PicOpt.h"
#include "CompPicThread.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog {
    Q_OBJECT
    
public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    
private:

    Ui::Dialog *ui;
    CapThread * video;
    PicOpt * picOpt;
    CompPicThread *comPic;
    unsigned char *cpRgbBuffer;

private slots:

    void showPicture(const QImage &img);
   // void on_label_linkActivated(const QString &link);
    void on_pushButton_clicked();
};

#endif // DIALOG_H
