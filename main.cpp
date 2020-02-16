#include "mainwindow.h"
#include <QApplication>
#include<QTextCodec>
#include<QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile qss("://emailui.qss");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());

    qss.close();
//    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));//显示中文
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    MainWindow w;
    w.show();
    
    return a.exec();
}
