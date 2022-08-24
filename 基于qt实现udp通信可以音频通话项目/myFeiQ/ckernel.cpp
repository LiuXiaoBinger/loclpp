#include "ckernel.h"
#include <QMessageBox>
#include <iostream>
#include "INet.h"

using namespace std;

CKernel::CKernel(QObject *parent) : QObject(parent),m_pMainWnd(nullptr),m_pMediator(nullptr)
{
    //初始化好友列表
    m_pMainWnd=new FeiQDialog;
    m_pMainWnd->show();
    //绑定用户点击好友列表某一行的信号和槽
    connect(m_pMainWnd,SIGNAL(SIG_userClicked(QString)),this,SLOT(slot_userClicked(QString)));
    //绑定关闭好友列表窗口的信号和槽
    connect(m_pMainWnd,SIGNAL(SIG_close()),this,SLOT(slot_close()));

    //初始化中介着指针
    m_pMediator=new UdpMediator;
    //打开网络
    if(!m_pMediator->OpenNet()){
        //打开网路失败，提示用户，退出进程
        QMessageBox::about(m_pMainWnd,"提示","打开网络失败");
        exit(0) ;//直接退出程序

    }
    //在接收信号的对象里面，new出发送信号的对象后面，绑定信号和槽函数
    //绑定发送接收到的所有数据的信号和处理槽函数
    connect(m_pMediator,SIGNAL(SIG_readyData(long ,char* ,int )),this,SLOT(Slot_readyData(long,char*,int)));
    //测试代码，发送广播，“hello world”
   // m_pMediator->SendData(INADDR_BROADCAST,"hello world",strlen("hello world"+1)) ;
    //上线，发送上线请求广播
    struct STRU_UDP_ONLINE rp;
    gethostname(rp.name,sizeof(rp.name));//获取主机名字
    m_pMediator->SendData(INADDR_BROADCAST,(char*)&rp,sizeof(rp)) ;

}

CKernel::~CKernel()
{
        //回收资源
        //回收好友列表窗口，先隐藏窗口，再销毁对象
        if(m_pMainWnd){
            m_pMainWnd->hide();
            delete m_pMainWnd;
            m_pMainWnd = nullptr;
        }
        //回收中介者类指针，先关闭网络，再销毁对象
        if(m_pMediator){
            m_pMediator->CloseNet();
            delete m_pMediator;
            m_pMediator = nullptr;
        }
        for(auto ite=m_mapIpToChatDlg.begin();ite!=m_mapIpToChatDlg.end();){
            chatdialog*chat=ite->second;
            if(chat){
                chat->hide();
                delete chat;
                chat=nullptr;
            }
            ite=m_mapIpToChatDlg.erase(ite);
        }
    }

//CKernel::~CKernel()

