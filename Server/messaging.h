#ifndef MESSAGING_H
#define MESSAGING_H
#include "operatedb.h"
#include "protocol.h"
#include<QDebug>
#include <QFile>
#include<QString>

class Messaging
{
public:
    Messaging();
    //注册
    PDU * register_User(PDU * pdu);
    //登录
    PDU * login_User(PDU * pdu,QString &m_Str_login_name);

    PDU * select_User(PDU * pdu);
    //在线用户
    PDU * online_User(PDU * pdu);
    //好友申请
    PDU * add_Friend(PDU * pdu);
    //同意好友申请
    PDU * add_Friend_Agree(PDU * pdu);
    //刷线好友列表
    PDU * refresh_Friend(PDU * pdu);
    //删除好友
    PDU * delete_Friend(PDU * pdu);
    //与好友聊天
    void chat_Friend(PDU * pdu);
    //创建文件夹
    PDU * make_Dir(PDU * pdu);
    //刷新文件
    PDU * flush_File(PDU * pdu);
    //删除文件夹
    PDU * delete_Dir(PDU * pdu);
    //重命名文件
    PDU * rename_File(PDU * pdu);
    //移动函数
    PDU * move_File(PDU * pdu);
    //上传文件
    PDU * upload_File(PDU * pdu);
    //上传文件数据
    PDU * upload_File_Data(PDU * pdu);
    QFile m_UploadFile;     //文件对象
    bool m_Upload;          //是否上传
    qint64 FileSize;        //文件的大小
    qint64 UploadFileSize;  //已经上传的大小
};

#endif // MESSAGING_H
