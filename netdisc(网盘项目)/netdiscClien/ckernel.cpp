#include "ckernel.h"
#include<iostream>
#include<QDebug>
#include<QMessageBox>
#include"MD5.h"
#include<errno.h>
#include<string.h>
#include"updownfile.h"
using namespace std;
//计算数组下标的宏替换
#define NetPackFunMap(a) m_netProtocalMap[a- _DEF_PROTOCAL_BASE-1]
Ckernel::Ckernel(QObject *parent) : QObject(parent),login(nullptr)
{
    function();
    login =new LoginDialog;
    login->showNormal();
    //绑定注册信息的信号和槽函数
    connect(login,SIGNAL(SIG_RegisterCommit(QString,QString,QString)),
            this,SLOT(slot_RegisterCommit(QString,QString,QString)));
    //绑定登录信息的信号和槽函数
    connect(login,SIGNAL(SIG_LoginCommit(QString,QString)),
            this,SLOT(slot_LoginCommit(QString,QString)));
    //绑定关闭登录窗口的信号和槽函数
    //    connect(login,SIGNAL(SIG_CloseWnd()),
    //            this,SLOT(slot_CloseWnd()));
    //初始化网络
    m_tcpClient=new TcpClientMediator;
    //绑定发送所有接收到的数据的信号和槽函数
    connect (m_tcpClient,SIGNAL (SIG_ReadyData (long, char*,int)) ,
             this, SLOT(ReadyData (long , char* ,int)));
    //打开网络
    if(!m_tcpClient->OpenNet()){
        cout<<"打开网络失败"<<endl;
        exit(0);
    }
    //发送file工作者
    m_pSendWorker= QSharedPointer<SendfileWorker>( new SendfileWorker);
    m_preadfileWorker= QSharedPointer<SendreadfileWorker>( new SendreadfileWorker);
    connect( this , SIGNAL(SIG_Sendfile(char*,int)) ,
             m_pSendWorker.data() , SLOT( slot_sendfile(char*,int)) );
    connect( this , SIGNAL(SIG_delupfile(const char*,const char*)) ,
             m_preadfileWorker.data() , SLOT( slot_delupfile(const char*,const char*)) );
    m_pwritefileWorker=QSharedPointer<recvwritefileWorker>( new recvwritefileWorker);
    connect( this , SIGNAL(SIG_deldownfile(char*)) ,
             m_pwritefileWorker.data() , SLOT( slot_deldownfile(char*)) );
    //用户界面
    m_mainWnd=new UserInterface;
    //绑定要上传文件路径给kernel
    connect(m_mainWnd,SIGNAL(SIG_uploadfile(const char*,const char*)),this,SLOT(slot_delupfilerq(const char*,const char*)));

    connect(m_mainWnd,SIGNAL(SIG_close()),this,SLOT(slot_offline () ));
}
//处理关闭登录和注册页面的信号
void Ckernel::slot_close (){
    //    // 1、登录&注册界面指针
    //    if (login){
    //        login->hide ( ) ;
    //        delete login;
    //        login = NULL;
    //    }
    //    //2、中介者类指针
    //    if (m_tcpClient){
    //        m_tcpClient->CloseNet ();
    //        delete m_tcpClient;
    //        m_tcpClient = NULL;
    //    }
    //    //3、好友列表窗口指针
    //    if(m_mainWnd){
    //        m_mainWnd->hide () ;
    //        delete m_mainWnd;
    //        m_mainWnd = NULL;
    //    }
    //    //4、聊天窗口指针
    ////    for (auto ite = m_mapIdToChatdlg.begin (); ite != m_mapIdToChatdlg.end();){
    ////        chatdialog*chat = *ite;
    ////        if (chat){
    ////            chat->hide();
    ////            delete chat;
    ////            chat = NULL;
    ////        }
    ////        ite = m_mapIdToChatdlg.erase (ite) ;
    ////    }
    //    //5、退出应用程序
    exit (0);
}
//处理关闭好友列表页面的信号
void Ckernel::slot_offline(){
    //    //1、给服务端发送下线请求
    //    STRU_TCP_OFFLINE_RQ rq;
    //    rq.userld = m_id;
    //    m_tcpClient->SendData(0,(char* ) &rq, sizeof(rq)) ;
    //    // 2、回收资源
    slot_close ();
}
//处理接收到的所有数据
void Ckernel::ReadyData(long iSendIp, char* buf, int nlen){
    //qDebug()<<__func__<<":"<<buf;
    //1、去除协议头
    int nType=*(int*)buf;
    //2、根据协议头计算出数组中对应的处理函数
    //cout<<nType<<endl;
    if(nType>=_DEF_PROTOCAL_BASE+1&&nType<=_DEF_PROTOCAL_BASE+100){
        //计算出数组下标，取出下标中存的函数指针
        PFUN pf=NetPackFunMap(nType);
        if(pf){
            (this->*pf)( iSendIp,  buf,  nlen);
        }
        //delete[]buf;
    }
}
Ckernel::~Ckernel()
{

}
//将kernel设置成单列者模式
Ckernel* Ckernel::GetInstance(){
    static Ckernel*kernel=new Ckernel;
    return kernel;
}
//设置函数映射
void Ckernel::function(){
    //初始化
    memset(m_netProtocalMap,0,100);
    //绑定协议头和处理函数
    NetPackFunMap(_DEF_PACK_TCP_REGISTER_RS)=&Ckernel::dealRedisteRs;
    NetPackFunMap(_DEF_PACK_TCP_LOGIN_RS)=&Ckernel::dealLonginRs;


    NetPackFunMap(_DEF_PACK_UP_FILESUESS)=&Ckernel::dealupfileif;
    NetPackFunMap(_DEF_PACK_START_UPFILE)=&Ckernel::dealupstart;
    NetPackFunMap(_DEF_PACK_SHOW_FILE)=&Ckernel::slot_delfileInforq;
    NetPackFunMap(_DEF_PACK_DOWN_FILERS)=&Ckernel::dealdownfilers;
    NetPackFunMap(_DEF_PACK_DOWN_FILEFinish)=&Ckernel::dealdownfilefinishrs;
    NetPackFunMap(_DEF_PACK_FUWU_DOWNBREAKPOINT)=&Ckernel::dealdownbreakpoint;
    //    NetPackFunMap(_DEF_PACK_TCP_REGISTER_RS)=&Ckernel::dealRedisteRs;
    //    NetPackFunMap(_DEF_PACK_TCP_LOGIN_RS)=&Ckernel::dealLonginRs;

}

