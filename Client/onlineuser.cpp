#include "client.h"
#include "onlineuser.h"
#include "ui_onlineuser.h"

OnlineUser::OnlineUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineUser)
{
    ui->setupUi(this);
}

OnlineUser::~OnlineUser()
{
    delete ui;
}

void OnlineUser::UpdateOnlineUserWigetList(QStringList UserList)
{
    ui->onlineuser_LW->clear();
    ui->onlineuser_LW->addItems(UserList);
}

//通过现在在线用户的界面点击选中项 cur 向服务器发送添加好友请求
void OnlineUser::on_onlineuser_LW_itemDoubleClicked(QListWidgetItem *item)
{
    //获取当前用户的名字 和 目标的用户名
    QString CurName = client::getInstance().m_Str_login_name;
    QString TarName = item->text();

    PDU * pdu = mkPDU(0);
    qDebug()<<"当前用户名 ： "<<CurName<<"要添加的用户名："<<TarName;
    pdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REQUEST;
    memcpy(pdu->caData,CurName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,TarName.toStdString().c_str(),32);

    client::getInstance().sendPDU(pdu);

}
