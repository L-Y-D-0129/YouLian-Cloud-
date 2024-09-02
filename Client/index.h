#ifndef INDEX_H
#define INDEX_H

#include <QWidget>
#include"friend.h"
#include"file.h"
namespace Ui {
class Index;
}

class Index : public QWidget
{
    Q_OBJECT

public:

    ~Index();
    //因为要通过ui得到界面但是因为ui 是私有成员那么就要通过共有函数来访问私有变量
    Friend * getFriendpage();
    File * getFilepage();
    //构造单例模式
    static Index & getInstance();
private slots:
    void on_friend_pb_clicked();

    void on_file_pb_clicked();

private:
    Ui::Index *ui;
    explicit Index(QWidget *parent = nullptr);
    Index(const  Index & instance) = delete;
    Index & operator =(const Index &) = delete ;
};

#endif // INDEX_H
