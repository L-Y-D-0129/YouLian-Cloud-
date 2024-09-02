#include "reshandle.h"
#include"client.h"
#include "index.h"
#include <QMessageBox>
#include"chat.h"

Reshandle::Reshandle()
{

}

void Reshandle::res_register_User(PDU *pdu)
{
    bool res ;
    memcpy(&res,pdu->caData,sizeof(bool));
    if(res){
        QMessageBox::information(&client::getInstance(),"注册","注册成功");
    }else {
         QMessageBox::information(&client::getInstance(),"注册","注册失败");
    }
}

void Reshandle::res_login_User(PDU *pdu)
{
    bool res ;
    memcpy(&res,pdu->caData,sizeof(bool));
    if(res){
        //QMessageBox::information(this,"登录","登录成功");
        //在登录成功之后通过页面的隐藏来实现跳转页面
        Index::getInstance().show();
        client::getInstance().hide();//用来隐藏的函数
    }else {
         QMessageBox::information(&client::getInstance(),"登录","登录失败");
    }
}

void Reshandle::res_select_User(PDU *pdu)
{
    int res;
    char Username[32] = {'\0'};
    memcpy(Username,pdu->caData,32);
    memcpy(&res,pdu->caData+32,sizeof(int));
    if(res == 1){
        int ret =QMessageBox::information(&Index::getInstance(),"搜索",QString("用户%1 在线").arg(Username),"添加好友","取消");
        if(ret !=0){//如果点击的是取消那么就可以关闭了
            return ;
        }
        //给查找用户增加添加好友的功能
        PDU * pdu = mkPDU(0);
        qDebug()<<"\n\nres_select_User 当前用户名 ： "<<client::getInstance().m_Str_login_name<<"要添加的用户名："<<Username;
        pdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
        memcpy(pdu->caData,client::getInstance().m_Str_login_name.toStdString().c_str(),32);
        memcpy(pdu->caData+32,Username,32);

        client::getInstance().sendPDU(pdu);
    }else if( res == 0){
        QMessageBox::information(&Index::getInstance(),"搜索",QString("用户%1 离线").arg(Username));
    }else if( res == -1){
        QMessageBox::information(&Index::getInstance(),"搜索",QString("用户%1 用户不存在").arg(Username));

    }
}

void Reshandle::res_online_User(PDU *pdu )
{
    char Utemp[32] = {'\0'};
    QStringList userlist ;
    userlist.clear();
    int Usize = pdu->uiMsgLen/32;
    for(int i =0;i<Usize;i++){
        memcpy(Utemp,pdu->caMsg+(i*32),32);
        if(QString(Utemp) == client::getInstance().m_Str_login_name){
            continue;
        }
        userlist.append(QString(Utemp));
    }
    Index::getInstance().getFriendpage()->getOnlineUser()->UpdateOnlineUserWigetList(userlist);
}

//服务器回馈给 CurUser 的弹窗
void Reshandle::res_add_Friend(PDU *pdu)
{
    int res;
    memcpy(&res,pdu->caData,sizeof(int));
    if(res == -1){
        QMessageBox::information(&Index::getInstance(),"添加好友",QString("添加好友失败，服务器出错"));
    }else if( res == 0){
        QMessageBox::information(&Index::getInstance(),"添加好友",QString("添加失败，用户不在线"));
    }else if(res == -2){
        QMessageBox::information(&Index::getInstance(),"添加好友",QString("添加失败，已经是好友了"));
    }
}

void Reshandle::res_add_Friend_Request(PDU *pdu)
{
    //这个添加者的名字
    char Username[32] = {"\0"};
    int ret ;
    memcpy(Username,pdu->caData,32);
    ret = QMessageBox::question(&Index::getInstance(),"添加好友申请",QString("是否同意'%1'的好友申请").arg(Username));
    //如果TarUser 用户点击的不是同意那我们目前不做处理 有时间可以进行反馈给 CurUser 告诉他 对方拒绝添加你为好友
    if(ret!=QMessageBox::Yes){
        return ;
    }

    PDU * respdu = mkPDU(0);
    //同意好友请求那么要在向服务器发送同意好友的请求 服务器要进行处理 例如在数据库中进行数据绑定
    memcpy(respdu->caData,pdu->caData,64);
    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE_REQUEST;
    //调用client 的单例模式来通过使用函数过去私有成员变量socket 在通过socket 向服务器发送请求
    client::getInstance().getTCPsocket().write((char * )respdu,respdu->uiPDULen);
    free(respdu);
    respdu = nullptr;
}

void Reshandle::res_add_Friend_Agree(PDU *pdu)
{
    bool ret = memcpy(&ret , pdu->caData,sizeof(bool));
    qDebug()<<"res_add_Friend_Agree ret = "<<ret;
    if(ret){
         QMessageBox::information(&Index::getInstance(),"添加好友","添加好友成功！");
         Index::getInstance().getFriendpage()->Send_Refresh_Friend();
    }else{
         QMessageBox::information(&Index::getInstance(),"添加好友","添加好友失败");
         return ;
    }

}

