#include "chat.h"
#include "protocol.h"
#include "ui_chat.h"
#include "client.h"
Chat::Chat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chat)
{
    ui->setupUi(this);
}

Chat::~Chat()
{
    delete ui;
}

void Chat::upDataShow_TE(QString strMsg)
{
    ui->textEdit->append(strMsg);
}


//发送按钮的槽函数
void Chat::on_send_PB_clicked()
{
    //获取输入文本框中的文字
    QString strMsg = ui->input_LE->text();

    if(strMsg == nullptr){
        return ;
    }
    //toStdString() 是为了保证中文也可以正常发送
    PDU * pdu = mkPDU(strMsg.toStdString().size()+1);
    //点击返送之后清空输入框
    ui->input_LE->clear();
    pdu->uiMsgType = ENUM_MSG_TYPE_CHAT_REQUEST;

    //将发起聊天的用户 和 聊天的目标用户放在caData 中
    memcpy(pdu->caData,client::getInstance().m_Str_login_name.toStdString().c_str(),32);
    memcpy(pdu->caData+32,m_str_chatname.toStdString().c_str(),32);

    //将消息放在caMsg 中
    memcpy(pdu->caMsg,strMsg.toStdString().c_str(),strMsg.toStdString().size());

    //调用实例发送给服务器
    client::getInstance().sendPDU(pdu);
}
