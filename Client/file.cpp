#include "file.h"
#include "ui_file.h"
#include "client.h"

#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
File::File(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::File)
{
    ui->setupUi(this);
    m_strCurPath = QString("./FileSystem/%1").arg(client::getInstance().m_Str_login_name);
    m_strUserPath = m_strCurPath;
    m_bUpload = false;
    //这样写由粘包问题 就是可能一下发送两个请求但是服务器只能响应一次
    Send_Flush_File();
}

File::~File()
{
    delete ui;
}

//封装刷新文件列表的函数
void File::Send_Flush_File()
{
    PDU * pdu = mkPDU(m_strCurPath.toStdString().size()+1);
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());;
    pdu->uiMsgType = ENUM_MSG_TYPE_FLUSH_FILE_REQUEST;
    client::getInstance().sendPDU(pdu);

}

void File::Updata_FileLW(QList<FileInfo *> filelist)
{
    //要先进行上一次的内存清空 清空上一次申请的空间也就是m_filelist
    foreach(FileInfo * pfilinfo, m_filelist){
        delete pfilinfo;
    }

    m_filelist.clear();
    m_filelist = filelist;
    ui->listWidget->clear();

    foreach(FileInfo * pfileinfo ,filelist){
        //每次申请空间
        QListWidgetItem * pItem = new QListWidgetItem;
        pItem->setText(pfileinfo->FileName);

        if(pfileinfo->FileType == 0){
            pItem->setIcon(QIcon(QPixmap(":/dir.png")));
        }else if(pfileinfo->FileType ==1){
            pItem->setIcon(QIcon(QPixmap(":/file.png")));
        }
        qDebug()<<"Updata_FileLW pfileinfo->FileName"<<pfileinfo->FileName;
        ui->listWidget->addItem(pItem);
    }

}

//上传文件函数
void File::uploadFile()
{
    //打开要上传的文件
    QFile file(m_strUpdataFilePath);
    if(!file.open(QIODevice::ReadOnly)){
        QMessageBox::warning(this,"上传文件","打开文件失败");
        return ;
    }
    //每次发送 4096
    PDU * datapdu = mkPDU(4096);
    datapdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_DATA_REQUEST;

    //打开文件成功代表已经开始上传文件了 那么就要改变上传状态的修改
    m_bUpload = true;
    qint64 ret = 0;
    while(true){
        ret =file.read(datapdu->caMsg,4096);
        if(ret < 0){
            QMessageBox::warning(this,"上传文件","上传文件失败");
            break;
        }
        if(ret == 0){//循环结束读完了
            break;
        }
        //根据ret 的大小来设置来这是PDU长度
        datapdu->uiMsgLen = ret;
        datapdu->uiPDULen = ret+sizeof(PDU);
        //因为我们下一次还要使用datapdu 所以不能使用我们封装好的函数 应调用socket中的write
        client::getInstance().getTCPsocket().write((char * )datapdu,datapdu->uiPDULen);
    }
    m_bUpload = false;
    file.close();
    free(datapdu);
    datapdu = nullptr;

}

//创建文件夹
void File::on_mkDir_PB_clicked()
{
    QString NewFileName =   QInputDialog::getText(this,"新建文件夹","新建文件夹名");
    if(NewFileName.isEmpty() || NewFileName.toStdString().size()>32){
        QMessageBox::information(this,"创建文件夹","文件夹名不合法");
        return;
    }
    qDebug()<<"NewFilename :"<<NewFileName;

    //加上\n的大小
    PDU * pdu = mkPDU(m_strCurPath.toStdString().size()+1);

    pdu->uiMsgType = ENUM_MSG_TYPE_MKDIR_REQUEST;

    memcpy(pdu->caData,NewFileName.toStdString().c_str(),32);
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());

    client::getInstance().sendPDU(pdu);

}

//刷新文件
void File::on_flushFile_PB_clicked()
{
    Send_Flush_File();
}

void File::on_delDir_PB_clicked()
{
    //获取选中当前项
    QListWidgetItem  *pItem = ui->listWidget->currentItem();

    if(pItem ==nullptr){
        QMessageBox::information(this,"删除文件","请选择要删除的文件夹");
        return ;
    }
    //获取选中的文件名
    QString TarFileName = pItem->text();
    //如果用户选则的是文件应该给出警告
    foreach(FileInfo * pFileInfo ,m_filelist){
        if(pFileInfo->FileName == TarFileName && pFileInfo->FileType !=0){
             QMessageBox::information(this,"删除文件","要删除的不是文件夹！");
             return;
        }
    }
    //给用户弹窗提示询问是否删除
    int ret = QMessageBox::question(this,"删除文件",QString("是否确认删除文件%1").arg(TarFileName));

    if(ret == QMessageBox::No){//如果选择不删除那么结束
        return ;
    }
    //对当前路径和选中的文件名进行拼接
    QString TarPath = QString("%1/%2").arg(m_strCurPath).arg(TarFileName);
    //存到PDU里面进行发送
    PDU * pdu = mkPDU(TarPath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_DELETE_DIR_REQUEST;
    memcpy(pdu->caMsg,TarPath.toStdString().c_str(),TarPath.toStdString().size());
    client::getInstance().sendPDU(pdu);
}

//重命名
void File::on_rename_PB_clicked()
{
    QListWidgetItem  *pItem = ui->listWidget->currentItem();

    if(pItem ==nullptr){
        QMessageBox::information(this,"重命名","请选择要重命名的文件夹");
        return ;
    }
    //获取选中旧的文件名
    QString OldFileName = pItem->text();
    QString NewFileName =   QInputDialog::getText(this,"重命名","新建文件名");
    if(NewFileName.isEmpty() || NewFileName.toStdString().size()>32){
        QMessageBox::information(this,"重命名文件夹","文件夹名不合法");
        return;
    }
    qDebug()<<"NewFilename :"<<NewFileName;

    //加上\n的大小
    PDU * pdu = mkPDU(m_strCurPath.toStdString().size()+1);

    pdu->uiMsgType = ENUM_MSG_TYPE_RENAME_FILE_REQUEST;

    memcpy(pdu->caData,OldFileName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,NewFileName.toStdString().c_str(),32);
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());

    client::getInstance().sendPDU(pdu);
}

