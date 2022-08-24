#pragma once
#include<iostream>
#include<WinSock2.h>
#include<ws2tcpip.h>
#include"pack.h"
using namespace std;
//#include<atlbase.h>
#include<QObject>
class INet;
class INetMediator:public QObject{
    Q_OBJECT
public:
    INetMediator() ;
    virtual~INetMediator() ;
//开启网络
virtual bool OpenNet()= 0;
//关闭网络
virtual void CloseNet()= 0;
//发送数据
virtual bool SendData(long ISendlp,char * buf, int nLen) = 0;
//处理数据
virtual void DealData(long lSendlp, char* buf,int nLen) = 0; 
protected:
	INet* m_pNet;
};
