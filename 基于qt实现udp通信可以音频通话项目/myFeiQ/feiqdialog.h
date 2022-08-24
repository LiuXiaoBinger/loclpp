#ifndef FEIQDIALOG_H
#define FEIQDIALOG_H
#include<QCloseEvent>
#include <QDialog>

namespace Ui {
class FeiQDialog;
}

class FeiQDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FeiQDialog(QWidget *parent = 0);
    ~FeiQDialog();
    //添加好友
    void addFriend (QString ip, QString name) ;
    //删除好友
    void deleteFriend (QString ip);
    //重写关闭事件
    void closeEvent(QCloseEvent*event);
signals:
    //双击好友列表某一行信号
    void SIG_userClicked(QString ip);
    //重写关闭窗口信号
    void SIG_close();
private slots:
    void on_tw_friend_cellDoubleClicked(int row, int column);

private:
    Ui::FeiQDialog *ui;
};

#endif // FEIQDIALOG_H
