#ifndef NETDISCLIEN_H
#define NETDISCLIEN_H
#include <iostream>
#include<WinSock2.h>
#include<ws2tcpip.h>
#include<process.h>
#include <thread>
#include"INet.h"
#include"ckernel.h"
class netdisClien :public INet
{
public:
    netdisClien(INetMediator* pMediator);
    ~netdisClien();
    //初始化网络
    bool InitNet();
    //关闭网络
    void UninitNet();
    //发送数据
    bool SendData(long ISendlp, char* buf, int nLen);
    static unsigned int _stdcall RecvThread(void* lpvoid);
    //接收数据
protected:
    //接收数据
   void RecvData();
   // void RecvData();
    SOCKET m_sock;
    HANDLE m_hThreadHandle;
    bool m_isStop;
};

#endif // NETDISCLIEN_H
