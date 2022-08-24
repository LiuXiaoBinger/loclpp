#ifndef VOICE_H
#define VOICE_H
#include"pack.h"
#include <QWidget>
#include <QAudioSink>
#include <QAudioSource>
#include <QAudioFormat>
#include <QBuffer>
#include<QMediaDevices>
namespace Ui {
class Voice;
}

class Voice : public QWidget
{
    Q_OBJECT

public:
    explicit Voice(QWidget *parent = nullptr);
    ~Voice();
    void openaudio();
    void devicemakessound(QByteArray buf);
private slots://槽函数
    void sendaudi();
signals:
    void fasong(long ip,char*p,int nlen);
private:
    Ui::Voice *ui;
public:
    long m_ip;
    QIODevice * iodevice;
    QAudioFormat qf;//获取音频设备
    QByteArray audiobuf;//音频缓冲区
    QBuffer* buffer;//定义个buf，设备写入buf在写入音频缓冲区
    QAudioSource *audiosource;
    QAudioDevice audiodevice;//音频设备
    QAudioSink *audiosink;//音频接收
};

#endif // VOICE_H
