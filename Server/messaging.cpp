#include "messaging.h"
#include "mytcpserver.h"
#include "mytcpsocket.h"
#include "server.h"

#include <QDir>
Messaging::Messaging()
{
    m_Upload = false;
}

PDU *Messaging::register_User(PDU *pdu)
{
    // 取出Data 中的 账号和密码
    char Username[33];  // 多分配一个字符存储null终止符
    char Password[33];  // 多分配一个字符存储null终止符
    memcpy(Username, pdu->caData, 32);
    Username[32] = '\0';  // 添加null终止符
    memcpy(Password, pdu->caData + 32, 32);
    Password[32] = '\0';  // 添加null终止符
    qDebug() << Username;
    qDebug() << Password;
    bool ret = Operatedb::getInstance().Regist(Username,Password);

    //在用户注册的时候就给用户创建属于自己文件目录
    if(ret == true){
        QDir dir;
        dir.mkdir(QString("%1/%2").arg(Server::getInstance().getRootPath()).arg(Username));
    }

    //服务器连接数据库得到的返回值 响应给客户端
    PDU *respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_REGIST_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
}

PDU * Messaging::login_User(PDU *pdu ,QString &m_Str_login_name)
{
    char Username[33];  // 多分配一个字符存储null终止符
    char Password[33];  // 多分配一个字符存储null终止符
    memcpy(Username, pdu->caData, 32);
    Username[32] = '\0';  // 添加null终止符
    memcpy(Password, pdu->caData + 32, 32);
    Password[32] = '\0';  // 添加null终止符
    qDebug() <<"login Username"<< Username;
    qDebug() <<"login Password"<< Password;
    //发送给数据库进行判断
    bool ret = Operatedb::getInstance().Login(Username,Password);
    if(ret==true){
        m_Str_login_name = Username;
    }
    PDU *respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
}

PDU *Messaging::select_User(PDU *pdu)
{
    char Username[33];
    //从客户端发送过来的PDU中取出用户名
    memcpy(Username, pdu->caData, 32);
    Username[32] = '\0';
    //因为是单例模式所以要是用过去实例来调用 查询功能
    qDebug()<<"select_User :"<<Username;
    int ret = Operatedb::getInstance().SelectUser(Username);
    qDebug()<<"ret = "<<ret;
    PDU * retpdu = mkPDU(0);
    retpdu->uiMsgType = ENUM_MSG_TYPE_SELECT_USER_RESPEND;
    memcpy(retpdu->caData , Username , 32);
    memcpy(retpdu->caData+32 , &ret , sizeof(int));

    return retpdu;
}

//在线用户的功能
PDU *Messaging::online_User(PDU *pdu)
{
    const QStringList Userlist = Operatedb::getInstance().OnlineUser();
    int n = Userlist.size();
    uint uiMsglen = n*32;
    PDU * retpdu = mkPDU(uiMsglen);
    retpdu ->uiMsgType = ENUM_MSG_TYPE_ONLINE_USER_RESPEND;
    for(int i=0;i<n;i++){
        //通过循环来将 Userlist 中的数据加入到柔性数组中
         memcpy(retpdu->caMsg+(i*32), Userlist.at(i).toStdString().c_str(), 32);
    }
    return retpdu;
}


PDU *Messaging::add_Friend(PDU *pdu)
{
    char Curname[33];  // 多分配一个字符存储null终止符
    char Tarname[33];  // 多分配一个字符存储null终止符
    memcpy(Curname, pdu->caData, 32);
    Curname[32] = '\0';  // 添加null终止符
    memcpy(Tarname, pdu->caData + 32, 32);
    Tarname[32] = '\0';  // 添加null终止符
    qDebug() <<"Curname"<< Curname;
    qDebug() <<"Tarname"<< Tarname;
    //发送给数据库进行判断
    int ret = Operatedb::getInstance().Add_Friend(Curname,Tarname);
     qDebug() <<"ret = "<< ret;
    //如果在线就是返回1 那么就是没有问题的
    if(ret==1){
      MyTcpServer::getInstance().resend(Tarname,pdu);
    }
    PDU *respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(int));
    return respdu;
}

//处理来自TarUser的同意好友申请的请求
//要在数据库操作类要一个将两个人的id 储存在好友表中的函数 返回成功和失败
//成功要返回给TarUser 一提醒
PDU *Messaging::add_Friend_Agree(PDU *pdu)
{

    char Curname[33];  // 多分配一个字符存储null终止符
    char Tarname[33];  // 多分配一个字符存储null终止符
    memcpy(Curname, pdu->caData, 32);
    Curname[32] = '\0';  // 添加null终止符
    memcpy(Tarname, pdu->caData + 32, 32);
    Tarname[32] = '\0';  // 添加null终止符
    bool ret = Operatedb::getInstance().Add_Friend_Agree(Curname,Tarname);
    PDU * respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(int));
    //同时要转发给Curname 一份消息这里应该是respdu 而不是 pdu 因为是接收同意的这个消息提醒就是同意好友申请的回应
    MyTcpServer::getInstance().resend(Curname,respdu);
    return respdu;
}

