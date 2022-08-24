#include "feiqdialog.h"
#include "ui_feiqdialog.h"
#include<QDebug>
#include<QMessageBox>
FeiQDialog::FeiQDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FeiQDialog)
{
    ui->setupUi(this);
}

FeiQDialog::~FeiQDialog()
{
    delete ui;
}

void FeiQDialog::addFriend(QString ip, QString name)
{
    //1、增加一行，总行数加一
    ui->tw_friend->setRowCount(ui->tw_friend->rowCount()+1);
    //2、设置这一行的两个item
    QTableWidgetItem* item1 = new QTableWidgetItem (ip);
    ui->tw_friend->setItem (ui->tw_friend->rowCount() - 1,0, item1);
    QTableWidgetItem* item2 = new QTableWidgetItem (name) ;
    ui->tw_friend->setItem(ui->tw_friend->rowCount() - 1,1, item2);

}

void FeiQDialog::deleteFriend(QString ip)
{

}

void FeiQDialog::closeEvent(QCloseEvent *event)
{
    //弹出一个询问窗口，是否关闭
    if(QMessageBox::Yes==QMessageBox::question(this,"提示","是否关闭")){
       event->accept();
       //发送关闭信号给kernnel类
       Q_EMIT SIG_close();

    }
    else{
        event->ignore();
    }
}

void FeiQDialog::on_tw_friend_cellDoubleClicked(int row, int column)
{
    qDebug()<<QString("(%1,%2)").arg(row).arg(column);
    QString ip=ui->tw_friend->item(row,0)->text();
    Q_EMIT SIG_userClicked(ip);

}
