#pragma once
#include<iostream>
#include<WinSock2.h>
#include<ws2tcpip.h>
#include<process.h>
#include<set>
using namespace std;
//#include<atlbase.h>
//#pragma comment(lib, "ws2_32.lib")//Qt加载不好使
class INetMediator;//直接声明

class INet {
public:
	INet(){}
	virtual~INet() {}//使用时父类指针指向子类对象，需要使用虚析构防止内存泄漏
	//初始化网络
	virtual bool InitNet() = 0;
	//关闭网络
	virtual void UninitNet() = 0;
	//发送数据
	virtual bool SendData(long iSendIp, char* buf, int nlen) = 0;
    // char*可以直接给std: : string或者Qstring赋值
    //std: : string调用c_str ()转换成char*
    // Qstring调用tostdstring转换成std: : string
    //ip地址转换，long->std: : string
    static std:: string getIpstring ( long ip){
              sockaddr_in addr;
              addr.sin_addr.S_un.S_addr = ip;
              return inet_ntoa (addr.sin_addr);
         }
    //获取本机ip地址列表
    static set<long> getvalidIpList (){
        set<long> setIp;
        // 1、获取主机名称
        char name [ 100]= "";
        gethostname (name, 100);
        //2、根据主机名称获取ip地址列表
        struct hostent *remoteHost = gethostbyname (name) ;
        int i = 0 ;
        while (remoteHost->h_addr_list[i] != 0){
        setIp.insert(*(u_long*) remoteHost->h_addr_list [i++] );
    }
    return setIp;
}

protected:
	//接收数据
	virtual void RecData() = 0;
	INetMediator* m_pMediator;//中介者指针，用于将数据传递给中介者
};