PDU *Messaging::refresh_Friend(PDU *pdu)
{
    char Curname[33] = {'\0'};
    memcpy(Curname,pdu->caData,32);
    const QStringList Friendlist = Operatedb::getInstance().RefreshFriend(Curname);
    int n = Friendlist.size();
    uint uiMsglen = n*32;
    PDU * retpdu = mkPDU(uiMsglen);
    retpdu ->uiMsgType = ENUM_MSG_TYPE_REFRESH_FRIEND_RESPEND;
    for(int i=0;i<n;i++){
        //通过循环来将 Userlist 中的数据加入到柔性数组中
         memcpy(retpdu->caMsg+(i*32), Friendlist.at(i).toStdString().c_str(), 32);
    }
    return retpdu;
}

//删除好友
PDU *Messaging::delete_Friend(PDU *pdu)
{
    //取出Curname 和 Tarname
    char Curname[32] = {'\0'};
    char Tarname[32] = {'\0'};
    memcpy(Curname,pdu->caData,32);
    memcpy(Tarname,pdu->caData+32,32);
    qDebug()<<"\n\nCurname: "<<Curname;
    qDebug()<<"Tarname: "<<Tarname;
    bool ret = Operatedb::getInstance().Delete_Friend(Curname,Tarname);
    qDebug()<<"delete_Friend 数据库操作";
    PDU * respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(bool));
    return respdu;
}

//服务器处理聊天---->提取目标用户名将消息转发给目标用户
 void Messaging::chat_Friend(PDU *pdu)
{
    //提取目标用户名
    char Tarname[32] = {'\0'};
    memcpy(Tarname,pdu->caData+32,32);
    //将消息转发给目标用户
    MyTcpServer::getInstance().resend(Tarname,pdu);
    qDebug()<<"Tarname : "<<Tarname;
 }

 //创建文件夹
 PDU *Messaging::make_Dir(PDU *pdu)
 {
     char newDirName[32] = {'\0'};
     QString curPathName =pdu->caMsg;
     memcpy(newDirName,pdu->caData,32);

     PDU * respdu = mkPDU(0);
     respdu->uiMsgType = ENUM_MSG_TYPE_MKDIR_RESPEND;

     QDir dir;
     bool ret = false;

     QString NewPath = QString("%1/%2").arg(curPathName).arg(newDirName);
     qDebug()<<"\nnewDirName: "<<newDirName;
     qDebug()<<"curPathName : "<<curPathName;
     qDebug()<<"NewPath: "<<NewPath;
     if(!dir.exists(curPathName)||dir.exists(NewPath)||!dir.mkdir(NewPath)){
         qDebug()<<"失败 ";
         memcpy(respdu->caData,&ret,sizeof(bool));
         return respdu;
     }

     ret = true;
     memcpy(respdu->caData,&ret,sizeof(bool));

     return respdu;

 }


//刷新文件
 PDU *Messaging::flush_File(PDU *pdu)
 {
    char * strCurPath = pdu->caMsg;
    QDir d(strCurPath);
    //通过这个函数得到当前路径下的所有文件
    QFileInfoList fileinfolist = d.entryInfoList();
    int FileCount = fileinfolist.size();
    //这里构建pdu 的大小就是文件的个数 * 每个结构体的大小
    PDU * respdu = mkPDU((FileCount-2) * (sizeof(FileInfo)));
    respdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_RESPEND;

    //将在fileinfolist 中的 名字都存到caMsg 中
    FileInfo *pfileinfo = nullptr;
    QString filename ;

    for(int i =0,j =0;i<FileCount ;i++){
        //这里指针的每次的偏移量就是 i 因为每次指针的便宜都是以类型的大小就行偏移 这里每次i 都是偏移一个fileinfo的大小
       filename = fileinfolist[i].fileName();
       if(filename ==QString("..")||filename==QString(".")){
            continue;
       }
       pfileinfo = (FileInfo*)respdu->caMsg+j++;

       memcpy(pfileinfo->FileName,filename.toStdString().c_str(),filename.toStdString().size());

       if(fileinfolist[i].isDir()){
           pfileinfo->FileType = 0;//目录 0
       }else if(fileinfolist[i].isFile()){
           pfileinfo->FileType =1;// 文件 1
       }
       qDebug()<<"flush_File-filename : "<<filename;
    }
    return respdu;
 }

