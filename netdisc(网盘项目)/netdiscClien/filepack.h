#ifndef FILEPACK_H
#define FILEPACK_H

#include <QDialog>

namespace Ui {
class filepack;
}

class filepack : public QDialog
{
    Q_OBJECT

public:
    explicit filepack(QWidget *parent = nullptr);
    ~filepack();
signals:
    //用户点击好友头像的信号
    void SIG_UserItemClicked(int id);
    //删除文件
     void SIG_DELETE(QString m_filename);
     //下载文件
     void SIG_DOWNLOCAD(QString m_filename);
     //分享文件
     void SIG_SHARE(QString m_filename);
public:
    //设置文件信息
    void setInfo(QString name,QString Attributes);
public:
    QString m_filename;
    int filesize;
private slots:
    void on_fileim_clicked();

    void on_pb_download_clicked();

    void on_pb_delete_clicked();

    void on_pb_share_clicked();

private:
    Ui::filepack *ui;
};

#endif // FILEPACK_H
