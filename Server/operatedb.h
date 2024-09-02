#ifndef OPERATEDB_H
#define OPERATEDB_H

#include <QObject>
#include<QSqlDatabase>
class Operatedb :public QObject
{
    Q_OBJECT
public:
    static Operatedb & getInstance();
    QSqlDatabase m_db;
    ~Operatedb();
    void connectdb();
    bool Regist(char * name , char * pwd);
    bool Login(char * name, char * pwd);
    void clientOffline(const char * name);

    int SelectUser(char * name);
    QStringList OnlineUser(); // 用来获取所有的在线用户

    int Add_Friend(char * CurName , char * TarName);//添加好友的数据库操作

    bool Add_Friend_Agree(char * Curname, char * Tarname);//同意添加好友请求

    QStringList RefreshFriend(char * Curname);//刷新好友

    bool Delete_Friend(char * Curname , char *Tarname);//删除好友
private:
    Operatedb(QObject * parent = nullptr);
    //Operatedb();
    Operatedb(const Operatedb & instance)=delete;
    Operatedb & operator  = (const Operatedb &) = delete ;
};

#endif // OPERATEDB_H
