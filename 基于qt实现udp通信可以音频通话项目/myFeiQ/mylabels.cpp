#include "mylabels.h"

#include"video.h"


mylabels::mylabels(QWidget *parent)
    : QLabel{parent}
{
    this->show();


}
void mylabels:: paintEvent(QPaintEvent *e){

//    if(MainWindow::trigger){
//      //  qDebug()<<"发送数据了";
//        MainWindow::trigger=false;


    QPainter qpi(this);

   // qDebug()<<qim.sizeInBytes();

    qpi.drawImage(QPoint(0,0), qim);
       // MainWindow:: buf1.clear();






 }
