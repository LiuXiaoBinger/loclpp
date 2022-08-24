//#include "filemod.h"

//fileMod::fileMod(QObject *parent) : QObject(parent)
//{

//}

////打开文件界面
//void fileMod::solt_openfileinterface(){
////    //文件浏览窗口
////        char FILE_PATH[1024] = { 0 };
////        OPENFILENAME file = { 0 }; //结构体给初值
////        file.lStructSize = sizeof(file); //结构体大小
////        file.lpstrFilter = "*.*";//"文本(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0"; //过滤器
////        file.lpstrFile = FILE_PATH; // open文件的路径存储在这个缓冲区
////        file.nMaxFile = sizeof(FILE_PATH) / sizeof(*FILE_PATH); //缓冲区能存的字符数
////        file.Flags = /*OFN_ALLOWMULTISELECT多选*/ OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;//type

////        bool flag = GetOpenFileNameA(&file);
////        if (flag == false) return;
//}
