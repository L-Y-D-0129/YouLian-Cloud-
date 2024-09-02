#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include"onlineuser.h"
#include"chat.h"
#include <QTimer>
namespace Ui {
class Friend;
}

class Friend : public QWidget
{
    Q_OBJECT

public:
    explicit Friend(QWidget *parent = nullptr);
    ~Friend();
    QString selUname;
    OnlineUser * getOnlineUser();
    Chat * getChat();//更安全 这样的方式不可以进行修改 只能获取
    void Updata_FriendLW(QStringList list);
    void Send_Refresh_Friend();
    QTimer *getTimer();
private slots:
    void on_findUser_PB_clicked();

    void on_onlineUser_PB_clicked();

    void on_flushFriend_PB_clicked();

    void on_delFriend_PB_clicked();

    void on_chat_PB_clicked();

private:
    Ui::Friend *ui;
    OnlineUser * onlinuser;
    Chat * m_chat;
    QTimer *m_timer;
};

#endif // FRIEND_H
