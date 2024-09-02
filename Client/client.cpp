#include "client.h"
#include "ui_client.h"
#include<QFile>
#include<QDebug>
#include <QHostAddress>
#include"protocol.h"
#include<QMessageBox>
#include"index.h"
client::client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::client)
{
    ui->setupUi(this);

    //调用加载配置文件的函数
    loadConfig();

    //连接成功的信号和连接成功的槽函数
    connect(&m_tcpSocket,&QTcpSocket::connected,this,&client::showConnect);

    //连接接收消息的槽函数
    connect(&m_tcpSocket,&QTcpSocket::readyRead,this,&client::recvMsg);

    //用tcp中的socket 连接服务器
    m_tcpSocket.connectToHost(QHostAddress(m_strIP),m_usPort);

    rh = new Reshandle;

}

client::~client()
{
    delete ui;
    delete rh;
}

void client:: loadConfig(){
    //创建QFile 类
    //：+ 前缀 加文件名
    QFile file(":/client.config");//固定以冒号开头
    //使用QFile 类操作文件
    if(file.open(QIODevice::ReadOnly)){//提取配置文件的ip 和端口号
        QByteArray baData = file.readAll();
        QString strData = QString(baData);
        qDebug()<<strData;
        QStringList strList = strData.split("\r\n");
         m_strIP = strList.at(0);
         m_usPort = strList.at(1).toUShort();
        qDebug()<<"loadConfig :: ip : "<<m_strIP <<"Port "<<m_usPort;
        file.close();
    }else{
        qDebug()<<"打开配置失败";
    }
}

//实现了单例模式之后要将创建对象的时候修改成getInstance ;
//返回的是引用是因我们禁用了 拷贝构造
client &client::getInstance()
{
    //c++ 11之后局部静态变量是线程安全的
    static client instance;
    return instance;
}

QTcpSocket& client::getTCPsocket()
{
    return m_tcpSocket;
}

PDU *client::readPDU()
{
    //要读取信息要先知道信息的长度 所以我们先读取消息中的第一位也就是总消息长度
    uint uiPDULen = 0;
    m_tcpSocket.read((char *)&uiPDULen,sizeof(uint));
    //计算出实际消息的大小也就是柔性数组的大小
    uint uiMsgLen =  uiPDULen - sizeof(PDU);
    PDU * pdu = mkPDU(uiMsgLen);
    //这里面是因为我们已经读取了数据中的第一个数据也就是总消息的长度他的类型是uint 所以我们要让指针读取后面的数据
    //参数二是 因为我们已经 读取了一个总消息长度所以要用总消息长度减去 我们读取的那个uint 的大小
    m_tcpSocket.read((char *)pdu +sizeof(uint),uiPDULen-sizeof(uint));


    //如果接收的类型是注册请求那么执行注册的相关操作
    return pdu;
}