//处理上线请求
void CKernel::slot_dealOnlineRq (long lsendIp,char* buf, int nLen){
    cout <<__func__<<endl;
    //1.拆包
    STRU_UDP_ONLINE*rp=(STRU_UDP_ONLINE*)buf;
    //2、将请求的用户添加到好友列表
    m_pMainWnd->addFriend(INet::getIpstring(lsendIp).c_str(),rp->name);
    //3、创建聊天窗口
    chatdialog*chat=new chatdialog;
    chat->setInfo ( INet::getIpstring ( lsendIp).c_str( ));
    //绑定处理用处输入的聊天内容的信号的槽函数
    connect(chat,SIGNAL(SIG_sendChatMsg(QString,QString)),this,SLOT(slot_sendChatMsg(QString,QString)));
     //绑定视频通话请求信号和槽
    connect(chat,SIGNAL(SIG_vide (long)),this,SLOT(slot_viderq (long)));
    //绑定语音通话请求信号和槽
    connect(chat,SIGNAL(SIG_voice (long)),this,SLOT(slot_voice (long)));
    //4、放到map中管理，ip->聊天窗口
    m_mapIpToChatDlg[lsendIp]=chat;
    //5、判断是否是自己的上线请求，先获取本机ip地址列表，然后在判断是不是自己的ip地址
    set<long>setIp=INet::getvalidIpList();
    if(setIp.count(lsendIp)){
        return;
    }

    //6、如果不是自己的上线请求，给对方发一个上线回复
    STRU_UDP_ONLINE rs;
    rs.nType=_DEF_UDP_ONLINE_RS;
    gethostname(rs.name,sizeof(rs.name));
    m_pMediator->SendData(lsendIp,(char*)&rs,sizeof(rs));
}
//处理上线回复
void CKernel::slot_dealonlineRs(long lsendIp,char* buf, int nLen){
    cout <<__func__<<endl;
    //1、拆包
    STRU_UDP_ONLINE*rq = (STRU_UDP_ONLINE* ) buf;
    //2、将请求的用户添加到好友列表里
    m_pMainWnd->addFriend (INet:: getIpstring (lsendIp).c_str(),rq->name) ;
    // 3、创建聊天窗口
    chatdialog*chat = new chatdialog;
    chat->setInfo ( INet::getIpstring ( lsendIp).c_str( ));
    //绑定视频通话请求信号和槽
    connect(chat,SIGNAL(SIG_vide (long)),this,SLOT(slot_viderq (long)));
    //绑定语音通话请求信号和槽
    connect(chat,SIGNAL(SIG_voice (long)),this,SLOT(slot_voice (long)));
    //绑定处理用处输入的聊天内容的信号和槽函数
    connect (chat,SIGNAL(SIG_sendChatMsg(QString,QString) ) ,
    this,SLOT (slot_sendChatMsg(QString, QString)) );
    //4、放在map中管理，ip->聊天窗口
    m_mapIpToChatDlg[lsendIp]= chat;

}
//处理下线请求
void CKernel::slot_dealofflineRq (long lsendIp,char* buf, int nLen){
    cout <<__func__ <<endl;
    //将好友从好友列表删除
    m_pMainWnd->deleteFriend(INet::getIpstring(lsendIp).c_str());
    //将与该好友的聊天窗口关闭，并从map中移除
    if(m_mapIpToChatDlg.find(lsendIp)!=m_mapIpToChatDlg.end()){
        chatdialog*chat=m_mapIpToChatDlg[lsendIp];
        if(chat){
            chat->hide();
            delete chat;
            chat=nullptr;
        }
        m_mapIpToChatDlg.erase(lsendIp);
    }


}
//处理聊天请求
void CKernel::slot_dealChatRq(long lsendIp,char* buf, int nLen){
    cout <<__func__<<endl;
    //拆包
    STRU_UDP_CHAT_RQ*rp=(STRU_UDP_CHAT_RQ*)buf;
    //2.判断是否跟这个人有聊天窗口
    if(m_mapIpToChatDlg.find(lsendIp)!=m_mapIpToChatDlg.end()){
        //如果有聊天窗口设置聊天内容，并且显示窗口
        chatdialog*chat=m_mapIpToChatDlg[lsendIp];
        chat->setChatContent(rp->content);
        chat->showNormal();
    }
    //



}

