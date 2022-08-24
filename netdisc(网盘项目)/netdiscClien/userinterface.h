#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include<QVBoxLayout>
#include<QMenu>
#include"filepack.h"
#include <QDialog>
#include <QCloseEvent>

#include<QFileDialog>
#include<iostream>
namespace Ui {
class UserInterface;
}

class UserInterface : public QDialog
{
    Q_OBJECT

public:
    explicit UserInterface(QWidget *parent = nullptr);
    ~UserInterface();

signals:
    //给kernel发送下载文件信号
    void SIG_downloadfile();
    //给kernel发送上传文件信号
    void SIG_uploadfile(const char *,const char *);
    //关闭客户文件界面
    void SIG_close();
public:
    //将自己的文件显示到界面上
    void addFile(filepack*item);

    //设置用户信息
    void setInfo(QString name ,QString feeling,int iconId);
    //重写关闭事件
    void closeEvent(QCloseEvent *event);
private slots:
    //void on pb_ menu_ clicked ( ) ;
    //处理菜单点击事件
    //void slot_dealMenu(QAction* action) ;


    void on_pb_upload_clicked();

private:
    QVBoxLayout* m_layout;

    QMenu*m_menu;

private:
    Ui::UserInterface *ui;
};

#endif // USERINTERFACE_H
