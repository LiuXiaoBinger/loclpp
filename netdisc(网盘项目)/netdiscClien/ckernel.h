#ifndef CKERNEL_H
#define CKERNEL_H
#include"netdisclien.h"
#include <QObject>
#include"pack.h"
#include"logindialog.h"
#include"filepack.h"
#include"userinterface.h"
#include"threadworker.h"
#include"mediator/INetMediator.h"
#include"mediator/TcpClienMediator.h"
#include<map>
#include"updownfile.h"
#include<unordered_map>
class myLock {
private:
    CRITICAL_SECTION cs;
public:
    myLock() {
        InitializeCriticalSection(&cs);
    }
    ~myLock() {
        DeleteCriticalSection(&cs);
    }
    void lock() {
        EnterCriticalSection(&cs);
    }
    void unLock() {
        LeaveCriticalSection(&cs);
    }
};
//断点传续包
typedef struct downfileduandian{
   FILE*writefd;
   FILE*breakpiont;
    string filedir;
    string breakpointfile;
}downfileduandian;
//声明文件发送工作者
class SendfileWorker;
class SendreadfileWorker;
class recvwritefileWorker;
//定义函数指针
class Ckernel;
typedef void (Ckernel::*PFUN)(long iSendIp, char* buf, int nlen);
class Ckernel: public QObject
{
    Q_OBJECT
public:

    explicit Ckernel(QObject *parent = nullptr);
    ~Ckernel();
public:
    //将kernel设置成单列者模式
    static Ckernel* GetInstance();
    //使用c++11新的特性，c++多线程与qt多线程
    std::thread wrap_events(void* arg);//对handleEvent包装，然后可以使用C++11的多线程
signals:
    //上传文件发送
    void SIG_Sendfile(char*buf,int nlen);
    void SIG_delupfile( const char*namefile,const char*name);
    //下载文件信号给下载文件工作者
    void SIG_deldownfile( char*name);
    //计算文件上传下载进度，传给下载上传控件
    void SIG_schedule(int ,int);
public slots:
    //设置函数映射
    void function();
    //处理注册请求信号
    void slot_RegisterCommit(QString name,QString tel,QString password);
    //处理登录请求
    void slot_LoginCommit(QString tel,QString password);
    //将用户信息加载到界面
    void slot_delnameInforq(long iSendIp, char *buf, int nlen);
   //处理注册回复
    void dealRedisteRs(long iSendIp, char *buf, int nlen);
    //处理登录回复
    void dealLonginRs(long iSendIp, char *buf, int nlen);
    //处理文件，将数据哭的文件获取到压到用户界面上
    void slot_delfileInforq(long iSendIp, char *buf, int nlen);
    //上传文件发送
    void slot_Sendfile(char*buf,int nlen);
    //将文件属性上传到服务器让服务器，跟数据库检查
    void slot_delupfilerq(const char*,const char*);
    //服务器检查完文件，服务器检查完文件返回是否可以上传
    void slot_delupfilers(const char*,const char*);
    //处理上传文件
    void slot_delupfile(const char*,const char*);
    void delupfile(void* arg);
    //处理接收到的所有数据
    void ReadyData(long iSendIp, char* buf, int nlen);
    //处理文件是否上传成功
    void dealupfileif(long iSendIp, char* buf, int nlen);
    //文件上传开始
    void dealupstart(long iSendIp, char* buf, int nlen);

    //删除文件
    void slot_DELETE(QString m_filename);
    //下载文件
    void slot_DOWNLOCAD(QString m_filename);
    //分享文件
    void slot_SHARE(QString m_filename);
    //处理关闭好友列表页面的信号
    void slot_offline () ;
    //处理关闭登录和注册页面的信号
    void slot_close();
    //下载文件信号给下载文件工作者
    void slot_deldownfile( char*name);
    //处理下载回复包
    void dealdownfilers(long iSendIp, char* buf, int nlen);
    //处理下载完成rs回复包
    void dealdownfilefinishrs(long iSendIp, char* buf, int nlen);

    //处理下载断点传续包
    void dealdownbreakpoint(long iSendIp, char* buf, int nlen);
    //处理上传断点传续包
    void dealupbreakpoint(long iSendIp, char* buf, int nlen);
private:
    int m_id;
    QString m_name;
    INetMediator*m_tcpClient;;
    LoginDialog* login;
    UserInterface* m_mainWnd;
    QSharedPointer<SendfileWorker> m_pSendWorker;
    QSharedPointer<SendreadfileWorker> m_preadfileWorker;
    QSharedPointer<recvwritefileWorker> m_pwritefileWorker;
    //协议映射表数据，协议头->函数地址
    PFUN m_netProtocalMap[100];
    std::map<int,const char*>kk;
    const char*yer[5];
    const char*der[5];
    map<string,string>filemap;
    unordered_map<string,upDownfile*>hasp_filename_upDownfile;
    map<string,downfileduandian*>downFile;

    myLock downlock;
};

class SendfileWorker : public ThreadWorker
{
    Q_OBJECT
public slots:

    void slot_sendfile( char * buf , int nlen ){
        Ckernel::GetInstance()->slot_Sendfile( buf , nlen );
    }
};
class SendreadfileWorker : public ThreadWorker
{
    Q_OBJECT
public slots:
    void slot_delupfile( const char*namefile,const char*name){
        Ckernel::GetInstance()->slot_delupfile(namefile,name);
    }
};
class recvwritefileWorker : public ThreadWorker
{
    Q_OBJECT
public slots:
    void slot_deldownfile(char*name){

        Ckernel::GetInstance()->slot_deldownfile(name);
    }
};
#endif // CKERNEL_H
