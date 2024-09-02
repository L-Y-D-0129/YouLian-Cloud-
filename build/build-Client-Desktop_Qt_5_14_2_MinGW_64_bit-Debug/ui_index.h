/********************************************************************************
** Form generated from reading UI file 'index.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INDEX_H
#define UI_INDEX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <file.h>
#include <friend.h>

QT_BEGIN_NAMESPACE

class Ui_Index
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QPushButton *friend_pb;
    QPushButton *file_pb;
    QStackedWidget *stackedWidget;
    Friend *Friend_page;
    File *File_page;

    void setupUi(QWidget *Index)
    {
        if (Index->objectName().isEmpty())
            Index->setObjectName(QString::fromUtf8("Index"));
        Index->resize(1062, 686);
        horizontalLayout = new QHBoxLayout(Index);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        friend_pb = new QPushButton(Index);
        friend_pb->setObjectName(QString::fromUtf8("friend_pb"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(friend_pb->sizePolicy().hasHeightForWidth());
        friend_pb->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(18);
        friend_pb->setFont(font);
        friend_pb->setStyleSheet(QString::fromUtf8("border:none;background-color:rgb(255, 255, 255);padding:20px;"));

        verticalLayout->addWidget(friend_pb);

        file_pb = new QPushButton(Index);
        file_pb->setObjectName(QString::fromUtf8("file_pb"));
        sizePolicy.setHeightForWidth(file_pb->sizePolicy().hasHeightForWidth());
        file_pb->setSizePolicy(sizePolicy);
        file_pb->setMinimumSize(QSize(100, 0));
        file_pb->setFont(font);
        file_pb->setStyleSheet(QString::fromUtf8("border:none;padding:20px;"));

        verticalLayout->addWidget(file_pb);


        horizontalLayout->addLayout(verticalLayout);

        stackedWidget = new QStackedWidget(Index);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        stackedWidget->setMinimumSize(QSize(100, 0));
        Friend_page = new Friend();
        Friend_page->setObjectName(QString::fromUtf8("Friend_page"));
        stackedWidget->addWidget(Friend_page);
        File_page = new File();
        File_page->setObjectName(QString::fromUtf8("File_page"));
        stackedWidget->addWidget(File_page);

        horizontalLayout->addWidget(stackedWidget);


        retranslateUi(Index);

        QMetaObject::connectSlotsByName(Index);
    } // setupUi

    void retranslateUi(QWidget *Index)
    {
        Index->setWindowTitle(QCoreApplication::translate("Index", "Form", nullptr));
        friend_pb->setText(QCoreApplication::translate("Index", "\345\245\275\n"
"\345\217\213", nullptr));
        file_pb->setText(QCoreApplication::translate("Index", "\346\226\207\n"
"\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Index: public Ui_Index {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INDEX_H
