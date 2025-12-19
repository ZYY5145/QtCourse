/********************************************************************************
** Form generated from reading UI file 'welcomeview.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WELCOMEVIEW_H
#define UI_WELCOMEVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WelcomeView
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btDepartment;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *btDoctor;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *btPatient;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *WelcomeView)
    {
        if (WelcomeView->objectName().isEmpty())
            WelcomeView->setObjectName("WelcomeView");
        WelcomeView->resize(540, 311);
        verticalLayout = new QVBoxLayout(WelcomeView);
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer = new QSpacerItem(20, 96, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btDepartment = new QPushButton(WelcomeView);
        btDepartment->setObjectName("btDepartment");
        btDepartment->setMinimumSize(QSize(81, 81));

        horizontalLayout->addWidget(btDepartment);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        btDoctor = new QPushButton(WelcomeView);
        btDoctor->setObjectName("btDoctor");
        btDoctor->setMinimumSize(QSize(81, 81));

        horizontalLayout->addWidget(btDoctor);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        btPatient = new QPushButton(WelcomeView);
        btPatient->setObjectName("btPatient");
        btPatient->setMinimumSize(QSize(81, 81));

        horizontalLayout->addWidget(btPatient);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 96, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        retranslateUi(WelcomeView);

        QMetaObject::connectSlotsByName(WelcomeView);
    } // setupUi

    void retranslateUi(QWidget *WelcomeView)
    {
        WelcomeView->setWindowTitle(QCoreApplication::translate("WelcomeView", "\346\254\242\350\277\216", nullptr));
        btDepartment->setText(QCoreApplication::translate("WelcomeView", "\347\247\221\345\256\244\347\256\241\347\220\206", nullptr));
        btDoctor->setText(QCoreApplication::translate("WelcomeView", "\345\214\273\347\224\237\347\256\241\347\220\206", nullptr));
        btPatient->setText(QCoreApplication::translate("WelcomeView", "\346\202\243\350\200\205\347\256\241\347\220\206", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WelcomeView: public Ui_WelcomeView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WELCOMEVIEW_H
