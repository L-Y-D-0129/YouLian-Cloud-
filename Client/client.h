#ifndef CLIENT_H
#define CLIENT_H
/*
    `#ifndef CLIENT_H`和`#define CLIENT_H`结合在一起可以防止同
    一个头文件被多次包含。通常在编写头文件时使用这种技术，可以避免重
    复包含同一个头文件导致的编译错误。当第一次包含`CLIENT_H`头文件时
    ，`#ifndef CLIENT_H`条件成立，定义`CLIENT_H`，然后再包含头文件
    内容。当第二次或者之后再次包含同一个头文件时，`CLIENT_H`已经被定义
    ，`#ifndef CLIENT_H`条件不成立，这部分内容就会被忽略，从而避免重
    复定义的问题。
*/
#include "protocol.h"

#include <QTcpSocket>
#include <QWidget>
#include"reshandle.h"
QT_BEGIN_NAMESPACE
namespace Ui { class client; }
QT_END_NAMESPACE

class client : public QWidget
{
    Q_OBJECT

public slots:
    void showConnect();
    void recvMsg();
public:

    //储存登录的用户名字
    QString m_Str_login_name;

    ~client();

    //加载配置文件
    void loadConfig();
    //构建单例模式 静态接口
    static client & getInstance();

    //获取私有成员m_tcpSocket; 因为没有拷贝函数那么要返回引用
    QTcpSocket & getTCPsocket();

    PDU *  readPDU();

    Reshandle * rh;

    void handlePDU(PDU * pdu);

    //发送并释放PDU
    void sendPDU(PDU * pdu);

    //处理半包的成员变量
    QByteArray buffer;
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    //为实现单例模式构造函数私有化 并且将复制运算符和 拷贝构造删除
    client(QWidget *parent = nullptr);
    client(const  client & instance) = delete;
    client & operator =(const client &) = delete ;
    Ui::client *ui;
    //ip地址和 端口号
    QString  m_strIP;
    quint16 m_usPort;
    //用于网络通信的套接字
    QTcpSocket m_tcpSocket;
};
#endif // CLIENT_H
