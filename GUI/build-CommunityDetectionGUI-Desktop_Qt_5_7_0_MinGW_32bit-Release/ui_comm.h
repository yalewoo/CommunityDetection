/********************************************************************************
** Form generated from reading UI file 'comm.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMM_H
#define UI_COMM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Comm
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *comm_name_2;
    QLabel *comm_name;
    QLabel *comm_info;
    QTableWidget *t;

    void setupUi(QWidget *Comm)
    {
        if (Comm->objectName().isEmpty())
            Comm->setObjectName(QStringLiteral("Comm"));
        Comm->resize(507, 456);
        verticalLayout = new QVBoxLayout(Comm);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        comm_name_2 = new QLabel(Comm);
        comm_name_2->setObjectName(QStringLiteral("comm_name_2"));

        verticalLayout->addWidget(comm_name_2);

        comm_name = new QLabel(Comm);
        comm_name->setObjectName(QStringLiteral("comm_name"));
        comm_name->setMinimumSize(QSize(0, 41));

        verticalLayout->addWidget(comm_name);

        comm_info = new QLabel(Comm);
        comm_info->setObjectName(QStringLiteral("comm_info"));
        comm_info->setMinimumSize(QSize(0, 71));
        comm_info->setMaximumSize(QSize(16777215, 71));

        verticalLayout->addWidget(comm_info);

        t = new QTableWidget(Comm);
        t->setObjectName(QStringLiteral("t"));

        verticalLayout->addWidget(t);


        retranslateUi(Comm);

        QMetaObject::connectSlotsByName(Comm);
    } // setupUi

    void retranslateUi(QWidget *Comm)
    {
        Comm->setWindowTitle(QApplication::translate("Comm", "Form", 0));
        comm_name_2->setText(QApplication::translate("Comm", "TextLabel", 0));
        comm_name->setText(QApplication::translate("Comm", "TextLabel", 0));
        comm_info->setText(QApplication::translate("Comm", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class Comm: public Ui_Comm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMM_H
