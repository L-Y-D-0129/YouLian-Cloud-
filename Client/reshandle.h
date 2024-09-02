#ifndef RESHANDLE_H
#define RESHANDLE_H

#include "protocol.h"

#include <QString>


//响应服务器发送回来的消息类
class Reshandle
{
public:
    Reshandle();

    //注册
    void res_register_User(PDU * pdu);
    //登录
    void res_login_User(PDU * pdu);

    void res_select_User(PDU * pdu);
    //在线用户
    void res_online_User(PDU * pdu);
    //服务器响应给CurUser 的结果
    void res_add_Friend(PDU * pdu);
    //TarUser 收到服务器的转发处理
    void res_add_Friend_Request(PDU * pdu);

    void res_add_Friend_Agree(PDU * pdu);

    //刷新好友
    void res_refresh_Friend(PDU * pdu);

    //删除好友
    void res_delete_Friend(PDU * pdu);

    //聊天功能
    void res_chat_Friend(PDU * pdu);

    //创建文件夹
    void res_make_Dir(PDU * pdu);

    //刷新文件
    void res_flush_File(PDU * pdu);

    //删除文件夹
    void res_delete_Dir(PDU * pdu);

    //重命名
    void res_reanme_File(PDU * pdu);

    //移动文件
    void res_move_File(PDU * pdu);

    //上传文件
    void res_upload_File(PDU * pdu);

    //上传文件数据
    void res_upload_File_Data(PDU * pdu);
};

#endif // RESHANDLE_H
