#include "mytcpsocket.h"
#include "operatedb.h"
#include<QDebug>
#include<QString>

//因为是每一个客户端发送的信号 每个客户端对应不同的socketfd 所以使用socket 类来接收消息
//通过连接readyRead 来判断是否有消息发送
MyTcpSocket::MyTcpSocket()
{
    mess = new Messaging;
    connect(this,&QTcpSocket::readyRead,this,&MyTcpSocket::recvMsg);
    //连接socket 下线的信号做出处理
    connect(this,&QTcpSocket::disconnected,this,&MyTcpSocket::Offline_User);
}

MyTcpSocket::~MyTcpSocket()
{
    delete mess;
}

//读取客户端发送过来的消息
void MyTcpSocket::recvMsg()
{
    qDebug()<<"\n\n recvMsg 接收到的消息长度:"<<this->bytesAvailable();
    //将数据一次读完 追加到我们的成员变量中 处理半包的方法
    QByteArray data  =this->readAll();
    buffer.append(data);

    //处理粘包
    // buffer 中是否是一个完整的pdu 如果都不是一个完整的pdu 那么一定是半包
    while(buffer.size() >= int(sizeof(PDU)))
    {
        PDU * pdu = (PDU * )buffer.data();
        if(buffer.size() < (int)pdu->uiPDULen){ // 是半包
            break;
        }

        PDU * pdun = mkPDU(pdu->uiMsgLen);
        memcpy(pdun,pdu,pdu->uiPDULen);

        //处理 并发送
        sendPUD(handlePDU(pdun));
        free(pdun);
        pdun = nullptr;

        //清理buffer中处理过的数据
        buffer.remove(0,pdu->uiPDULen);
        qDebug()<<" 发送没有问题 ";
    }


}



//发送信号下线
void MyTcpSocket::Offline_User()
{
    Operatedb::getInstance().clientOffline(m_Str_login_name.toStdString().c_str());
    emit Offline(this);
}


//读取PDU的函数
PDU *MyTcpSocket::readPDU()
{
    //要读取信息要先知道信息的长度 所以我们先读取消息中的第一位也就是总消息长度
    qDebug()<<"\n\n 接收到的消息长度:"<<this->bytesAvailable();
    uint uiPDULen = 0;
    this->read((char *)&uiPDULen,sizeof(uint));
    //计算出实际消息的大小也就是柔性数组的大小
    uint uiMsgLen =  uiPDULen - sizeof(PDU);
    PDU * pdu = mkPDU(uiMsgLen);
    //这里面是因为我们已经读取了数据中的第一个数据也就是总消息的长度他的类型是uint 所以我们要让指针读取后面的数据
    //参数二是 因为我们已经 读取了一个总消息长度所以要用总消息长度减去 我们读取的那个uint 的大小
    this->read((char *)pdu +sizeof(uint),uiPDULen-sizeof(uint));


    //如果接收的类型是注册请求那么执行注册的相关操作

    return pdu;
}

//发送消息并释放
void MyTcpSocket::sendPUD(PDU *pdu)
{
    if(pdu == nullptr) {
        qDebug()<<"空指针： sendPDU";
        return ;
    }
    this->write((char *)pdu , pdu->uiPDULen);
    qDebug()<<" 发送消息类型："<<pdu->uiMsgType;
    free(pdu);
    pdu = nullptr;

}

//处理消息类型
PDU *MyTcpSocket::handlePDU(PDU *pdu)
{
    //打印对应数据
    qDebug()<<"handlePDU 的消息类型： "<<pdu->uiMsgType
           <<" 参数内容1 ： "<<pdu->caData
           <<" 参数内容2 ： "<<pdu->caData+32
           <<" 消息内容 ： "<<pdu->caMsg;

    if(pdu->uiMsgType==ENUM_MSG_TYPE_REGIST_REQUEST){
        return mess->register_User(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_LOGIN_REQUEST){
        return mess->login_User(pdu, m_Str_login_name);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_SELECT_USER_REQUEST){
        return  mess->select_User(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_ONLINE_USER_REQUEST){
        return mess->online_User(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_ADD_FRIEND_REQUEST){
        return mess->add_Friend(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST){
        return mess->add_Friend_Agree(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_REFRESH_FRIEND_REQUEST){
        return mess->refresh_Friend(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST){
        return mess->delete_Friend(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_CHAT_REQUEST){
        mess->chat_Friend(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_MKDIR_REQUEST){
        return mess->make_Dir(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_FLUSH_FILE_REQUEST){
        return mess->flush_File(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_DELETE_DIR_REQUEST){
        return mess->delete_Dir(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_RENAME_FILE_REQUEST){
        return mess->rename_File(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_MOVE_FILE_REQUEST){
        return mess->move_File(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST){
        return mess->upload_File(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_UPLOAD_FILE_DATA_REQUEST){
        return mess->upload_File_Data(pdu);
    }
    return nullptr;
}


