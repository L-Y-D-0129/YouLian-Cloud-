#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include"mytcpsocket.h"
class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:

    //构建单例模式
    static MyTcpServer & getInstance();
    //有客户端连接  参数链接客户端的句柄
    void incomingConnection(qintptr handle) ;

    //添加用户是要转发给目标用户要在 List 列表中找到目标的socket 发送
    void resend(char * TarName ,PDU * pdu);

private:
   MyTcpServer();
   MyTcpServer(const MyTcpServer & instance) = delete ;
   MyTcpServer & operator  = (const MyTcpServer &) = delete ;
   QList<MyTcpSocket *> m_tcpSocketList; // 储存客户的socket 的链表
public slots:
     void Offline_client(MyTcpSocket * socket);
};

#endif // MYTCPSERVER_H
