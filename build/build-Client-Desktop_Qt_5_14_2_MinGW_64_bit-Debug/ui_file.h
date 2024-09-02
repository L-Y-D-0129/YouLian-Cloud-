/********************************************************************************
** Form generated from reading UI file 'file.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILE_H
#define UI_FILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_File
{
public:
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget;
    QGridLayout *gridLayout;
    QPushButton *flushFile_PB;
    QPushButton *return_PB;
    QPushButton *mkDir_PB;
    QPushButton *rename_PB;
    QPushButton *delDir_PB;
    QPushButton *shareFile_PB;
    QPushButton *dirFile_PB;
    QPushButton *download_PB;
    QPushButton *moveFile_PB;
    QPushButton *upload_PB;

    void setupUi(QWidget *File)
    {
        if (File->objectName().isEmpty())
            File->setObjectName(QString::fromUtf8("File"));
        File->resize(855, 649);
        horizontalLayout = new QHBoxLayout(File);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 5, 0);
        listWidget = new QListWidget(File);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setFrameShape(QFrame::NoFrame);

        horizontalLayout->addWidget(listWidget);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        flushFile_PB = new QPushButton(File);
        flushFile_PB->setObjectName(QString::fromUtf8("flushFile_PB"));
        flushFile_PB->setMinimumSize(QSize(0, 50));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(14);
        flushFile_PB->setFont(font);

        gridLayout->addWidget(flushFile_PB, 0, 0, 1, 1);

        return_PB = new QPushButton(File);
        return_PB->setObjectName(QString::fromUtf8("return_PB"));
        return_PB->setMinimumSize(QSize(0, 50));
        return_PB->setFont(font);

        gridLayout->addWidget(return_PB, 0, 1, 1, 1);

        mkDir_PB = new QPushButton(File);
        mkDir_PB->setObjectName(QString::fromUtf8("mkDir_PB"));
        mkDir_PB->setMinimumSize(QSize(0, 50));
        mkDir_PB->setFont(font);

        gridLayout->addWidget(mkDir_PB, 1, 0, 1, 1);

        rename_PB = new QPushButton(File);
        rename_PB->setObjectName(QString::fromUtf8("rename_PB"));
        rename_PB->setMinimumSize(QSize(0, 50));
        rename_PB->setFont(font);

        gridLayout->addWidget(rename_PB, 1, 1, 1, 1);

        delDir_PB = new QPushButton(File);
        delDir_PB->setObjectName(QString::fromUtf8("delDir_PB"));
        delDir_PB->setMinimumSize(QSize(0, 50));
        delDir_PB->setFont(font);

        gridLayout->addWidget(delDir_PB, 2, 0, 1, 1);

        shareFile_PB = new QPushButton(File);
        shareFile_PB->setObjectName(QString::fromUtf8("shareFile_PB"));
        shareFile_PB->setMinimumSize(QSize(0, 50));
        shareFile_PB->setFont(font);

        gridLayout->addWidget(shareFile_PB, 2, 1, 1, 1);

        dirFile_PB = new QPushButton(File);
        dirFile_PB->setObjectName(QString::fromUtf8("dirFile_PB"));
        dirFile_PB->setMinimumSize(QSize(0, 50));
        dirFile_PB->setFont(font);

        gridLayout->addWidget(dirFile_PB, 3, 0, 1, 1);

        download_PB = new QPushButton(File);
        download_PB->setObjectName(QString::fromUtf8("download_PB"));
        download_PB->setMinimumSize(QSize(0, 50));
        download_PB->setFont(font);

        gridLayout->addWidget(download_PB, 3, 1, 1, 1);

        moveFile_PB = new QPushButton(File);
        moveFile_PB->setObjectName(QString::fromUtf8("moveFile_PB"));
        moveFile_PB->setMinimumSize(QSize(0, 50));
        moveFile_PB->setFont(font);

        gridLayout->addWidget(moveFile_PB, 4, 0, 1, 1);

        upload_PB = new QPushButton(File);
        upload_PB->setObjectName(QString::fromUtf8("upload_PB"));
        upload_PB->setMinimumSize(QSize(0, 50));
        upload_PB->setFont(font);

        gridLayout->addWidget(upload_PB, 4, 1, 1, 1);


        horizontalLayout->addLayout(gridLayout);


        retranslateUi(File);

        QMetaObject::connectSlotsByName(File);
    } // setupUi

    void retranslateUi(QWidget *File)
    {
        File->setWindowTitle(QCoreApplication::translate("File", "Form", nullptr));
        flushFile_PB->setText(QCoreApplication::translate("File", "\345\210\267\346\226\260\346\226\207\344\273\266", nullptr));
        return_PB->setText(QCoreApplication::translate("File", "\350\277\224\345\233\236\344\270\212\344\270\200\347\272\247", nullptr));
        mkDir_PB->setText(QCoreApplication::translate("File", "\345\210\233\345\273\272\346\226\207\344\273\266\345\244\271", nullptr));
        rename_PB->setText(QCoreApplication::translate("File", "\351\207\215\345\221\275\345\220\215", nullptr));
        delDir_PB->setText(QCoreApplication::translate("File", "\345\210\240\351\231\244\346\226\207\344\273\266\345\244\271", nullptr));
        shareFile_PB->setText(QCoreApplication::translate("File", "\345\210\206\344\272\253\346\226\207\344\273\266", nullptr));
        dirFile_PB->setText(QCoreApplication::translate("File", "\345\210\240\351\231\244\346\226\207\344\273\266", nullptr));
        download_PB->setText(QCoreApplication::translate("File", "\344\270\213\350\275\275\346\226\207\344\273\266", nullptr));
        moveFile_PB->setText(QCoreApplication::translate("File", "\347\247\273\345\212\250\346\226\207\344\273\266", nullptr));
        upload_PB->setText(QCoreApplication::translate("File", "\344\270\212\344\274\240\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class File: public Ui_File {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILE_H
