#ifndef THREADWORKER_H
#define THREADWORKER_H

#include <QObject>

#include <QObject>
#include <QThread>
#include <QSharedPointer>
//线程类，大文件传输send缓存可能会堵塞而影响主界面流畅性，将发送放到后台进行
class ThreadWorker : public QObject
{
    Q_OBJECT
public:
    explicit ThreadWorker(QObject *parent = nullptr);
    ~ThreadWorker();
signals:

protected:
    QThread *m_pThread;
};


#endif // THREADWORKER_H
