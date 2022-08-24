#ifndef CHATDIALOG_H
#define CHATDIALOG_H
#include<QTime>
#include <QWidget>
#include"video.h"
#include"voice.h"
namespace Ui {
class chatdialog;
}

class chatdialog : public QWidget
{
    Q_OBJECT
signals:
//发送聊天数据给kernel
void SIG_sendChatMsg (QString ip,QString content);
//发送信号给kernel，请求视频聊天
void SIG_vide (long ip);
//发送信号给kernel，请求语音通话
void SIG_voice (long ip);
public:
    explicit chatdialog(QWidget *parent = 0);
    ~chatdialog();
//设置聊天窗口信息
void setInfo(QString ip);
//设置聊天内容
void setChatContent(QString content);

private slots:
    void on_pb_send_clicked();

    void on_pb_vide_clicked();

    void on_pb_voice_clicked();

private:
    Ui::chatdialog *ui;
    QString m_ip;
public:
    video *Myvideo;
    Voice *Myvoice;
};

#endif // CHATDIALOG_H



