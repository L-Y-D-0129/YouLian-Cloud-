#ifndef ONLINEUSER_H
#define ONLINEUSER_H

#include <QWidget>
#include <qlistwidget.h>

namespace Ui {
class OnlineUser;
}

class OnlineUser : public QWidget
{
    Q_OBJECT

public:
    explicit OnlineUser(QWidget *parent = nullptr);
    ~OnlineUser();
    void UpdateOnlineUserWigetList(QStringList UserList);

private slots:
    //点击双击在线用户发送好友添加申请
    void on_onlineuser_LW_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::OnlineUser *ui;
};

#endif // ONLINEUSER_H