void Reshandle::res_refresh_Friend(PDU *pdu)
{
    char Utemp[32] = {'\0'};
    QStringList friendlist;
    friendlist.clear();
    int Usize = pdu->uiMsgLen/32;
    for(int i =0;i<Usize;i++){
        memcpy(Utemp , pdu->caMsg+(i*32),32);
        friendlist.append(QString(Utemp));
    }
    Index::getInstance().getFriendpage()->Updata_FriendLW(friendlist);

}

//删除好友
void Reshandle::res_delete_Friend(PDU *pdu)
{
    bool ret = false;
    memcpy(&ret,pdu->caData,sizeof(bool));
    if(ret == false){
        QMessageBox::information(&Index::getInstance(),"删除好友","你们已经不是好友了，无需删除");
    }else{
        QMessageBox::information(&Index::getInstance(),"删除好友","删除成功！");
    }
    //删除无论成功失败自动刷新好友列表
    Index::getInstance().getFriendpage()->Send_Refresh_Friend();
}

//经过服务器转发给目标用户的处理
//将接收到的消息展示在那个聊天框中
void Reshandle::res_chat_Friend(PDU *pdu)
{
    Chat * pC = Index::getInstance().getFriendpage()->getChat();

    char Curname[32] = {'\0'};
    memcpy(&Curname,pdu->caData,32);
    //接收到的消息
    pC->m_str_chatname = Curname;
    if(pC->isHidden()){
        pC->setWindowTitle(pC->m_str_chatname);
        pC->show();
    }
    pC->upDataShow_TE(QString("%1 : %2").arg(Curname).arg(pdu->caMsg));
}

//创建文件夹
void Reshandle::res_make_Dir(PDU *pdu)
{
    bool res;
    memcpy(&res,pdu->caData,sizeof(bool));

    if(!res){
        QMessageBox::information(&Index::getInstance(),"创建文件夹","创建文件夹失败");
    }
    //创建完文件列表我们进行刷线文件列表
    Index::getInstance().getFilepage()->Send_Flush_File();
}

void Reshandle::res_flush_File(PDU *pdu)
{
    int iCount = pdu->uiMsgLen/(sizeof(FileInfo));
    qDebug()<<"res_flush_File iCount:"<<iCount;
    QList<FileInfo *> filelist;
    for(int i=0;i<iCount;i++){
        FileInfo* pfileinfo = new FileInfo;
        memcpy(pfileinfo,pdu->caMsg+i*sizeof(FileInfo) , sizeof(FileInfo));
        filelist.append(pfileinfo);
        qDebug()<<"res_flush_File FileName :"<<pfileinfo->FileName;
    }
    Index::getInstance().getFilepage()->Updata_FileLW(filelist);
}

//删除文件夹 问题服务器发送的是 true 到我们这边就是 false;
void Reshandle::res_delete_Dir(PDU *pdu)
{

    bool res;
    memcpy(&res,pdu->caData,sizeof(bool));
    qDebug()<<"res_delete_Dir res "<<res;
    if(!res){
        QMessageBox::information(&Index::getInstance(),"删除文件夹","删除文件夹失败！");
    }
    //删除文件我们进行刷线文件列表
    Index::getInstance().getFilepage()->Send_Flush_File();
}
//重命名
void Reshandle::res_reanme_File(PDU *pdu)
{
    bool res;
    memcpy(&res,pdu->caData,sizeof(bool));
    qDebug()<<"res_reanme_File res "<<res;
    if(!res){
        QMessageBox::information(&Index::getInstance(),"重命名文件","重命名文件夹失败！");
    }
    //删除文件我们进行刷线文件列表
    Index::getInstance().getFilepage()->Send_Flush_File();
}

//移动文件
void Reshandle::res_move_File(PDU *pdu)
{
    bool res;
    memcpy(&res,pdu->caData,sizeof(bool));
    qDebug()<<"res_move_File res "<<res;
    if(!res){
        QMessageBox::information(&Index::getInstance(),"移动文件","移动文件夹失败！");
    }
    //删除文件我们进行刷线文件列表
    Index::getInstance().getFilepage()->Send_Flush_File();
}

//上传文件
void Reshandle::res_upload_File(PDU *pdu)
{
    int res;
    memcpy(&res,pdu->caData,sizeof(int));
    if(res == -1){
        QMessageBox::information(Index::getInstance().getFilepage(),"上传文件","打开文件失败");
    }else if( res == 0){
       Index::getInstance().getFilepage()->uploadFile();
    }else if(res == -2){
        QMessageBox::information(Index::getInstance().getFilepage(),"上传文件","已有文件在上传");
    }
}

//上传文件数据
void Reshandle::res_upload_File_Data(PDU *pdu)
{
    bool res;
    memcpy(&res,pdu->caData,sizeof(bool));
    qDebug()<<"res_upload_File_Data res "<<res;
    if(!res){
        QMessageBox::information(&Index::getInstance(),"上传文件","上传文件失败！");
    }else {
        QMessageBox::information(&Index::getInstance(),"上传文件","上传文件成功！");
    }
    Index::getInstance().getFilepage()->Send_Flush_File();
    Index::getInstance().getFilepage()->m_bUpload = false;
}
