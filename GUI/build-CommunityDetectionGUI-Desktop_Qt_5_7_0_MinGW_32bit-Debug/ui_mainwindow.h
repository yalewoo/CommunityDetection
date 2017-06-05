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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTextEdit *path;
    QPushButton *pushButton;
    QPushButton *loadcomm1;
    QPushButton *loadcomm2;
    QLabel *graph_info;
    QLabel *comm2_info;
    QLabel *comm1_info;
    QLabel *comm12;
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
        path = new QTextEdit(centralWidget);
        path->setObjectName(QStringLiteral("path"));
        path->setGeometry(QRect(30, 20, 771, 51));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(100, 150, 93, 28));
        loadcomm1 = new QPushButton(centralWidget);
        loadcomm1->setObjectName(QStringLiteral("loadcomm1"));
        loadcomm1->setGeometry(QRect(90, 300, 93, 28));
        loadcomm2 = new QPushButton(centralWidget);
        loadcomm2->setObjectName(QStringLiteral("loadcomm2"));
        loadcomm2->setGeometry(QRect(90, 430, 93, 28));
        graph_info = new QLabel(centralWidget);
        graph_info->setObjectName(QStringLiteral("graph_info"));
        graph_info->setGeometry(QRect(300, 120, 361, 91));
        comm2_info = new QLabel(centralWidget);
        comm2_info->setObjectName(QStringLiteral("comm2_info"));
        comm2_info->setGeometry(QRect(290, 410, 361, 91));
        comm1_info = new QLabel(centralWidget);
        comm1_info->setObjectName(QStringLiteral("comm1_info"));
        comm1_info->setGeometry(QRect(290, 280, 361, 91));
        comm12 = new QLabel(centralWidget);
        comm12->setObjectName(QStringLiteral("comm12"));
        comm12->setGeometry(QRect(120, 520, 521, 91));
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
        pushButton->setText(QApplication::translate("MainWindow", "\350\275\275\345\205\245graph", 0));
        loadcomm1->setText(QApplication::translate("MainWindow", "\350\275\275\345\205\245\347\244\276\345\233\2421", 0));
        loadcomm2->setText(QApplication::translate("MainWindow", "\350\275\275\345\205\245\347\244\276\345\233\2422", 0));
        graph_info->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        comm2_info->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        comm1_info->setText(QApplication::translate("MainWindow", "TextLabel", 0));
        comm12->setText(QApplication::translate("MainWindow", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