//注册请求发到服务器
void Ckernel::slot_RegisterCommit(QString name,QString tel,QString password){
    cout<<__func__<<endl;
    //数据格式转换
    std::string nameStr=name.toStdString();
    std::string telstr=tel.toStdString();
    std::string passwordstr=password.toStdString();
    //封包
    STRU_TCP_REGISTER_RQ rp;

    strcpy(rp.name,nameStr.c_str());
    strcpy(rp.tel,telstr.c_str());
    strcpy(rp.password,passwordstr.c_str());
    //数据发给服务器
    m_tcpClient->SendData(0,(char*)&rp,sizeof(rp));


}
//处理注册回复
void Ckernel::dealRedisteRs(long iSendIp, char *buf, int nlen)
{
    //cout<<__func__<<endl;
    //拆包
    STRU_TGP_REGISTER_RS* rs = (STRU_TGP_REGISTER_RS* ) buf;//根据注册结果显示提示信息
    switch (rs->result){
    case parameter_error:
        //QMessageBox::about(login,"提示","注册失败，输入信息有误");
        cout<<"提示,注册失败，输入信息有误"<<endl;
        break;
    case user_is_exist :
        //QMessageBox :: about (login,"提示","注册失败，用户己存在");
        cout<<"提示,注册失败，用户己存在"<<endl;
        break;
    case register_success:
        // QMessageBox ::about (login,"提示","注册成功");
        cout<<"提示,注册成功"<<endl;
        break;
    default:
        break;
    }
    cout<<__func__<<endl;
    delete[]buf;
}
//登录请求
void Ckernel::slot_LoginCommit(QString tel,QString password){
    cout<<__func__<<endl;
    //数据格式转换

    std::string telstr=tel.toStdString();
    std::string passwordstr=password.toStdString();
    //封包
    STRU_TCP_LOGIN_RQ rp;

    strcpy(rp.tel,telstr.c_str());
    strcpy(rp.password,passwordstr.c_str());
    //数据发给服务器
    m_tcpClient->SendData(0,(char*)&rp,sizeof(rp));
    //测试，将文件属性压入
    //    for(int i=0;i<100;i++){
    //        filepack*pp =new filepack;
    //        pp->setInfo("fate天之杯.MP4","大小4g，时间2021.2.3");
    //        m_mainWnd->addFile(pp);
    //    }
    //    m_mainWnd->show();
}
//处理登录回复
void Ckernel::dealLonginRs(long iSendIp, char *buf, int nlen)
{
    cout<<__func__<<endl;

    //拆包
    STRU_TCP_LOGIN_RS* rs = (STRU_TCP_LOGIN_RS* ) buf;
    cout<<"你好i"<<endl;
    //根据注册结果显示提示信息
    switch (rs->result){
    case parameter_error:
        //QMessageBox::about(login,"提示","登录失败，输入信息有误1");
        cout<<"提示,登录失败，输入信息有误1"<<endl;
        break;
    case user_not_exist :
        //QMessageBox :: about (login,"提示","登录失败，用户不存在2");
        cout<<"提示,登录失败，用户不存在2"<<endl;
        break;
    case password_error :
        //QMessageBox :: about (login,"提示","登录失败，密码错误3");
        cout<<"提示,登录失败，密码错误3"<<endl;
        break;
    case register_success:
        //QMessageBox ::about (m_loginDia,"提示","登录成功3");break;
        //1.隐藏登录界面
        login->hide();
        //2、显示好友列表
        m_mainWnd->showNormal();
        //保存用户id
        m_id=rs->userld;
        //cout<<"abdhjsdbfgjhsdgbfjksgfkusdgfbkdsuhgfdkshgf"<<m_id<<endl;
        break;
    }
    delete[]buf;
}

