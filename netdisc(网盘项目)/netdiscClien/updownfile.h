#ifndef UPDOWNFILE_H
#define UPDOWNFILE_H


#include <QDialog>
#include <QCloseEvent>
#include<QMessageBox>
namespace Ui {
class upDownfile;
}

class upDownfile : public QDialog
{
    Q_OBJECT

public:
    explicit upDownfile(QWidget *parent = nullptr);
    ~upDownfile();
    //设置用户信息
    void setInfo(QString name,int );
    //重写关闭事件
    void closeEvent(QCloseEvent *event);
public slots:
    //设置进度
    void slot_setprogressbar(int,int);
private:
    Ui::upDownfile *ui;
    QString filename;
};

#endif // UPDOWNFILE_H
