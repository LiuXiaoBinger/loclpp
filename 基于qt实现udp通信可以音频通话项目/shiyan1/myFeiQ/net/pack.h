
#pragma once
//#include <vcruntime_string.h>
#include<string.h>
#include<iostream>
#include<QDebug>
#define _DEF_UDP_PORT (43210)
#define _DEF_UDP_PORT_zijie (4096)
#define _DEF_NAME_SIZE (50)
#define _DEF_CONTENT_SIZE (1024)
#include <QByteArray>
//协议头基础
#define _DEF_PROTCAL_BASE  (1000)

//udp协议头
//上线
#define _DEF_UDP_ONLINE_RQ   (_DEF_PROTCAL_BASE+1)
#define _DEF_UDP_ONLINE_RS   (_DEF_PROTCAL_BASE+2)
//下线
#define _DEF_UDP_OFFLINE_RQ   (_DEF_PROTCAL_BASE+3)
//聊天
#define _DEF_UDP_CHAT_RQ   (_DEF_PROTCAL_BASE+4)
//视频聊天请求与回复
#define _DEF_UDP_VIDEO_RQ  (_DEF_PROTCAL_BASE+5)
#define _DEF_UDP_VIDEO_RS  (_DEF_PROTCAL_BASE+6)
//视频聊天
#define _DEF_UDP_VIDEO_CHAT  (_DEF_PROTCAL_BASE+7)
//视频下线
#define _DEF_UDP_OFFVIDEO_CHAT  (_DEF_PROTCAL_BASE+9)
//语音通话
#define _DEF_UDP_voice_CHAT  (_DEF_PROTCAL_BASE+10)
//语音通话请求与回复
#define _DEF_UDP_voice_RQ  (_DEF_PROTCAL_BASE+11)
#define _DEF_UDP_voice_RS  (_DEF_PROTCAL_BASE+12)
//音频聊天
#define _DEF_UDP_AUDIO_CHAT  (_DEF_PROTCAL_BASE+8)
//udp结构体

//上线请求&回复，包含：协议头、name，复用的

struct STRU_UDP_ONLINE{
    STRU_UDP_ONLINE():nType(_DEF_UDP_ONLINE_RQ){
        memset(name,0,_DEF_NAME_SIZE);
    }

        int nType;
    // long ip;
        char name [_DEF_NAME_SIZE];

};
//视频聊天的图片包
struct STRU_UDP_VIDEO_CHAT{
    STRU_UDP_VIDEO_CHAT():nType(_DEF_UDP_VIDEO_CHAT){
        memset (content, 0,50000);
    }
    int nType;
    //len表示实际写入content数组的大小
    int nlen;
    char content[50000];
    //QByteArray str;
};
//视频聊天的语音包
struct STRU_UDP_AUDIO_CHAT{
    STRU_UDP_AUDIO_CHAT():nType(_DEF_UDP_AUDIO_CHAT){
        memset (content, 0,18000);
    }
    int nType;
    //len表示实际写入content数组的大小
    int nlen;
    char content[18000];
    //QByteArray str;
};
//语音通话
struct STRU_UDP_voice_CHAT{
    STRU_UDP_voice_CHAT():nType(_DEF_UDP_voice_CHAT){
        memset (content, 0,18000);
    }
    int nType;
    //len表示实际写入content数组的大小
    int nlen;
    char content[18000];
    //QByteArray str;
};
//下线，包含:协议头
struct STRU_UDP_OFFLINE{
    STRU_UDP_OFFLINE() :nType (_DEF_UDP_OFFLINE_RQ){ }
    int nType;
};
//聊天，包含:协议头、聊天内容
struct STRU_UDP_CHAT_RQ{
    STRU_UDP_CHAT_RQ () :nType (_DEF_UDP_CHAT_RQ){
    memset (content, 0,_DEF_CONTENT_SIZE);
    }
    int nType;
    char content[_DEF_CONTENT_SIZE];
};
//视频聊天请求与回复
struct STRU_UDP_VIDEO{
    STRU_UDP_VIDEO():nType(_DEF_UDP_VIDEO_RQ){
    }
    int nType;
    //char content[_DEF_CONTENT_SIZE];

};
//语音通话请求与回复
struct STRU_UDP_voice{
    STRU_UDP_voice():nType(_DEF_UDP_voice_RQ){
    }
    int nType;
    //char content[_DEF_CONTENT_SIZE];

};
//视频下线包
struct STRU_UDP_OFFVIDEO{
    STRU_UDP_OFFVIDEO():nType(_DEF_UDP_OFFVIDEO_CHAT){
    }
    int nType;
    //char content[_DEF_CONTENT_SIZE];

};