//点击进入文件夹
void File::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString TarDirName = item->text();
    foreach(FileInfo * pFileInfo ,m_filelist){
        if(pFileInfo->FileName == TarDirName && pFileInfo->FileType !=0){
             QMessageBox::information(this,"进入文件","要进入的不是文件夹！");
             return;
        }
    }
    m_strCurPath = QString("%1/%2").arg(m_strCurPath).arg(TarDirName);
    Send_Flush_File();
}

//返回上一级
void File::on_return_PB_clicked()
{
      if(m_strCurPath == m_strUserPath){//为了防止用户进入到其他用户的目录
          //给出警告
          QMessageBox::warning(this,"返回上一级","返回失败：已在顶层目录");
          return ;
      }
      int index =  m_strCurPath.lastIndexOf('/');
      m_strCurPath.remove(index,m_strCurPath.size()-index);
      Send_Flush_File();

}

//移动文件
void File::on_moveFile_PB_clicked()
{
    if(ui->moveFile_PB->text() == "移动文件"){
        QListWidgetItem  *pItem = ui->listWidget->currentItem();

        if(pItem ==nullptr){
            QMessageBox::information(this,"移动文件","请选择要移动的文件");
            return ;
        }
        QMessageBox::information(this,"移动文件","请选择要移动到的目录");
        m_strMoveFileName = pItem->text();
        m_strMoveFilePath = m_strCurPath+ '/' +m_strMoveFileName;

        qDebug()<<"on_moveFile_PB_clicked : m_strMoveFileName :"<<m_strMoveFileName;
        qDebug()<<"on_moveFile_PB_clicked : m_strMoveFilePath :"<<m_strMoveFilePath;
        ui->moveFile_PB->setText("确认/取消");
        return ;
    }

    QString tarPath ;
    QString boxMsg;
    QListWidgetItem  *pItem = ui->listWidget->currentItem();
    if(pItem == nullptr){//用户没用选中，目标路径就是当前路径
        tarPath = m_strCurPath+ '/' +m_strMoveFileName;
        boxMsg = "是否移动到当前目录下";
    }else {
        QString TarDirName = pItem->text();
        //如果用户选则的是文件应该给出警告
        foreach(FileInfo * pFileInfo ,m_filelist){
            if(pFileInfo->FileName == TarDirName && pFileInfo->FileType !=0){
                 QMessageBox::information(this,"移动文件","要移动到的不是文件夹！");
                 return;
            }
        }
        tarPath = m_strCurPath+ '/' +TarDirName + '/' +m_strMoveFileName;
         boxMsg = QString("是否移动到 %1 目录下").arg(TarDirName);
    }
    int ret = QMessageBox::information(this,"移动文件",boxMsg,"确认","取消");
    ui->moveFile_PB->setText("移动文件");
    if(ret != 0 ){
        return ;
    }
    int strcLen = m_strMoveFilePath.toStdString().size();
    int tarLen = tarPath.toStdString().size();
    PDU * pdu = mkPDU(strcLen + tarLen +1);
    pdu ->uiMsgType = ENUM_MSG_TYPE_MOVE_FILE_REQUEST;
    memcpy(pdu->caData , &strcLen,sizeof (int));
    memcpy(pdu->caData+32 , &tarLen,sizeof (int));
    memcpy(pdu->caMsg,m_strMoveFilePath.toStdString().c_str(),strcLen);
    memcpy(pdu->caMsg+strcLen,tarPath.toStdString().c_str(),tarLen);

    client::getInstance().sendPDU(pdu);
}


//上传文件
void File::on_upload_PB_clicked()
{
    if(m_bUpload){//表示是否在上传的变量判断如果有在上传文件那么给一个提醒说已经有文件在上传稍等
        QMessageBox::warning(this,"上传文件","已有文件在上传，请稍等");
        return ;
    }
    m_strUpdataFilePath = QFileDialog::getOpenFileName();
    qDebug()<<"on_upload_PB_clicked :m_strUpdataFilePath: "<<m_strUpdataFilePath;
    int index = m_strUpdataFilePath.lastIndexOf('/');
    QString UpdataFileName = m_strUpdataFilePath.right(m_strUpdataFilePath.toStdString().size()-index -1);
    qDebug()<<"on_upload_PB_clicked :UpdataFileName: "<<UpdataFileName;

    QFile file(m_strUpdataFilePath);
    qint64 FileSize = file.size();
    qDebug()<<"on_upload_PB_clicked :FileSize: "<<FileSize;
    //这里放的当前路径的大小而不是选择文件的大小

    PDU * pdu = mkPDU(m_strCurPath.toStdString().size()+1);
    pdu->uiMsgType = ENUM_MSG_TYPE_UPLOAD_FILE_REQUEST;
    memcpy(pdu->caData,UpdataFileName.toStdString().c_str(),32);
    memcpy(pdu->caData+32,&FileSize,sizeof(qint64));
    memcpy(pdu->caMsg,m_strCurPath.toStdString().c_str(),m_strCurPath.toStdString().size());

    client::getInstance().sendPDU(pdu);

}
