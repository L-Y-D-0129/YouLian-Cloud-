#include "operatedb.h"
#include "protocol.h"
#include<QDebug>
#include <qsqlquery.h>

Operatedb::Operatedb(QObject *parent) : QObject(parent)
{
    m_db = QSqlDatabase::addDatabase("QMYSQL");
}


Operatedb &Operatedb::getInstance()
{
    static Operatedb instance;
    return instance;
}

Operatedb::~Operatedb()
{
    m_db.close();
}


//与数据库建立连接
void Operatedb::connectdb()
{
    this->m_db.setHostName("localhost");
    this->m_db.setPort(3307);
    this->m_db.setUserName("root");
    this->m_db.setPassword("123456");
    this->m_db.setDatabaseName("qt");
    if(m_db.open()){
        qDebug()<<"连接成功";
    }else {
        qDebug()<<"连接失败";
    }

}

bool Operatedb::Regist(char *name, char *pwd)
{
    if( name == nullptr|| pwd == nullptr){
        return false;
    }
    QString sql = QString("select * from user_info where name = '%1';").arg(name);
    QSqlQuery q;
    //执行语句失败或者有数据都代表注册失败 or 用户名已存在
    if(!q.exec(sql)||q.next()){
          return false;
    }
    sql = QString("insert into user_info(name, pwd) values('%1','%2');").arg(name).arg(pwd);
    qDebug()<<"创建成功";
    return q.exec(sql);
}

//登录的数据库判断要进行判断 用户是否存在 ，不存在要提示用户进行注册 ， 判断密码是否正确 正确登录成功 ， 不正确提示密码错误
bool Operatedb::Login(char *name, char *pwd)
{   //使用提前退出的代码格式！
    if( name == nullptr|| pwd == nullptr){
        return false;
    }
    QString sql = QString("select * from user_info where name='%1' and pwd='%2';").arg(name).arg(pwd);
    QSqlQuery q;
    //登录是执行失败和用户不存在 返回 false;
    if(!q.exec(sql)||!q.next()){
        return false;
    }
    sql = QString ("update user_info set online = 1 where name = '%1' and pwd = '%2';").arg(name).arg(pwd);
    qDebug()<<"登录成功！";
    return q.exec(sql);
}

//处理客户端下线的函数 并且将online 字段设置位 0 这个字段就就代表在线状态
void Operatedb::clientOffline(const char *name)
{
    if(name == nullptr){
        return ;
    }
    QString sql = QString ("update user_info set online = 0 where name = '%1';").arg(name);
    QSqlQuery q;
    q.exec(sql);
}


int Operatedb::SelectUser(char *name)
{
    if(name == nullptr){
        return -1;
    }
    QSqlQuery q;
    QString  sql = QString ("select online from user_info where name='%1';" ).arg(name);
    q.exec(sql);
    if(!q.next()){
       qDebug()<<"用户不存在";
       return -1;
    }
    //通过这里进行返回查询到的第一列的值
   return q.value(0).toInt();  // 0 是离线 1 是在线

}

//显示在线用户功能
QStringList Operatedb::OnlineUser()
{
    QStringList strlist;
    strlist.clear();//防止乱码进行清空 效果 和初始化一样
    QSqlQuery q;
    QString  sql = QString ("select name from user_info where online = 1;" );
    q.exec(sql);
    while(q.next()){
        strlist.append(q.value(0).toString());
    }
    return strlist;
}

int Operatedb::Add_Friend(char *CurName, char *TarName)
{
        if(CurName== nullptr||TarName ==nullptr){
            return -1;
        }
        QSqlQuery q;

        QString sql = QString
                (R"(select * from friend
                 where (
                 friend_id = (select id  from user_info where name = '%1')
                 and
                 user_id =(select id from user_info where name = '%2')
                 )
                 or
                 (user_id = (select id from user_info where name = '%3')
                 and
                 friend_id = (select id from user_info where name = '%4')
                 );)").arg(CurName).arg(TarName).arg(CurName).arg(TarName);

        q.exec(sql);
        //表示两个人已经是好友了
        if(q.next()){
            qDebug()<<"已经是好友了";
            return -2;
        }

       sql = QString ("select online from user_info where name='%1';" ).arg(TarName);
       q.exec(sql);
       q.next();
        //通过这里进行返回查询到的第一列的值
        //有一个问题现在的ret 返回的是 0
       return q.value(0).toInt();  // 0 是离线 1 是在线
}


//服务器做同意好友的请求
bool Operatedb::Add_Friend_Agree(char *Curname, char *Tarname)
{
    if(Curname == nullptr || Tarname == nullptr){
        return false;
    }

    QString sql = QString(R"(insert into friend(user_id,friend_id)
                          select u1.id , u2.id from user_info u1 ,
                          user_info u2 where u1.name = '%1' and u2.name ='%2';)").arg(Curname).arg(Tarname);
    QSqlQuery q;
    q.exec(sql);
    q.next();
    qDebug()<<"执行成功";
    return true;
}


//在数据中提取好友列表的函数
QStringList Operatedb::RefreshFriend(char *Curname)
{
    QStringList strlist;
    strlist.clear();//防止乱码进行清空 效果 和初始化一样
    if(Curname == nullptr){
        return strlist;
    }
    qDebug()<<"\n\nRefreshFriend  Curname : "<<Curname;
    QSqlQuery q;
    QString  sql = QString (R"(select name from user_info
                            where id in(
                            select user_id from friend where friend_id = (select id from user_info where name = '%1')
                            union
                            select friend_id from friend where user_id = (select id from user_info where name = '%1')
                            ) and online  = 1;)" ).arg(Curname);
    q.exec(sql);
    while(q.next()){
        qDebug()<<q.value(0).toString();
        strlist.append(q.value(0).toString());
    }
    return strlist;
}

//删除好友
bool Operatedb::Delete_Friend(char *Curname, char *Tarname)
{
    if(Curname == nullptr || Tarname == nullptr){
        return false;
    }

    QSqlQuery q;
    QString sql  = QString(R"(select * from friend
                           where (
                           friend_id = (select id  from user_info where name = '%1')
                           and
                           user_id =(select id from user_info where name = '%2')
                           )
                           or
                           (user_id = (select id from user_info where name = '%1')
                           and
                           friend_id = (select id from user_info where name = '%2')
                           );)").arg(Curname).arg(Tarname);
    q.exec(sql);

    //如果没有记录代表已经不是好友了
    int id = -1;
    if(!q.next()){
        qDebug()<<q.value(0).toInt();
        return false;
    }
    //是好友继续执行删除操作
    id = q.value(0).toInt();
    sql =QString("delete from friend where id = %1;").arg(id);
    qDebug()<<"\n 执行的删除语句 ："<<sql;
    q.exec(sql);
    return true;
}




