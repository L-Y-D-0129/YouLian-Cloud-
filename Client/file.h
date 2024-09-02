#ifndef FILE_H
#define FILE_H

#include <QListWidget>
#include <QWidget>
#include"protocol.h"

namespace Ui {
class File;
}

class File : public QWidget
{
    Q_OBJECT

public:
    explicit File(QWidget *parent = nullptr);
    ~File();

    void Send_Flush_File();

    void Updata_FileLW(QList<FileInfo *> filelist);
    QList<FileInfo *> m_filelist;

    QString m_strMoveFilePath;  //移动文件路径
    QString m_strMoveFileName;  //移动文件名
    QString m_strUpdataFilePath;//上传文件路径
    bool m_bUpload;             //是否在执行上传状态
    void uploadFile();
private slots:
    void on_mkDir_PB_clicked();

    void on_flushFile_PB_clicked();

    void on_delDir_PB_clicked();

    void on_rename_PB_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_return_PB_clicked();

    void on_moveFile_PB_clicked();

    void on_upload_PB_clicked();

private:
    Ui::File *ui;
    QString m_strCurPath;
    QString m_strUserPath;
};

#endif // FILE_H
