/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "comm.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QLabel *graph_info;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton;
    QLabel *info;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    Comm *comm1;
    Comm *comm2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *add1;
    QPushButton *add2;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *getsub;
    QLabel *subinfo;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(836, 690);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        graph_info = new QLabel(centralWidget);
        graph_info->setObjectName(QStringLiteral("graph_info"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(graph_info->sizePolicy().hasHeightForWidth());
        graph_info->setSizePolicy(sizePolicy);
        graph_info->setMinimumSize(QSize(0, 41));

        verticalLayout->addWidget(graph_info);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMaximumSize(QSize(93, 16777215));

        horizontalLayout_2->addWidget(pushButton);

        info = new QLabel(centralWidget);
        info->setObjectName(QStringLiteral("info"));
        sizePolicy.setHeightForWidth(info->sizePolicy().hasHeightForWidth());
        info->setSizePolicy(sizePolicy);
        info->setMinimumSize(QSize(0, 81));

        horizontalLayout_2->addWidget(info);


        verticalLayout->addLayout(horizontalLayout_2);

        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        comm1 = new Comm(widget);
        comm1->setObjectName(QStringLiteral("comm1"));

        horizontalLayout->addWidget(comm1);

        comm2 = new Comm(widget);
        comm2->setObjectName(QStringLiteral("comm2"));

        horizontalLayout->addWidget(comm2);


        verticalLayout->addWidget(widget);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        add1 = new QPushButton(centralWidget);
        add1->setObjectName(QStringLiteral("add1"));
        add1->setMaximumSize(QSize(93, 28));

        horizontalLayout_3->addWidget(add1);

        add2 = new QPushButton(centralWidget);
        add2->setObjectName(QStringLiteral("add2"));
        add2->setMaximumSize(QSize(93, 28));

        horizontalLayout_3->addWidget(add2);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        getsub = new QPushButton(centralWidget);
        getsub->setObjectName(QStringLiteral("getsub"));
        getsub->setMaximumSize(QSize(93, 28));

        horizontalLayout_4->addWidget(getsub);

        subinfo = new QLabel(centralWidget);
        subinfo->setObjectName(QStringLiteral("subinfo"));

        horizontalLayout_4->addWidget(subinfo);


        verticalLayout->addLayout(horizontalLayout_4);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 836, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        graph_info->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        pushButton->setText(QApplication::translate("MainWindow", "\345\244\215\345\210\266\346\225\260\345\200\274", 0));
        info->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        add1->setText(QApplication::translate("MainWindow", "add", 0));
        add2->setText(QApplication::translate("MainWindow", "add", 0));
        getsub->setText(QApplication::translate("MainWindow", "\347\224\237\346\210\220\345\255\220\345\233\276", 0));
        subinfo->setText(QApplication::translate("MainWindow", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
