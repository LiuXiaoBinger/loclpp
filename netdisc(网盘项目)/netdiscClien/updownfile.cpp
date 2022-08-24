#include "updownfile.h"
#include "ui_updownfile.h"

upDownfile::upDownfile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::upDownfile)
{
    ui->setupUi(this);
}

upDownfile::~upDownfile()
{
    delete ui;
}

//设置用户信息
void upDownfile::setInfo(QString filename,int flag){
    ui->filename->setText(filename);
    if(flag==1)
    setWindowTitle(QString("下载【%s】").arg(filename));
    else
        setWindowTitle(QString("上传【%s】").arg(filename));
}

void upDownfile::closeEvent(QCloseEvent *event)
{
    ui->~upDownfile();
}

//设置进度
void upDownfile::slot_setprogressbar(int filesize,int pos){

            ui->schedule->setValue((float)pos / filesize * 100);
}
