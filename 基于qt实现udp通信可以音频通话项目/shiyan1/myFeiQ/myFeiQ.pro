#-------------------------------------------------
#
# Project created by QtCreator 2022-04-23T17:51:49
#
#-------------------------------------------------
QT       += core gui network  multimedia multimediawidgets
CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myFeiQ
TEMPLATE = app

INCLUDEPATH +=./net
INCLUDEPATH +=./mediator
LIBS +=-lws2_32

SOURCES += main.cpp\
    bei.cpp \
        feiqdialog.cpp \
    mylabels.cpp \
    net/UdpNet.cpp \
    mediator/UdpMediator.cpp \
    mediator/INetMediator.cpp \
    chatdialog.cpp \
    ckernel.cpp \
    video.cpp \
    voice.cpp

HEADERS  += feiqdialog.h \
    bei.h \
    mylabels.h \
    net/INet.h \
    net/pack.h \
    net/UdpNet.h \
    mediator/INetMediator.h \
    mediator/UdpMediator.h \
    chatdialog.h \
    ckernel.h \
    video.h \
    voice.h

FORMS    += feiqdialog.ui \
    chatdialog.ui \
    video.ui \
    voice.ui

RESOURCES += \
    res.qrc

DISTFILES += \
    Picture/6.jpg
