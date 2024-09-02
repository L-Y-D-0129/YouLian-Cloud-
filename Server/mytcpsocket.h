#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include"protocol.h"
#include <QSqlQuery>//使用这个头文件是对数据的怎删改查
#include"messaging.h"
class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    QSqlQuery D_operation;
    //储存登录的用户名字
    QString m_Str_login_name;
    ~MyTcpSocket();
    //读取客户端发送过来的消息
    void recvMsg();
    //读取pdu
    PDU * readPDU();
    //发送pdu 并且 释放pdu
    void sendPUD(PDU * pdu);
    PDU * handlePDU(PDU * pdu);
    Messaging *mess;
    QByteArray buffer;
public slots :
    //处理下线的槽函数
    void Offline_User();
signals:
    void Offline(MyTcpSocket * socket);


};

#endif // MYTCPSOCKET_H