void client::handlePDU(PDU * pdu)
{
    //打印对应数据
    qDebug()<<"handlePDU 的消息类型： "<<pdu->uiMsgType
           <<" 参数内容 ： "<<pdu->caData
           <<" 消息内容 ： "<<pdu->caMsg;

    if(pdu->uiMsgType==ENUM_MSG_TYPE_REGIST_RESPEND){
           rh->res_register_User(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_LOGIN_RESPEND){
        rh->res_login_User(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_SELECT_USER_RESPEND){
            rh->res_select_User(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_ONLINE_USER_RESPEND){
        rh->res_online_User(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_ADD_FRIEND_RESPEND){
        rh->res_add_Friend(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_ADD_FRIEND_REQUEST){
        rh->res_add_Friend_Request(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_ADD_FRIEND_AGREE_RESPEND){
        rh->res_add_Friend_Agree(pdu);
    }else if(pdu ->uiMsgType == ENUM_MSG_TYPE_REFRESH_FRIEND_RESPEND){
        rh->res_refresh_Friend(pdu);
    }else if(pdu ->uiMsgType == ENUM_MSG_TYPE_DELETE_FRIEND_RESPEND){
        rh->res_delete_Friend(pdu);
    }else if(pdu ->uiMsgType == ENUM_MSG_TYPE_CHAT_REQUEST){
        rh->res_chat_Friend(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_MKDIR_RESPEND){
        rh->res_make_Dir(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_FLUSH_FILE_RESPEND){
        rh->res_flush_File(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_DELETE_DIR_RESPEND){
        rh->res_delete_Dir(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_RENAME_FILE_RESPEND){
        rh->res_reanme_File(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_MOVE_FILE_RESPEND){
        rh->res_move_File(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_UPLOAD_FILE_RESPEND){
        rh->res_upload_File(pdu);
    }else if(pdu->uiMsgType == ENUM_MSG_TYPE_UPLOAD_FILE_DATA_RESPEND){
        rh->res_upload_File_Data(pdu);
    }

    free(pdu);
    pdu = nullptr;

}

//发送PDU
void client::sendPDU(PDU *pdu)
{
        m_tcpSocket.write((char *)pdu , pdu->uiPDULen);
        free(pdu);
        pdu = nullptr;
}



void client::showConnect()
{
    qDebug()<<"连接服务器成功";
}

void client::recvMsg()
{

    qDebug()<<"\n\n recvMsg 接收到的消息长度:"<<m_tcpSocket.bytesAvailable();
    //将数据一次读完 追加到我们的成员变量中 处理半包的方法
    QByteArray data  =m_tcpSocket.readAll();
    buffer.append(data);

    //处理粘包
    // buffer 中是否是一个完整的pdu 如果都不是一个完整的pdu 那么一定是半包
    while(buffer.size() >= int(sizeof(PDU))){
        PDU * pdu = (PDU * )buffer.data();
        if(buffer.size() < (int)pdu->uiPDULen){ // 是半包
            break;
        }

        PDU * pdun = mkPDU(pdu->uiMsgLen);
        memcpy(pdun,pdu,pdu->uiPDULen);

        //处理 并发送
        handlePDU(pdun);

        //清理buffer中处理过的数据
        buffer.remove(0,pdu->uiPDULen);
        qDebug()<<" 发送没有问题 ";
    }

}


//注册按钮实现
void client::on_pushButton_clicked()
{   //前32位存储用户名
    QString strR_name = ui->lineEdit->text();
    if(strR_name.isEmpty()){
        QMessageBox::information(this,"用户名","用户名不能为空");
        return;
    }
    //数据放在data中
    PDU * pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_REGIST_REQUEST;
    memcpy(pdu->caData,strR_name.toStdString().c_str(),32);
    //后32位用来存储密码
    QString strR_pwd  = ui->lineEdit_2->text();
    if(strR_pwd.isEmpty()){
        QMessageBox::information(this,"密码","密码不能为空");
        return;
    }
    memcpy(pdu->caData+32,strR_pwd.toStdString().c_str(),32);
    //通过socket 发送给服务器
    m_tcpSocket.write((char *)pdu , pdu->uiPDULen);
    qDebug()<<"87 on_pushButton_clicked ： 账号密码发送成功";
    free(pdu);
    pdu=nullptr;
}


//登录按钮的实现
void client::on_pushButton_2_clicked()
{
    //前32位存储用户名
        QString strR_name = ui->lineEdit->text();
        if(strR_name.isEmpty()){
            QMessageBox::information(this,"用户名","用户名不能为空");
            return;
        }
        //数据放在data中
        PDU * pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_REQUEST;
        memcpy(pdu->caData,strR_name.toStdString().c_str(),32);
        //后32位用来存储密码
        QString strR_pwd  = ui->lineEdit_2->text();
        if(strR_pwd.isEmpty()){
            QMessageBox::information(this,"密码","密码不能为空");
            return;
        }
        memcpy(pdu->caData+32,strR_pwd.toStdString().c_str(),32);
        //通过socket 发送给服务器
        m_tcpSocket.write((char *)pdu , pdu->uiPDULen);
        m_Str_login_name = strR_name;
        qDebug()<<"170 on_pushButton_clicked ： 账号密码发送成功";
        free(pdu);
        pdu=nullptr;
}
