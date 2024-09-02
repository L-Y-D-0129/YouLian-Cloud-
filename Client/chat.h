#ifndef CHAT_H
#define CHAT_H

#include <QWidget>

namespace Ui {
class Chat;
}

class Chat : public QWidget
{
    Q_OBJECT

public:
    explicit Chat(QWidget *parent = nullptr);
    ~Chat();
    //聊天对象的名字
    QString m_str_chatname;

    void upDataShow_TE(QString strMsg);

private slots:
    void on_send_PB_clicked();
private:
    Ui::Chat *ui;
};

#endif // CHAT_H
