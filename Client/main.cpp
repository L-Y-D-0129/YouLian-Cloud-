#include "client.h"
#include "index.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //创建单例对象 获取对象 .show() 是显示出来
    client::getInstance().show();

    return a.exec();
}