//将自己的用户信息加到界面
void Ckernel::slot_delnameInforq(long iSendIp, char *buf, int nlen){
    //1、拆包
    STRU_TCP_FRIEND_INFO* info = (STRU_TCP_FRIEND_INFO* ) buf;
    QString nameTemp=info->name;
    QString feelingTemp=info->feeling;
    if (info->userld == m_id){
        //2.1、更新用户界面
        m_mainWnd->setInfo (nameTemp,feelingTemp,info->iconld);
        //2.2保存自己名字
        m_name=nameTemp;
    }
}
//处理文件，将数据哭的文件获取到压到用户界面上
void Ckernel::slot_delfileInforq(long iSendIp, char *buf, int nlen){
    SHOW_FILE *file=(SHOW_FILE*)buf;
    cout<<__func__<<file->filename<<endl;
    //    //删除文件
    //     void SIG_DELETE(QString m_filename);
    //     //下载文件
    //     void SIG_DOWNLOCAD(QString m_filename);
    //     //分享文件
    //     void SIG_SHARE(QString m_filename);

    filepack*pp =new filepack;
    connect(pp,SIGNAL(SIG_DELETE(QString)),this,SLOT(slot_DELETE(QString)));
    connect(pp,SIGNAL(SIG_DOWNLOCAD(QString)),this,SLOT(slot_DOWNLOCAD(QString)));
    connect(pp,SIGNAL(SIG_SHARE(QString)),this,SLOT(slot_SHARE(QString)));
    pp->m_filename=file->filename;
    cout<<file->filename<<endl;
    pp->setInfo(file->filename,"大小4g，时间2021.2.3");
    m_mainWnd->addFile(pp);
}
//将文件属性上传到服务器让服务器，跟数据库检查
void Ckernel::slot_delupfilerq(const char*namefile,const char*name){
    filemap[name]=namefile;
    cout<<namefile<<endl;
    FILE* readFile = fopen(namefile,"rb");//以只读方式打开文本文件
    if(readFile==NULL){
        cout<<"打开文件出错"<<endl;
        return ;
    }
    //获取文件大小
    fseek(readFile, 0, SEEK_END); //跳到文件尾
    //文件大小
    int FileSize= ftell(readFile); // 64位 要用的 ... _ftelli64  32为 ftell
    fseek(readFile, 0, SEEK_SET); //跳到文件头

    //时间获取
    SYSTEMTIME tv;
    GetLocalTime(&tv);
    cout<<"文件大小"<<FileSize<<endl;
    UP_FILEAttributes filear;
    filear.userld=m_id;
    memcpy(filear.dir,namefile,strlen(namefile)+1);
    filear.datasize=FileSize;
    memcpy(filear.filename,name,strlen(name)+1);
    //memcpy(filear.clienfiedir,name,strlen(name)+1);
    sprintf(filear.szFileID, "%s_%02d_%02d_%02d_%d", name
            , tv.wHour, tv.wMinute, tv.wSecond, tv.wMilliseconds);
    string md5;
    int ret=get_file_md5(namefile,md5);
    if(ret==-1){
        printf("md5生成出错");
    }
    memcpy(filear.md5,md5.c_str(),md5.size());
    //保存进度条窗口
    upDownfile*progressbar=new upDownfile;
    hasp_filename_upDownfile[name]=progressbar;
    connect(this,SIGNAL(SIG_schedule(int,int)),progressbar,SLOT(slot_setprogressbar(int,int)));
    progressbar->setInfo(name,0);
    progressbar->show();

    m_tcpClient->SendData(0,(char*)&filear,sizeof(filear));
    fclose(readFile);

}
//服务器检查完文件，服务器检查完文件返回是否可以上传
void Ckernel::slot_delupfilers(const char*,const char*){

}
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
//处理上传文件
void Ckernel::slot_delupfile(const char*namefile,const char*name){
    cout<<__func__<<namefile<<endl;
    FILE* readFile = fopen(filemap[name].c_str(),"rb");//以只读方式打开文本文件
    if(readFile==NULL){
        int errNum = errno;
        printf("open fail errno = %d reason = %s \n", errNum, strerror(errNum));
        cout<<"打开文件出错"<<endl;
        cout<<namefile<<endl;
        return ;
    }
    //获取文件大小
    fseek(readFile, 0, SEEK_END); //跳到文件尾
    //文件大小
    int FileSize= ftell(readFile); // 64位 要用的 ... _ftelli64  32为 ftell
    fseek(readFile, 0, SEEK_SET); //跳到文件头
    long long nPos=0;
    //时间获取
    SYSTEMTIME tv;
    GetLocalTime(&tv);
    cout<<"文件大小"<<FileSize<<"    "<<endl;
    //    UP_FILEAttributes filear;
    //    filear.datasize=FileSize;
    //    memcpy(filear.filename,name,strlen(name));
    //    sprintf(filear.szFileID, "%s_%02d_%02d_%02d_%d", name
    //        , tv.wHour, tv.wMinute, tv.wSecond, tv.wMilliseconds);
    //myclien->SendData(0,(char*)&filear,sizeof(filear));
    string filename=name;
    //filename.c
    while(1){
        UP_FILERQ rq;

        //rq.filesize=FileSize;
        memcpy(rq.filename,filename.c_str(),filename.size()+1);
        //=const_cast<char*>(filename);
        int nlen = fread(rq.file ,1, 16*1024, readFile);
        rq.userld=m_id;
        rq.filelen=nlen;

        rq.filesize=FileSize;
        //发送给发送线程信号
        //cout<<"读取成功"<<endl;

        rq.pos=nPos;
        // Q_EMIT SIG_Sendfile((char*)&rq,sizeof(rq));
        m_tcpClient->SendData(0,(char*)&rq,sizeof(rq));
        nPos += nlen;
        //            if(nPos%(32*1024)==0){
        //                Sleep(500);
        //            }
        cout<<"文件包大小"<<sizeof(rq);
        //触发结束条件退出
        if (nPos >= FileSize /*|| nlen < _DEF_FILE_CONTENT_SIZE*/){
            fclose(readFile);
            break;
        }
        cout<<rq.filelen<<"  "<<rq.filename<<"  "<<rq.pos<<endl;
    }
    Finish_UPFILE rqf;
    memcpy(rqf.filename,filename.c_str(),filename.size()+1);
    rqf.userld=m_id;
    //Q_EMIT SIG_Sendfile((char*)&rqf,sizeof(rqf));
    m_tcpClient->SendData(0,(char*)&rqf,sizeof(rqf));
    filemap.erase(filename.c_str());
    fclose(readFile);
    //
}