//删除文件夹
 PDU *Messaging::delete_Dir(PDU *pdu)
 {
    QString DelDirPath = pdu->caMsg;

    QDir d(DelDirPath);
    bool ret = false;
    ret = d.removeRecursively();
    qDebug()<< "delete_Dir : DelDirPath"<<DelDirPath;
    PDU * respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_DELETE_DIR_RESPEND;
    memcpy(respdu->caData,&ret , sizeof(bool));
    qDebug()<< "delete_Dir : ret"<<ret;
    return respdu;
 }

 //重命名
 PDU *Messaging::rename_File(PDU *pdu)
 {
     char newDirName[32] = {'\0'};
     char oldDirName[32] = {'\0'};
     QString curPathName =pdu->caMsg;
     memcpy(oldDirName,pdu->caData,32);
     memcpy(newDirName,pdu->caData+32,32);

     PDU * respdu = mkPDU(0);
     respdu->uiMsgType = ENUM_MSG_TYPE_RENAME_FILE_RESPEND;

     QDir dir;
     bool ret = false;
     QString OldPath = QString("%1/%2").arg(curPathName).arg(oldDirName);
     QString NewPath = QString("%1/%2").arg(curPathName).arg(newDirName);

     ret = dir.rename(OldPath,NewPath);
     memcpy(respdu->caData,&ret,sizeof(bool));
     return respdu;
 }

 //移动文件
 PDU *Messaging::move_File(PDU *pdu)
 {
    int strcLen = 0;
    int tarLen = 0;
    memcpy(&strcLen,pdu->caData,sizeof(int));
    memcpy(&tarLen,pdu->caData+32,sizeof(int));
    //初始化
    char * pSrcPath = new char[strcLen+1];
    char * pTarPath = new char[tarLen+1];
    memset(pSrcPath,'\0',strcLen+1);
    memset(pTarPath,'\0',tarLen+1);

    memcpy(pSrcPath,pdu->caMsg,strcLen);
    memcpy(pTarPath,pdu->caMsg+strcLen,tarLen);

    bool ret =  QFile::rename(pSrcPath,pTarPath);

    PDU * respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_MOVE_FILE_RESPEND;
    memcpy(respdu->caData,&ret,sizeof(bool));

    delete [] pSrcPath;
    delete [] pTarPath;
    pSrcPath = nullptr;
    pTarPath = nullptr;
    return respdu;
 }

 //上传文件
 //ret = 1 已经有文件上传  ret =-1 打开文件失败 ret = 0 成功
 PDU *Messaging::upload_File(PDU *pdu)
 {

    PDU * respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_RESPEND;
    int ret = 0;
    if(m_Upload){
        ret =1;
        qDebug()<<"已经有文件在上传，稍后执行";
        memcpy(respdu->caData,&ret,sizeof (int));
        return respdu;
    }
    char FileName [32] = {'\0'};
    qint64 filesize = 0;

    memcpy(FileName,pdu->caData,32);
    memcpy(&filesize,pdu->caData+32,sizeof(qint64));
    QString strPath  = QString("%1/%2").arg(pdu->caMsg).arg(FileName);
    qDebug()<<"upload_File :strPath "<<strPath;
    qDebug()<<"upload_File :FileName "<<FileName;
    //因为要多次上传所以要储存文件名字 下面是初始化文件对象
    m_UploadFile.setFileName(strPath);
    //以只写的方式打开是否成功进行判断
    if(m_UploadFile.open(QIODevice::WriteOnly)){
        m_Upload = true;
        FileSize = filesize;
        UploadFileSize = 0;//已经上传的大小更新为 0;
        qDebug()<<"upload_File :file open success! ";
    }else{
        ret = -1;
        qDebug()<<"upload_File :file open failed";
    }
    memcpy(respdu->caData,&ret,sizeof (int));
    return respdu;
 }

 //上传文件数据
 PDU *Messaging::upload_File_Data(PDU *pdu)
 {

    //将数据写入文件中并更新记录已经更新字节数的变量
    m_UploadFile.write(pdu->caMsg,pdu->uiMsgLen);
    UploadFileSize += pdu->uiMsgLen;

    if(UploadFileSize < FileSize){
        return nullptr;
    }

    //已将讲全部数据全部写入了文件中
    m_UploadFile.close(); // 写完了就要关闭文件！
    m_Upload = false;
    PDU * respdu = mkPDU(0);
    respdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_DATA_RESPEND;
    bool ret = UploadFileSize == FileSize;
    memcpy(respdu->caData , &ret , sizeof(bool));
    return respdu;
 }
