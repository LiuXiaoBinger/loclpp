#include "netdisclien.h"
#include"ckernel.h"
using namespace std;
netdisClien::netdisClien(INetMediator* pMediator)
{
    m_pMediator = pMediator;
}
netdisClien::~netdisClien(){

}

//初始化网络
bool netdisClien::InitNet(){


        //1.做个小买卖 炸串 --加载库
        WORD wVersionRequested;
        WSADATA wsaData;
        int err;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
        wVersionRequested = MAKEWORD(2, 2);

        err = WSAStartup(wVersionRequested, &wsaData);
        if (err != 0) {
            return false;
        }

        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
            WSACleanup();
            return false;

    }
    //2、雇人——创建套接字socket
    //SOCKET sock = INVALID_SOCKET;
    m_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sock == INVALID_SOCKET) {
        std::cout << "sock:错误" << WSAGetLastError() <<  std::endl;
        WSACleanup();
    }
    else {
         std::cout << "sock:成功" <<  std::endl;
    }
    //int err;
    //3、连接服务器
    sockaddr_in addr;
    addr.sin_family = AF_INET ;
    addr.sin_addr.S_un.S_addr = inet_addr( _DEF_TCP_SERVER_IP );  /*inet_addr("192.168.31.115")*/ ;  //绑定任意网卡
    addr.sin_port = htons( _DEF_TCP_PORT );  //htons 转换为网络字节序 大端存储  43232


    if( connect( m_sock ,(const sockaddr* ) &addr , sizeof(addr) ) == SOCKET_ERROR )
    {
        //UnInitNet();
        std::cout << "12bind error : " << WSAGetLastError() <<  std::endl;
        return false;
    }
    //设置客户端发送缓冲区
    int nSendBuf=128*1024;//设置为64K
    setsockopt(m_sock,SOL_SOCKET,SO_SNDBUF,(const char*)&nSendBuf,sizeof(int));
    //设置客户端接收缓冲区
    int nRecvBuf=64*1024;//设置为64K
    setsockopt(m_sock,SOL_SOCKET,SO_RCVBUF,(const char*)&nRecvBuf,sizeof(int));
    //禁用 TCP-NODELAY
    int value = 1;
    setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (char *)&value, sizeof(int));

    //4.TODO:创建一个接收数据的线程
    //使用detach线程结束自己回收
    //std::thread(&netdisClien::RecvThread,this,nullptr).detach();
    _beginthreadex(NULL, 0, &RecvThread, this, 0, 0);
    return true;
}
//关闭网络
void netdisClien::UninitNet(){
    //关闭套接字
    if (m_sock && m_sock != INVALID_SOCKET) {
        closesocket(m_sock);
    }
}
//发送数据
bool netdisClien::SendData(long iSendIp, char* szbuf, int nlen) {
    //1、校验参数
    if (!szbuf || nlen <= 0) {
         std::cout << "parameter eror" <<  std::endl;
        return false;
    }
      iSendIp = m_sock;
       int DataLen = nlen + 4;
       std::vector<char> vecbuf;
       vecbuf.resize( DataLen );

    //   char* buf = new char[ DataLen ];
       char* buf = &*vecbuf.begin();
       char* tmp = buf;
       *(int*) tmp = nlen;
       tmp+= sizeof(int);
        //cout<<"包大小：："<<nlen<<endl;
       memcpy( tmp , szbuf , nlen);

    //3、发包内容
       int err=SOCKET_ERROR;
    if (err==send( iSendIp ,buf,DataLen , 0)){
        std::cout <<err<< "sendto eror:" << WSAGetLastError() << "::"<<DataLen<< std::endl;
        exit(1);
        return false;
    }
     cout<<"发送成功"<<*(int*)szbuf<<endl;
    return true;
}
//使用c++11新特性std：：thread多线程
//接收数据
unsigned int _stdcall netdisClien::RecvThread(void* lpvoid){
    netdisClien* pThis = (netdisClien*)lpvoid;

    pThis->RecvData();
    return -1;
}



 void netdisClien::RecvData(){
     int nlen;
     int packsize;//包大小
     int offset;//偏移量
     while(1){
         //先接受包大小
         nlen=recv(m_sock,(char*)&packsize,sizeof (packsize),0);
         //错误处理如果elen<=0那么网络出现问题
         if(nlen<=0){
             UninitNet();
             break;
         }
         //cout<<"包大小：："<<packsize<<endl;
         //接收包内容
         char*buff=new char [packsize];
         offset=0;
         while(packsize){
             nlen=recv(m_sock,buff+offset,packsize,0);
             if(nlen>0){
                offset+=nlen;
                packsize-=nlen;
             }
         }
         //接收完将数据转给ckenl
         if(m_pMediator)
          m_pMediator->DealData(m_sock, buff, offset);

     }

 }