//上传文件发送
void Ckernel::slot_Sendfile(char*buf,int nlen){
    //UP_FILERQ *rq=(UP_FILERQ*)buf;
    m_tcpClient->SendData(0,buf,nlen);

}
//处理文件是否上传成功
void Ckernel::dealupfileif(long iSendIp, char* buf, int nlen){
    //拆包
    STRU_TCP_LOGIN_RS* rs = (STRU_TCP_LOGIN_RS* ) buf;
    //根据注册结果显示提示信息
    switch (rs->result){
    case 0:
        QMessageBox::about(m_mainWnd,"提示","上传失败文件名重复");
        //cout<<"提示,登录失败，输入信息有误1"<<endl;
        break;
    case 1 :
        QMessageBox :: about (m_mainWnd,"提示","上传成功");
        //cout<<"提示,登录失败，用户不存在2"<<endl;
        break;
    }
    delete[]buf;
}
void Ckernel::dealupstart(long iSendIp, char* buf, int nlen){
    cout<<__func__<<endl;
    START_UPFILE *rs=(START_UPFILE*)buf;
    char *fileName=new char[strlen(rs->filename)];
    wrap_events(fileName).detach();
    // Q_EMIT SIG_delupfile(rs->dir,rs->filename);
    delete[]buf;

}
std::thread Ckernel::wrap_events(void* arg){
    return std::thread(&Ckernel::delupfile,this,arg);
}
//处理上传文件
void Ckernel::delupfile(void* arg){
    char*name=(char*)arg;
    cout<<__func__<<name<<endl;
    FILE* readFile = fopen(filemap[name].c_str(),"rb");//以只读方式打开文本文件
    if(readFile==NULL){
        int errNum = errno;
        printf("open fail errno = %d reason = %s \n", errNum, strerror(errNum));
        cout<<"打开文件出错"<<endl;
        cout<<name<<endl;
        return ;
    }
    //获取文件大小
    fseek(readFile, 0, SEEK_END); //跳到文件尾
    //文件大小
    int FileSize= ftell(readFile); // 64位 要用的 ... _ftelli64  32为 ftell
    fseek(readFile, 0, SEEK_SET); //跳到文件头
    long long nPos=0;
    //时间获取
    SYSTEMTIME tv;
    GetLocalTime(&tv);
    cout<<"文件大小"<<FileSize<<"    "<<endl;
    //    UP_FILEAttributes filear;
    //    filear.datasize=FileSize;
    //    memcpy(filear.filename,name,strlen(name));
    //    sprintf(filear.szFileID, "%s_%02d_%02d_%02d_%d", name
    //        , tv.wHour, tv.wMinute, tv.wSecond, tv.wMilliseconds);
    //myclien->SendData(0,(char*)&filear,sizeof(filear));
    string filename=name;
    //filename.c
    while(1){
        UP_FILERQ rq;

        //rq.filesize=FileSize;
        memcpy(rq.filename,filename.c_str(),filename.size()+1);
        //=const_cast<char*>(filename);
        int nlen = fread(rq.file ,1, 16*1024, readFile);
        rq.userld=m_id;
        rq.filelen=nlen;

        rq.filesize=FileSize;
        //发送给发送线程信号
        //cout<<"读取成功"<<endl;

        rq.pos=nPos;
        // Q_EMIT SIG_Sendfile((char*)&rq,sizeof(rq));
        m_tcpClient->SendData(0,(char*)&rq,sizeof(rq));

        nPos += nlen;
        Q_EMIT SIG_schedule(FileSize,nPos);
        //            if(nPos%(32*1024)==0){
        //                Sleep(500);
        //            }
        cout<<"文件包大小"<<sizeof(rq);
        //触发结束条件退出
        if (nPos >= FileSize /*|| nlen < _DEF_FILE_CONTENT_SIZE*/){
            fclose(readFile);
            break;
        }
        cout<<rq.filelen<<"  "<<rq.filename<<"  "<<rq.pos<<endl;
    }

    Finish_UPFILE rqf;
    memcpy(rqf.filename,filename.c_str(),filename.size()+1);
    rqf.userld=m_id;
    //Q_EMIT SIG_Sendfile((char*)&rqf,sizeof(rqf));
    m_tcpClient->SendData(0,(char*)&rqf,sizeof(rqf));
    filemap.erase(filename.c_str());
    fclose(readFile);
    delete[]name;
    //
}
//删除文件
void Ckernel::slot_DELETE(QString m_filename){

}
#include <string.h>
#include<cstring>
//下载文件  QString name = QFileDialog::getOpenFileName(this, "1111", "C:\\Users\\");//"Images (*.png *.xpm *.jpg);Text files (*.txt);;XML files (*.xml)");
void Ckernel::slot_DOWNLOCAD(QString m_filename){
    cout<<__func__<<endl;
    QString fileName = QFileDialog::getSaveFileName(m_mainWnd,
                                                    tr("保存的文件"),
                                                    tr(m_filename.toStdString().c_str()),
                                                    tr("Items files(*.items)"));
    //fileName;
    DOWN_FILERQ rq;
    //将文件地址传给服务器方便断点传续
    memcpy(rq.filedir,fileName.toStdString().c_str(),fileName.toStdString().size());
    FILE* wrFile;
    fopen_s(&wrFile,fileName.toStdString().c_str(),"wb");
    //fseek(wrFile,0,SEEK_SET);
    // string filekkk(fileName.toStdString());
    //char*p=strtok_s(fileName.toStdString().c_str(),".");
    QStringList list=(fileName.split('.'));
    //filekkk
    FILE* breakpointfile;
    cout<<"断点传续文件"<<list[0].toStdString().c_str()<<endl;
    fopen_s(&breakpointfile,list[0].toStdString().c_str(),"wb");
    downfileduandian* breakpointpack=new downfileduandian;
    breakpointpack->writefd=wrFile;
    breakpointpack->breakpiont=breakpointfile;
    downFile[m_filename.toStdString().c_str()]=breakpointpack;

    //保存进度条窗口
    upDownfile*progressbar=new upDownfile;
    hasp_filename_upDownfile[m_filename.toStdString().c_str()]=progressbar;
    connect(this,SIGNAL(SIG_schedule(int,int)),progressbar,SLOT(slot_setprogressbar(int,int)));
    progressbar->setInfo(m_filename,1);
    progressbar->show();
    rq.pos=0;
    //将断点传续文件目录地址上传到服务器
    memcpy(rq.breakpointfile,list[0].toStdString().c_str(),list[0].toStdString().size());
    memcpy(rq.filename,m_filename.toStdString().c_str(),m_filename.toStdString().size());
    rq.userld=m_id;
    m_tcpClient->SendData(0,(char*)&rq,sizeof(rq));
}
//分享文件
void Ckernel::slot_SHARE(QString m_filename){

}
//下载文件信号给下载文件工作者
void Ckernel::slot_deldownfile(char*buf){
    //cout<<__func__<<endl;

    DOWN_FILERS*buff=(DOWN_FILERS*)buf;
    //cout<<buff->filename<<"pos::"<<buff->pos<<endl;
    if(downFile[buff->filename]){
        FILE*wrFile=downFile[buff->filename]->writefd;
        FILE*breakpoint=downFile[buff->filename]->breakpiont;
        downlock.lock();
        //ftruncate(wrFile,buff->filesize);
        //fseek(wrFile,buff->pos,SEEK_SET);

        int res=fwrite(buff->file ,1, buff->filelen, wrFile);
        ferror(wrFile) ;
        //cout<<"pack::"<<buff->file<<endl;
        //断点文件写入
        fseek(breakpoint,0,SEEK_SET);
        fwrite((char*)&buff->pos ,1, 4, breakpoint);
        Q_EMIT SIG_schedule(buff->filesize,buff->pos+buff->filelen);
        downlock.unLock();
         cout<<"写入"<<res<<"pos::"<<buff->pos<<endl;
    }
    //cout<<"kongkongssssssssssssss"<<endl;50118656
    delete[]buf;
}
//处理下载回复包
void Ckernel::dealdownfilers(long iSendIp, char* buf, int nlen){
    //cout<<__func__<<endl;

    Q_EMIT SIG_deldownfile(buf);
}
//处理下载完成rs回复包
#include <stdio.h>
void Ckernel::dealdownfilefinishrs(long iSendIp, char* buf, int nlen){
    cout<<__func__<<endl;
    DOWN_FILEFinish*buff=(DOWN_FILEFinish*)buf;
    cout<<buff->buff<<endl;
    hasp_filename_upDownfile.erase(buff->filename);
    //fseek(downFile[buff->filename]->writefd,0,SEEK_SET);
    fclose(downFile[buff->filename]->writefd);
    fclose(downFile[buff->filename]->breakpiont);
    remove(downFile[buff->filename]->breakpointfile.c_str());
    delete[]buf;
}
//处理下载断点传续包
void Ckernel::dealdownbreakpoint(long iSendIp, char* buf, int nlen){
    FUWU_DOWNBREAKPOINT *buff=(FUWU_DOWNBREAKPOINT*)buf;
    FILE* breakpointfile;
    fopen_s(&breakpointfile,buff->breakpointfile,"rb");
    char filepos[4];
    fread(filepos,1,4,breakpointfile);
    int pos=*(int *)&filepos;
    cout<<"文件断点续传偏移量：：：：："<<pos<<endl;
    DOWN_FILERQ rq;
    FILE* wrFile;
    fopen_s(&wrFile,buff->clienfiledir,"wb");
    if(wrFile){
        cout<<"wrFile不为空"<<endl;
    }
    cout<<buff->clienfiledir<<endl;
    fseek(wrFile,pos,SEEK_SET);
    downfileduandian* breakpointpack=new downfileduandian;
    breakpointpack->writefd=wrFile;
    breakpointpack->breakpiont=breakpointfile;
    downFile[buff->filename]=breakpointpack;

    //保存进度条窗口
    upDownfile*progressbar=new upDownfile;
    hasp_filename_upDownfile[buff->filename]=progressbar;
    connect(this,SIGNAL(SIG_schedule(int,int)),progressbar,SLOT(slot_setprogressbar(int,int)));
    progressbar->setInfo(buff->filename,1);
    progressbar->show();
    rq.pos=pos;
    rq.userld=buff->userld;
    //将断点传续文件目录地址上传到服务器
    memcpy(rq.filedir,buff->clienfiledir,strlen(buff->clienfiledir)+1);
    memcpy(rq.breakpointfile,buff->breakpointfile,strlen(buff->breakpointfile)+1);
    memcpy(rq.filename,buff->filename,strlen(buff->filename)+1);

    m_tcpClient->SendData(0,(char*)&rq,sizeof(rq));
}
//处理上传断点传续包
void Ckernel::dealupbreakpoint(long iSendIp, char* buf, int nlen){

}
