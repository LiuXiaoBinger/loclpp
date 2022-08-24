QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
LIBS+= -lws2_32
LIBS += -lpthread libwsock32 libws2_32
LIBS += -lpthread libMswsock libMswsock
#LIBS += -L"C:/Program Files/OpenSSL-Win32/lib" -llibeay32
#LIBS += -L"C:/Program Files/OpenSSL-Win32/lib" -lssleay32
include(./filemodule/filemod.pri)
INCLUDEPATH += $$quote(C:/OpenSSL-Win32/include)
#LIBS += -L"C:\OpenSSL-Win32\lib" -lcrypt
LIBS += -L"C:\OpenSSL-Win32\lib" -llibcrypto
 -lcrypto
SOURCES += \
    ckernel.cpp \
    filepack.cpp \
    logindialog.cpp \
    main.cpp \
    mediator/INetMediator.cpp \
    mediator/TcpClienMediator.cpp \
    netdisclien.cpp \
    threadworker.cpp \
    updownfile.cpp \
    userinterface.cpp

HEADERS += \
    INet.h \
    MD5.h \
    ckernel.h \
    filepack.h \
    logindialog.h \
    mediator/INetMediator.h \
    mediator/TcpClienMediator.h \
    netdisclien.h \
    pack.h \
    threadworker.h \
    updownfile.h \
    userinterface.h

FORMS += \
    filepack.ui \
    logindialog.ui \
    updownfile.ui \
    userinterface.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
