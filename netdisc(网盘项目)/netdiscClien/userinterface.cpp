#include "userinterface.h"
#include "ui_userinterface.h"
#include<QMessageBox>
UserInterface::UserInterface(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInterface)
{
    ui->setupUi(this);
    //初始化垂直布局的层
    m_layout=new QVBoxLayout;
    m_layout->setContentsMargins(0,0,0,0);
    //把层设置到控件上
    ui->wdg_list->setLayout(m_layout);
    //给菜单new对象，this就是指父窗口，子控件的回收由父窗口负责
    //m_menu=new QMenu(this);
}

UserInterface::~UserInterface()
{
    delete ui;
}
//重写关闭事件
void UserInterface::closeEvent(QCloseEvent *event){
    //忽略关闭事件，在kernel类中控制窗口关闭
    event->ignore();
    //弹出提示框，重复确定是否关闭
    if(QMessageBox::Yes==QMessageBox::question(this,"提示","是否关闭")){
        Q_EMIT SIG_close();
    }
}
//设置用户信息
void UserInterface::setInfo(QString name ,QString feeling,int iconId){
    ui->id_name->setText(name);
    ui->le_feeling->setText(feeling);
    ui->pb_icon->setIcon(QIcon(QString(":/tx/%1.png").arg(iconId)));
}
//将自己的文件显示到界面上
void UserInterface::addFile(filepack*item){
    m_layout->addWidget(item);
}

void UserInterface::on_pb_upload_clicked()
{
   //QString file_full, file_name, file_path，file_suffix ;

    //QFileInfo fileinfo;
//    file_full = QFileDialog::getOpenFileName(this,.....);
//    fileinfo = QFileInfo(file_full);
//    //文件名
//    file_name = fileinfo.fileName();
//    //文件后缀
//    file_suffix = fileinfo.suffix()
//    //绝对路径
//    file_path = fileinfo.absolutePath();
    QString name = QFileDialog::getOpenFileName(this, "1111", "C:\\Users\\");//"Images (*.png *.xpm *.jpg);Text files (*.txt);;XML files (*.xml)");
   // std::cout<<name.toStdString().c_str()<<std::endl;
    QFileInfo fileinfo;
    fileinfo = QFileInfo(name);
    Q_EMIT  SIG_uploadfile(name.toStdString().c_str(),fileinfo.fileName().toStdString().c_str());
}

