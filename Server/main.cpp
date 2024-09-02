#include "server.h"

#include <QApplication>
#include"operatedb.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Operatedb::getInstance().connectdb();
    Server::getInstance();
   // w.show();
    return a.exec();
}
