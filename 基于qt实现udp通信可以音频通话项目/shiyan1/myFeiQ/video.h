#ifndef VIDEO_H
#define VIDEO_H

#include <QWidget>

#include"pack.h"
#include<QCamera>
#include<QVideoWidget>
#include<QMediaCaptureSession>
#include<QMediaDevices>
#include<QAudioSink>
#include<QImageCapture>
#include<QTimer>
#include<QBuffer>
#include<QAudioFormat>
#include<QAudioDevice>
#include<QAudioSource>
#include<QIODevice>
#include<QCloseEvent>
namespace Ui {
class video;
}

class video : public QWidget
{
    Q_OBJECT

public:
     static QByteArray buf1;
    explicit video(QWidget *parent = nullptr);
    ~video();
    void imagevisable(QByteArray buf);//x显示视频
    void devicemakessound(QByteArray buf);
    //音频设备把语言数据写入buffer
    void on_startaud_buf();
    //重写关闭事件
    void closeEvent(QCloseEvent*event);
    //关闭视频聊天窗口，前的清理new出来数据的工作
    void cleardata();
private slots://槽函数
     void oncapture(int id,const QImage &preview);//发送截图
     void onmessage();//服务器发送数据

    void onmessage1();
    //读取语音包数据让设备发声

private:
    Ui::video *ui;
    QVideoWidget videowidget;
    QMediaCaptureSession mcs;
    QImageCapture imagecapture;
    QCamera cam;
    //QImage qim;//测试
   // QHostAddress address;
    QTimer timer;
    QTimer timer1;
    QIODevice * iodevice;

    QAudioSource *audiosource;
    QAudioDevice audiodevice;

      QAudioFormat qf;//获取音频设备
      QByteArray audiobuf;//音频缓冲区
      QBuffer* buffer;//定义个buf，设备写入buf在写入音频缓冲区
      QAudioSink *audiosink;
public:
      long m_ip;
signals:
    void fasong(long ip,char*p,int nlen);
    //传给ckernel类
    void SIG_closevideo(long ip);
};

#endif // VIDEO_H
