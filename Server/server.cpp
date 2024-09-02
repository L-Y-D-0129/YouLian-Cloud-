#include "server.h"
#include "ui_server.h"
#include<QDebug>
#include<QFile>
#include"mytcpserver.h"
//检查是否有客户端建立连接
Server::Server(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Server)
{
    ui->setupUi(this);
    //加载配置文件
    loadConfig();
    //进行监听
    MyTcpServer::getInstance().listen(QHostAddress(m_strIP),m_usPort);

}

Server::~Server()
{
    delete ui;
}

void Server::loadConfig()
{
    //创建QFile 类
    //：+ 前缀 加文件名
    QFile file(":/server.config");//固定以冒号开头
    //使用QFile 类操作文件
    if(file.open(QIODevice::ReadOnly)){//提取配置文件的ip 和端口号
        QByteArray baData = file.readAll();
        QString strData = QString(baData);
        qDebug()<<strData;
        QStringList strList = strData.split("\r\n");
         m_strIP = strList.at(0);
         m_usPort = strList.at(1).toUShort();
         m_strRootPath = strList.at(2);
        qDebug()<<"loadConfig :: ip : "<<m_strIP <<"Port "<<m_usPort<<"RootPath "<<m_strRootPath;
        file.close();
    }else{
        qDebug()<<"打开配置失败";
    }
}

//创建服务器的单例模式
Server &Server::getInstance()
{
    static Server  instance;
    return instance;
}

//使用共有方法获取私有成员
QString &Server::getRootPath()
{
    return m_strRootPath;
}


