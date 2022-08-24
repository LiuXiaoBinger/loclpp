#include "logindialog.h"
#include "ui_logindialog.h"
#include<QMessageBox>
#include<QCloseEvent>
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::closeEvent(QCloseEvent *event)
{
    //接收事件，发信号通知kernel
    event->accept();
    Q_EMIT SIG_CloseWnd();
}

void LoginDialog::on_pb_commit_register_clicked()
{
    //1、从控件上获取数据
    QString name = ui->le_name_register->text();
    QString tel = ui->le_tel_register->text();
    QString pas = ui->le_password_register->text();
    //2、校验用户输入的数据

    //2.1、长度校验，不能为空，不能全是空格
    QString nameTemp = name;
     QString telTemp = tel;
    if(name.isEmpty() || tel.isEmpty() ||pas.isEmpty()
        || nameTemp.remove(" ").isEmpty() || telTemp.remove(" ").isEmpty())
    {
        QMessageBox::about(this,"提示","昵称，电话，密码不能为空，并且昵称和电话不能全是空格");
        return ;

    }
    //2.2、长度限制，tel只能是11位，昵称不能超过10位，密码不能超过15位
    if(name.length()>10 || tel.length()!=11 || pas.length() >15){
        QMessageBox::about(this,"提示","tel只能是11位，昵称不能超过10位，密码不能超过15位");
        return ;
    }
    //TODO:2.3、内容校验，tel只能是数字，昵称不能有特殊字符--后面会讲正则表达式

    //3、使用信号发送数据给Kernel
    Q_EMIT SIG_RegisterCommit( name, tel, pas);
}

//注册界面的清空按钮
void LoginDialog::on_pb_clear_register_clicked()
{
    ui->le_name_register->setText("");
    ui->le_tel_register->setText("");
    ui->le_password_register->setText("");
}


void LoginDialog::on_pb_commit_clicked()
{
    //1、从控件上获取数据
    QString tel = ui->le_tel->text();
    QString pas = ui->le_password->text();
    //2、校验用户输入的数据

    //2.1、长度校验，不能为空，不能全是空格
     QString telTemp = tel;
    if(tel.isEmpty() || pas.isEmpty() || telTemp.remove(" ").isEmpty())
    {
        QMessageBox::about(this,"提示","电话，密码不能为空，并且昵称和电话不能全是空格");
        return ;

    }
    //2.2、长度限制，tel只能是11位，密码不能超过15位
    if( tel.length()!=11 || pas.length() >15){
        QMessageBox::about(this,"提示","tel只能是11位，密码不能超过15位");
        return ;
    }
    //TODO:2.3、内容校验，tel只能是数字，昵称不能有特殊字符--后面会讲正则表达式

    //3、使用信号发送数据给Kernel
    Q_EMIT SIG_LoginCommit( tel, pas);
}

//登陆界面的清空按钮
void LoginDialog::on_pb_clear_clicked()
{
    ui->le_password->setText("");
    ui->le_tel->setText("");
}

