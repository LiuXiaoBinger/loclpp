#include "file.h"
#include<QFileDialog>
file::file()
{

}

void file::SendFile()
{
    QString name = QFileDialog::getOpenFileName(this, "1111", "C:\\Users\\", "img (*.png *.jpg)");
}