void CKernel::slot_dealVideRq(long lsendIp, char *buf, int nLen)
{
    cout <<__func__<<endl;
    //拆包
    //char*p=(to_string(lsendIp).c_str());
    if(QMessageBox::Yes==QMessageBox::question(this->m_pMainWnd,"视频聊天",
    INet::getIpstring(lsendIp).c_str(),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)){
      // event->accept();
       //发送关闭信号给kernnel类
       //Q_EMIT SIG_close();question(this,"视频聊天请求",p)
        struct STRU_UDP_VIDEO rp;
        rp.nType=_DEF_UDP_VIDEO_RS;
        m_pMediator->SendData(lsendIp,(char*)&rp,sizeof(rp));
        if(m_mapIpToChatDlg.find(lsendIp)!=m_mapIpToChatDlg.end()){
            //如果有聊天窗口设置聊天内容，并且显示窗口
            chatdialog*chat=m_mapIpToChatDlg[lsendIp];
            if(chat->Myvideo==nullptr){
               chat->Myvideo=new video;
               //将Myvideo截图数据转到ckermel里面在转给外面
              connect(chat->Myvideo,SIGNAL(fasong(long,char*,int)),this,SLOT(slot_fasongvide (long,char*,int)));
               connect(chat->Myvideo,SIGNAL(SIG_closevideo(long )),this,SLOT(slot_videoclose (long )));
               chat->Myvideo->show();
               chat->Myvideo->on_startaud_buf();
               chat->Myvideo->m_ip=lsendIp;
            }
    }
    //
 }
}
//处理视频聊天回复
void CKernel::slot_dealVideRs(long lsendIp, char *buf, int nLen)
{
    if(m_mapIpToChatDlg.find(lsendIp)!=m_mapIpToChatDlg.end()){
        //如果有聊天窗口设置聊天内容，并且显示窗口
        chatdialog*chat=m_mapIpToChatDlg[lsendIp];
        if(chat->Myvideo==nullptr){
           chat->Myvideo=new video;
           //将Myvideo截图数据转到ckermel里面在转给外面
           connect(chat->Myvideo,SIGNAL(fasong(long,char*,int)),this,SLOT(slot_fasongvide (long,char*,int)));
           connect(chat->Myvideo,SIGNAL(SIG_closevideo(long )),this,SLOT(slot_videoclose (long )));
           chat->Myvideo->show();
           chat->Myvideo->on_startaud_buf();
           chat->Myvideo->m_ip=lsendIp;
        }

    }
}
//处理视频聊天音频
void CKernel::slot_dealaudiochat(long lsendIp, char *buf, int nLen)
{
    chatdialog*chat=m_mapIpToChatDlg[lsendIp];
    if(chat->Myvideo){
        struct STRU_UDP_AUDIO_CHAT *ker=(struct STRU_UDP_AUDIO_CHAT*)buf;
        QByteArray array;
        array = QByteArray(ker->content, ker->nlen);
        chat->Myvideo->devicemakessound(array);
    }
}
//这个处理视频图片包的
void CKernel::slot_dealVidechat(long lsendIp, char *buf, int nLen)
{
    cout<<__func__<<endl;
    chatdialog*chat=m_mapIpToChatDlg[lsendIp];
    if(chat->Myvideo){
        struct STRU_UDP_VIDEO_CHAT *ker=(struct STRU_UDP_VIDEO_CHAT*)buf;
        QByteArray array;
        array = QByteArray(ker->content, ker->nlen);
        chat->Myvideo->imagevisable(array);
    }
}
//处理语音通话包
void CKernel::slot_voicechat(long lsendIp, char *buf, int nLen)
{
    chatdialog*chat=m_mapIpToChatDlg[lsendIp];
    if(chat->Myvoice){
        struct STRU_UDP_voice_CHAT *ker=(struct STRU_UDP_voice_CHAT*)buf;
        QByteArray array;
        array = QByteArray(ker->content, ker->nlen);
        chat->Myvoice->devicemakessound(array);
    }
}
//处理语音通话请求
void CKernel::slot_voicechatRp(long lsendIp, char *buf, int nLen)
{
    cout <<__func__<< endl;
    //string f=getIpstring(lsendIp);
    qDebug()<<(INet::getIpstring(lsendIp)).c_str();
    if(QMessageBox::Yes==QMessageBox::question(this->m_pMainWnd,"视频聊天",
    INet::getIpstring(lsendIp).c_str(),QMessageBox::Yes|QMessageBox::No,QMessageBox::No)){
        struct STRU_UDP_voice rp;
        rp.nType=_DEF_UDP_voice_RS;
        m_pMediator->SendData(lsendIp,(char*)&rp,sizeof(rp));
        if(m_mapIpToChatDlg.find(lsendIp)!=m_mapIpToChatDlg.end()){
            chatdialog*chat=m_mapIpToChatDlg[lsendIp];
            if(chat->Myvoice==nullptr){
               chat->Myvoice=new Voice;
            cout<<"kkkk"<<endl;
 connect(chat->Myvoice,SIGNAL(fasong(long,char*,int)),this,SLOT(slot_fasongvide (long,char*,int)));
               chat->Myvoice->show();
               chat->Myvoice->openaudio();
               chat->Myvoice->m_ip=lsendIp;
            }

        }
    }
}
 //处理语音通话回复
