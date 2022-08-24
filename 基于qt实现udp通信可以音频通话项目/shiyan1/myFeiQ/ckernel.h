#ifndef CKERNEL_H
#define CKERNEL_H
#include"feiqdialog.h"
//#include"INetMediator.h"
#include"UdpMediator.h"
#include <QObject>
#include"chatdialog.h"
#include<map>
#include<set>
#include"video.h"
#include"voice.h"
class CKernel : public QObject
{
    Q_OBJECT

public:
    explicit CKernel(QObject *parent = 0);
    ~CKernel();
    //~CKernel();
signals:

public slots:
    //处理所有接收到的数据
    void Slot_readyData(long lSendlp,char* buf,int nLen);
    //处理用户点击好友列表某一行的槽函数
    void slot_userClicked (QString ip);
    //处理用户输入的聊天内容
    void slot_sendChatMsg (QString ip,QString content) ;
    //处理关闭好友列表窗口的信号的槽函数
    void slot_close();
    //发送视频聊天请求
    void slot_viderq (long ip);
    //发送语音tong聊天请求
    void slot_voice (long ip);
    //处理video发出的数据需要ckernel传出去
    void slot_fasongvide (long ip,char*p,int nlen);
    //关闭视频窗口槽函数
    void slot_videoclose (long ip);
private:
    //处理上线请求
    void slot_dealOnlineRq (long lsendIp,char* buf,int nLen) ;
    //处理上线回复
    void slot_dealonlineRs (long lSendIp,char* buf, int nLen) ;
    //处理下线请求
    void slot_dealofflineRq(long lsendIp,char* buf, int nLen);
    //处理聊天请求
    void slot_dealChatRq (long lsendIp, char* buf, int nLen);
    //处理视频请求
    void slot_dealVideRq (long lsendIp, char* buf, int nLen);
    //视频回复
    void slot_dealVideRs (long lsendIp, char* buf, int nLen);
    //处理视频聊天音频
    void slot_dealaudiochat(long lsendIp, char* buf, int nLen);
    //处理视频聊天
    void slot_dealVidechat (long lsendIp, char* buf, int nLen);
    //处理语音通话
    void slot_voicechat(long lsendIp, char* buf, int nLen);
    //处理语音通话请求
    void slot_voicechatRp(long lsendIp, char* buf, int nLen);
    //处理语音通话回复
    void slot_voicechatRs(long lsendIp, char* buf, int nLen);
private:
    //好友列表窗口
    FeiQDialog*m_pMainWnd;
    //中介者类指针
    INetMediator* m_pMediator;
    //管理聊天窗口
    std::map<long,chatdialog*>m_mapIpToChatDlg;

};

#endif // CKERNEL_H


