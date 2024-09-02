#include "mytcpserver.h"

//用于监听客户端连接用的
MyTcpServer::MyTcpServer()
{

}


//实现单例
MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}
//因为要重写这个函数用于显示客户端连接成功并且保存 socket 的文件描述符
void MyTcpServer::incomingConnection(qintptr handle)
{
       qDebug()<<"新客户端连接";
       //用list 储存新客户端的信息
       //建立mytcpsocket 对象
       MyTcpSocket * qTcpSocket = new MyTcpSocket();
       //设置socket 文件描述符
       qTcpSocket->setSocketDescriptor(handle);
       //将客户端的信息加入存储到socket 的List 中
       m_tcpSocketList.append(qTcpSocket);
       qDebug()<<"连接成功！";
       //与我们自定义的信号进行连接 在mytcpserver中删除列表中的socket
       connect(qTcpSocket,&MyTcpSocket::Offline,this,&MyTcpServer::Offline_client);

}

//转发函数
void MyTcpServer::resend(char *TarName, PDU *pdu)
{
    if(TarName==nullptr||pdu == nullptr){
        return ;
    }

    for(int i = 0;i<m_tcpSocketList.size();i++){
        if(TarName == m_tcpSocketList.at(i)->m_Str_login_name){
            qDebug()<<"找到目标用户用户名为："<<TarName;
            m_tcpSocketList.at(i)->write((char * )pdu , pdu->uiPDULen);
            break;
        }
    }

}

//与我们自定义的信号进行连接 在mytcpserver中删除列表中的socket
void MyTcpServer::Offline_client(MyTcpSocket *socket)
{
    m_tcpSocketList.removeOne(socket);
    socket->deleteLater();
    socket = nullptr;
    //验证退出程序是否正确
//    qDebug()<<"有一名用户退出 当前的用户数量为： "<<m_tcpSocketList.size();
//    for(int i =0;i<m_tcpSocketList.size();i++){
//        qDebug()<<m_tcpSocketList.at(i)->m_Str_login_name;
//    }
}
