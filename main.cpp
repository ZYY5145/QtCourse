#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 设置一些应用样式（可选）
    a.setStyle("Fusion");

    MainWindow w;
    w.show();
    return a.exec();
}