void CKernel::slot_voicechatRs(long lsendIp, char *buf, int nLen)
{
    cout <<__func__<< endl;
    if(m_mapIpToChatDlg.find(lsendIp)!=m_mapIpToChatDlg.end()){
        chatdialog*chat=m_mapIpToChatDlg[lsendIp];
        if(chat->Myvoice==nullptr){
           chat->Myvoice=new Voice;
 connect(chat->Myvideo,SIGNAL(fasong(long,char*,int)),this,SLOT(slot_fasongvide (long,char*,int)));
            cout<<"kkkk"<<endl;
           chat->Myvoice->show();
           chat->Myvoice->openaudio();



           chat->Myvoice->m_ip=lsendIp;
        }
    }
}
void CKernel::Slot_readyData(long lSendlp, char *buf, int nLen)
{

    //cout <<__func__<< endl;
    // 1、取出协议头―
    int nType = *(int*)buf;
    //cout<<__func__<<":"<<nType<<endl;
    // 2、根据协议头判断走哪个处理函数
    switch (nType){
        case _DEF_UDP_ONLINE_RQ:
            slot_dealOnlineRq ( lSendlp, buf, nLen) ;break;
        case _DEF_UDP_ONLINE_RS:
            slot_dealonlineRs ( lSendlp, buf, nLen) ;break;
        case _DEF_UDP_OFFLINE_RQ:
            slot_dealofflineRq ( lSendlp, buf, nLen) ;break;
        case _DEF_UDP_CHAT_RQ:
            slot_dealChatRq( lSendlp, buf, nLen) ;break;
        case _DEF_UDP_VIDEO_RQ:
            slot_dealVideRq( lSendlp, buf, nLen) ;break;
        case _DEF_UDP_VIDEO_RS:
            slot_dealVideRs( lSendlp, buf, nLen) ;break;
        case _DEF_UDP_VIDEO_CHAT:
            slot_dealVidechat( lSendlp, buf, nLen) ;break;
        case _DEF_UDP_AUDIO_CHAT:
            slot_dealaudiochat( lSendlp, buf, nLen) ;break;
        case _DEF_UDP_OFFVIDEO_CHAT:
            slot_videoclose(lSendlp);break;
        case  _DEF_UDP_voice_CHAT:
            slot_voicechat(lSendlp, buf, nLen);break;
        case _DEF_UDP_voice_RQ:
            slot_voicechatRp(lSendlp, buf, nLen);break;
        case _DEF_UDP_voice_RS:
            slot_voicechatRs(lSendlp, buf, nLen);break;
        default:
            break;
        delete[]buf;
    }

}
//处理用户点击好友列表的，某一行槽函数
void CKernel::slot_userClicked(QString ip)
{
    cout<<__func__<<endl;
    //判断map里面是否有这个IP
    long IP=inet_addr(ip.toStdString().c_str());
    if(m_mapIpToChatDlg.find(IP)!=m_mapIpToChatDlg.end()){
        m_mapIpToChatDlg[IP]->show();
    }

}
//处理用户输入聊天内容
void CKernel::slot_sendChatMsg(QString ip,QString content)
{
        cout<<__func__<<endl;
        //组装包
        STRU_UDP_CHAT_RQ rp;
        strcpy(rp.content,content.toStdString().c_str());
        //2.发包
        m_pMediator->SendData(inet_addr(ip.toStdString().c_str()),(char*)&rp,sizeof(rp));
}

void CKernel::slot_close()
{
    cout<<__func__<<endl;
    //发送离线请求
    STRU_UDP_OFFLINE rp;
    m_pMediator->SendData(INADDR_BROADCAST,(char*)&rp,sizeof(rp));

        //回收资源
        //回收好友列表窗口，先隐藏窗口，再销毁对象
        if(m_pMainWnd){
            m_pMainWnd->hide();
            delete m_pMainWnd;
            m_pMainWnd = nullptr;
        }
        //回收中介者类指针，先关闭网络，再销毁对象
        if(m_pMediator){
            m_pMediator->CloseNet();
            delete m_pMediator;
            m_pMediator = nullptr;
        }
        for(auto ite=m_mapIpToChatDlg.begin();ite!=m_mapIpToChatDlg.end();){
            chatdialog*chat=ite->second;
            if(chat){
                chat->hide();
                delete chat;
                chat=nullptr;
            }
            ite=m_mapIpToChatDlg.erase(ite);
        }
        //关闭进程
        exit(0);

}
//发送视频聊天请求
void CKernel::slot_viderq(long ip)
{
        struct STRU_UDP_VIDEO rp;
        m_pMediator->SendData(ip,(char*)&rp,sizeof(rp));

}
//发送语音tong聊天请求
void CKernel::slot_voice(long ip)
{
    struct STRU_UDP_voice rp;
     m_pMediator->SendData(ip,(char*)&rp,sizeof(rp));
}
//处理video发出的数据需要ckernel传出去
void CKernel::slot_fasongvide(long ip,char *p,int len)
{
    m_pMediator->SendData(ip,p,len);
}
//关闭视频聊天窗口
void CKernel::slot_videoclose(long ip)
{
     chatdialog*chat=m_mapIpToChatDlg[ip];
     if(chat->Myvideo!=nullptr){
         chat->Myvideo->cleardata();
         delete chat->Myvideo;
         chat->Myvideo=nullptr;
         struct STRU_UDP_OFFVIDEO rp;
        m_pMediator->SendData(INADDR_BROADCAST,(char*)&rp,sizeof(rp));
     }
}

