#include "filepack.h"
#include "ui_filepack.h"

filepack::filepack(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::filepack)
{
    ui->setupUi(this);
}

filepack::~filepack()
{
    delete ui;
}
//设置文件信息
void filepack::setInfo(QString filename,QString Attributes){
        m_filename=filename;
     ui->filename->setText (filename);
     ui->fileAttributes->setText(Attributes);
}

void filepack::on_fileim_clicked()
{

}

//下载
void filepack::on_pb_download_clicked()
{
    Q_EMIT SIG_DOWNLOCAD(m_filename);
}

//删除
void filepack::on_pb_delete_clicked()
{
    Q_EMIT SIG_DELETE(m_filename);
}

//分享
void filepack::on_pb_share_clicked()
{
    Q_EMIT SIG_SHARE(m_filename);
}

