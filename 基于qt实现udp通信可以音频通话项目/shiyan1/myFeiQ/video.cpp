#include "video.h"
#include "ui_video.h"
#include <QtGlobal>
#include<iostream>
#include<QMessageBox>
#include <qt_windows.h>
using namespace std;
QByteArray video::buf1;
video::video(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::video)
{
    ui->setupUi(this);
    //ui->label->;
    videowidget.setParent(this);//将取景器设置在ui界面中
    videowidget.setGeometry(641,0,320,240);//设置位置
    mcs.setCamera(&cam);
    mcs.setVideoOutput(&videowidget);
   // imagecapture.setQuality();
    mcs.setImageCapture(&imagecapture);
    cam.start();
     videowidget.show();
    videowidget.setVisible(true);
    //------------------
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
    //------------------------------------------
    timer.start(20);
    mcs.imageCapture()->setQuality(QImageCapture::VeryLowQuality);//设置图片编码质量
    connect(&imagecapture,&QImageCapture::imageCaptured,this,&video::oncapture);//摄像机截图
    connect(&timer,&QTimer::timeout,this,&video::onmessage);
    //connect(&timer1,&QTimer::timeout,this,&video::onmessage);
    connect(buffer,&QIODevice::readyRead,this,&video::onmessage1);//当数据可以读取的时候发射readyRead信号，只要你说话就调用这个
    iodevice= audiosink->start();
}

video::~video()
{
    delete ui;

}



void video::onmessage(){
    //服务器发送数据
   mcs.imageCapture()->capture();

    //触发截图信号

}
void video::oncapture(int id,const QImage &preview)//截图
{
    struct STRU_UDP_VIDEO_CHAT rp;

   //cout<<__func__<<endl;
    QImage qim=preview.scaled(640,480,Qt::IgnoreAspectRatio);//如果 aspectRatioMode 是 Qt：：IgnoreAspectRatio，则图像将缩放到大小。
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    qim.save(&buffer, "JPG");
    ///
   // QByteArray array(9,'Q');
   // char buf[10];//数组
    //int len_array = array.size();
    //int len_buf = sizeof(buf);
    int len = qMin( ba.size(), (int)sizeof(rp.content ));
    buf1=ba;
    // 转化
    memcpy( rp.content, ba,  len );
    rp.nType=_DEF_UDP_VIDEO_CHAT;
    rp.nlen=ba.size();
    //cout<<"ba.size():  "<<rp.nlen<<endl;
    Q_EMIT fasong (m_ip,(char*)&rp,sizeof(rp));
     // cout<<sizeof(rp)<<endl;
    //imagevisable( buf1);
  }
 //显示视频函数
void video::imagevisable(QByteArray buf){

  //  cout<<"buf"<<buf.size()<<endl;
    {
   // cout<<__func__<<endl;
  //  unsigned char *a=( unsigned char *)MainWindow:: buf.data();
    //struct STRU_UDP_VIDEO_CHAT *ker=(struct STRU_UDP_VIDEO_CHAT*)buf;
    //QByteArray buf1;
    //cout<<ker->str.size();
    //int n=ker->jer;

    //--------------------------------------------------------

    //buf1 = QByteArray(ker->content, n);
    //cout<<"n:"<<n<<endl;
   // QByteArray bu=buf;
    //buf1=buf;
    }
  bool kkk= ui->label->qim.loadFromData(buf,"JPG");
  //cout<<buf1.size();

    //qDebug()<<"图片合成"<<kkk;
   // cout<<buf1.size()<<endl;
    //trigger=true;
    ui->label->update();

}
//音频设备把语言数据写入buffer
void video::on_startaud_buf(){

    audiosource->start(buffer);
}
//重写关闭事件
void video::closeEvent(QCloseEvent *event){
    //弹出一个询问窗口，是否关闭
    if(QMessageBox::Yes==QMessageBox::question(this,"提示","是否关闭")){
       event->accept();
       //发送关闭信号给kernnel类
       Q_EMIT SIG_closevideo(m_ip);

    }
    else{
        event->ignore();
    }
}
//关闭视频聊天窗口，前的清理new出来数据的工作
void video::cleardata()
{
    buffer->close();
    audiosource->stop();
    videowidget.hide();


    cam.stop();

    videowidget.setVisible(false);
    //------------------
    delete audiosource;
    audiosource=nullptr;
    //将视频缓冲传进去，音频设备开始向buffer写入时会向audiobuf写入
    //buffer=new QBuffer(&audiobuf);

    delete buffer;
    buffer=nullptr;

    audiosink->stop();
    delete audiosink;
    audiosink=nullptr;
    //delete iodevice;
    iodevice=nullptr;
}


void video::onmessage1()
{
    cout<<__func__<<endl;
    buffer->close();//改
    struct STRU_UDP_AUDIO_CHAT rp;
    int len = qMin( audiobuf.size(), (int)sizeof(rp.content ));
    memcpy( rp.content, audiobuf,  len );
    rp.nlen=audiobuf.size();
    cout<<"audiobuf.size():  "<<rp.nlen<<endl;
     audiobuf.clear();
     Q_EMIT fasong (m_ip,(char*)&rp,sizeof(rp));
    buffer->open(QIODevice::ReadWrite);//改
}
void video::devicemakessound(QByteArray buf){
    //客户端读数据
       iodevice->write(buf);//设备
}
