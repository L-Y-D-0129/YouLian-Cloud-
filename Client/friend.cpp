#include "friend.h"
#include "ui_friend.h"

#include<QInputDialog>
#include<QDebug>
#include<protocol.h>
#include<client.h>
#include <qmessagebox.h>
Friend::Friend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Friend)
{
    ui->setupUi(this);
    onlinuser = new OnlineUser();
    Send_Refresh_Friend();
    m_chat = new Chat;
    m_timer = new QTimer;
    //定时器
    m_timer->setTimerType(Qt::PreciseTimer);
    //连接定时器和刷新好友列表函数
    connect(m_timer,&QTimer::timeout,this,&Friend::Send_Refresh_Friend);
    //启动定时器 时间间隔定在 1000 ms = 1s
    m_timer->start(10000);
}

Friend::~Friend()
{
    delete ui;
    delete onlinuser;
    delete m_chat;
    m_timer->stop();
    delete m_timer;
}

OnlineUser *Friend::getOnlineUser()
{
    return onlinuser;
}

Chat *Friend::getChat()
{
    return m_chat;
}

//更新好友列表
void Friend::Updata_FriendLW(QStringList list)
{
    ui->listWidget->clear();
    ui->listWidget->addItems(list);
}

//封装发送刷新好友的函数
void Friend::Send_Refresh_Friend()
{
    PDU * pdu = mkPDU(0);
    //这里要赋值 参数第二个如果本身不是字符数组 那么要转成字符
    memcpy(pdu->caData,client::getInstance().m_Str_login_name.toStdString().c_str(),32);
    qDebug()<<"Send_Refresh_Friend Curname"<<pdu->caData;
    pdu -> uiMsgType = ENUM_MSG_TYPE_REFRESH_FRIEND_REQUEST;
    client::getInstance().sendPDU(pdu);
}

QTimer *Friend::getTimer()
{
    return m_timer;
}



//查找用户的功能
void Friend::on_findUser_PB_clicked()
{
    //QInputDialog 返回值就是用户输入的值
   QString name =  QInputDialog::getText(this,"查找","用户名： ");
   selUname  = name;
   if(name.isEmpty()){
       QMessageBox::information(this,"用户名","用户名不能为空");
       return ;
   }
   client::getInstance();
   PDU * selPDU = mkPDU(0);
   //设置类型查找用户的请求
   selPDU->uiMsgType = ENUM_MSG_TYPE_SELECT_USER_REQUEST;
   memcpy(selPDU->caData,name.toStdString().c_str(),32);
    //因为本身要使用单例模式重点tcpsocket进行发送但是因为tcpsocket是私有的要创建一个共有的函数去获取私有成员变量
    client::getInstance().getTCPsocket().write((char *)selPDU,selPDU->uiPDULen);
    qDebug()<<"findUser send seccess！";
    free(selPDU);
    selPDU = nullptr;
}


//显示在线用户的功能
void Friend::on_onlineUser_PB_clicked()
{
    //这里是如果这个在线用户的页面如果被隐藏了 那么点击就出现 但是出现了没有被叉掉就不在显示同一个页面了
    if(onlinuser->isHidden()){
        onlinuser->show();
    }
    PDU * pdu = mkPDU(0);
    pdu ->uiMsgType = ENUM_MSG_TYPE_ONLINE_USER_REQUEST;
    client::getInstance().getTCPsocket().write((char*)pdu,pdu->uiPDULen);
    free(pdu);
    pdu= nullptr;
}


//刷新好有功能
void Friend::on_flushFriend_PB_clicked()
{
    Send_Refresh_Friend();
}


//删除好友的功能 向服务器发送请求
void Friend::on_delFriend_PB_clicked()
{
    //从好友列表框中获取当前项
    QListWidgetItem * pItem   =ui->listWidget->currentItem();
    if(pItem == nullptr){
        QMessageBox::information(this,"删除好友","请选择要删除的好友");
        return;
    }
    QString Tarname = pItem->text();
    int ret = QMessageBox::question(this,"删除好友",QString("是否确认删除好友 %1").arg(Tarname));
    if(ret!=QMessageBox::Yes){
        return ;
    }
    PDU * pdu = mkPDU(0);
    pdu->uiMsgType = ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST;
    //执行删除操作的前32位储存Curname
    memcpy(pdu->caData,client::getInstance().m_Str_login_name.toStdString().c_str(),32);
    //后32位储存目标用户
    memcpy(pdu->caData+32,Tarname.toStdString().c_str(),32);
    client::getInstance().sendPDU(pdu);

}

//聊天按钮
void Friend::on_chat_PB_clicked()
{
    //从好友列表框中获取当前项 选择和谁聊天
    QListWidgetItem * pItem   =ui->listWidget->currentItem();
    if(pItem == nullptr){
        QMessageBox::information(this,"聊天","请选择聊天的好友");
        return;
    }
    //用chat 来储存聊天对象的名字
    m_chat->m_str_chatname = pItem->text();

    //如果是隐藏的我们就展示出来
    if(m_chat->isHidden()){
        m_chat->setWindowTitle(m_chat->m_str_chatname);
        m_chat->show();
    }

}
