#include "voice.h"
#include <qstyle.h>
#include "ui_voice.h"

Voice::Voice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Voice)
{

    ui->setupUi(this);
    //style()->unpolish(ui->btn);
    qf.setSampleRate(44100);
    qf.setChannelCount(2);
    qf.setSampleFormat(QAudioFormat::UInt8);
    audiodevice=QMediaDevices::defaultAudioInput();
     qDebug()<<audiodevice.isFormatSupported(qf);
    //-------------------------------------------
    audiosource=new QAudioSource(qf,this);
    //将视频缓冲传进去，音频设备开始向buffer写入时会向audiobuf写入
    buffer=new QBuffer(&audiobuf);
    //设置buffer权限
    buffer->open(QIODevice::ReadWrite|QIODevice::Truncate);
    //构造新的音频输出并将其附加到父级。默认音频输出设备与输出格式参数一起使用。
    audiosink=new QAudioSink(qf,this);

    connect(buffer,&QIODevice::readyRead,this,&Voice::sendaudi);//当数据可以读取的时候发射readyRead信号，只要你说话就调用这个
    iodevice= audiosink->start();
}

Voice::~Voice()
{
    delete ui;
}
void Voice::sendaudi()
{
    std::cout<<__func__<<std::endl;
    buffer->close();//改
    struct STRU_UDP_voice_CHAT rp;
    int len = qMin( audiobuf.size(), (int)sizeof(rp.content ));
    memcpy( rp.content, audiobuf,  len );
    rp.nlen=audiobuf.size();
    //cout<<"audiobuf.size():  "<<rp.nlen<<endl;
     audiobuf.clear();
     Q_EMIT fasong (m_ip,(char*)&rp,sizeof(rp));
    buffer->open(QIODevice::ReadWrite);//改
}
void Voice::openaudio()
{
    audiosource->start(buffer);
}
void Voice::devicemakessound(QByteArray buf){
    //客户端读数据
       iodevice->write(buf);//设备
}
