#pragma once
#include"INetMediator.h"
class UdpMediator : public INetMediator {
    Q_OBJECT
public:

	UdpMediator();
	~UdpMediator();
	//开启网络
	bool OpenNet();
	//关闭网络
	void CloseNet();
	//发送数据
	bool SendData(long lSendlp, char* buf, int nLen);
	//处理数据
	void DealData(long lSendlp,char* buf,int nLen);
signals:
    //发送接收到的数据给kernel
    void SIG_readyData(long lSendlp,char* buf,int nLen);
};

