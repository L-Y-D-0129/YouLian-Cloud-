#include "index.h"
#include "ui_index.h"
#include"client.h"
Index::Index(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Index)
{
    ui->setupUi(this);
    setWindowTitle(client::getInstance().m_Str_login_name);
}

Index::~Index()
{
    delete ui;
}

Friend *Index::getFriendpage()
{
    return ui->Friend_page;
}

File *Index::getFilepage()
{
    return ui->File_page;
}

Index &Index::getInstance()
{
    static Index instance;
    return instance;
}
//通过点击页面进行跳转页面 -- 跳转好友页面
void Index::on_friend_pb_clicked()
{
    //通过控制容器进行切换页面
    ui->stackedWidget->setCurrentIndex(0);
    //点击的同时改变按钮的颜色表示被选中
    ui->friend_pb->setStyleSheet("QPushButton{border:none;background-color:rgb(255, 255, 255);padding:20px;}");
    ui->file_pb->setStyleSheet("QPushButton{border:none;padding:20px;}");
}





void Index::on_file_pb_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->file_pb->setStyleSheet("QPushButton{border:none;background-color:rgb(255, 255, 255);padding:20px;}");
    ui->friend_pb->setStyleSheet("QPushButton{border:none;padding:20px;}");
}


