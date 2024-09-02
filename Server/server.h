#ifndef SERVER_H
#define SERVER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Server; }
QT_END_NAMESPACE

class Server : public QWidget
{
    Q_OBJECT

public:
    ~Server();

    //加载配置文件
    void loadConfig();
    static Server& getInstance();
    QString & getRootPath();
private:
    Ui::Server *ui;
    Server(QWidget *parent = nullptr);
    Server(const Server & instance) = delete;
    Server & operator =(const Server &) = delete;
    QString m_strIP ;
    quint16 m_usPort ;
    //文件系统路径
    QString m_strRootPath;
};
#endif // SERVER_H
