#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

signals:
    //发送注册信息给kernel
    void SIG_RegisterCommit(QString name,QString tel,QString password);
    //发送登录信息给kernel
    void SIG_LoginCommit(QString tel,QString password);
    //关闭窗口信号
    void SIG_CloseWnd();


public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    //重写关闭窗口事件
    void closeEvent(QCloseEvent*event);

private slots:
    void on_pb_commit_register_clicked();

    void on_pb_clear_register_clicked();

    void on_pb_commit_clicked();

    void on_pb_clear_clicked();

private:
    Ui::LoginDialog *ui;
};

#endif // LOGINDIALOG_H
