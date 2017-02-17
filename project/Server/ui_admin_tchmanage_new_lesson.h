/********************************************************************************
** Form generated from reading UI file 'admin_tchmanage_new_lesson.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADMIN_TCHMANAGE_NEW_LESSON_H
#define UI_ADMIN_TCHMANAGE_NEW_LESSON_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_admin_tchmanage_new_lesson
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QPushButton *ok;
    QLabel *name_label;
    QLineEdit *name_edit;
    QPushButton *cancel;
    QLabel *lid_label;
    QLabel *grade_label;
    QLineEdit *optp_edit;
    QLineEdit *lid_edit;
    QLineEdit *credit_edit;
    QLabel *optp_label;
    QLineEdit *grade_edit;
    QLabel *credit_label;

    void setupUi(QDialog *admin_tchmanage_new_lesson)
    {
        if (admin_tchmanage_new_lesson->objectName().isEmpty())
            admin_tchmanage_new_lesson->setObjectName(QStringLiteral("admin_tchmanage_new_lesson"));
        admin_tchmanage_new_lesson->resize(400, 300);
        gridLayoutWidget = new QWidget(admin_tchmanage_new_lesson);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(50, 10, 311, 271));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        ok = new QPushButton(gridLayoutWidget);
        ok->setObjectName(QStringLiteral("ok"));

        gridLayout->addWidget(ok, 5, 1, 1, 1);

        name_label = new QLabel(gridLayoutWidget);
        name_label->setObjectName(QStringLiteral("name_label"));

        gridLayout->addWidget(name_label, 1, 0, 1, 1);

        name_edit = new QLineEdit(gridLayoutWidget);
        name_edit->setObjectName(QStringLiteral("name_edit"));

        gridLayout->addWidget(name_edit, 1, 1, 1, 1);

        cancel = new QPushButton(gridLayoutWidget);
        cancel->setObjectName(QStringLiteral("cancel"));

        gridLayout->addWidget(cancel, 5, 0, 1, 1);

        lid_label = new QLabel(gridLayoutWidget);
        lid_label->setObjectName(QStringLiteral("lid_label"));

        gridLayout->addWidget(lid_label, 0, 0, 1, 1);

        grade_label = new QLabel(gridLayoutWidget);
        grade_label->setObjectName(QStringLiteral("grade_label"));

        gridLayout->addWidget(grade_label, 3, 0, 1, 1);

        optp_edit = new QLineEdit(gridLayoutWidget);
        optp_edit->setObjectName(QStringLiteral("optp_edit"));

        gridLayout->addWidget(optp_edit, 4, 1, 1, 1);

        lid_edit = new QLineEdit(gridLayoutWidget);
        lid_edit->setObjectName(QStringLiteral("lid_edit"));

        gridLayout->addWidget(lid_edit, 0, 1, 1, 1);

        credit_edit = new QLineEdit(gridLayoutWidget);
        credit_edit->setObjectName(QStringLiteral("credit_edit"));

        gridLayout->addWidget(credit_edit, 2, 1, 1, 1);

        optp_label = new QLabel(gridLayoutWidget);
        optp_label->setObjectName(QStringLiteral("optp_label"));

        gridLayout->addWidget(optp_label, 4, 0, 1, 1);

        grade_edit = new QLineEdit(gridLayoutWidget);
        grade_edit->setObjectName(QStringLiteral("grade_edit"));

        gridLayout->addWidget(grade_edit, 3, 1, 1, 1);

        credit_label = new QLabel(gridLayoutWidget);
        credit_label->setObjectName(QStringLiteral("credit_label"));

        gridLayout->addWidget(credit_label, 2, 0, 1, 1);


        retranslateUi(admin_tchmanage_new_lesson);

        QMetaObject::connectSlotsByName(admin_tchmanage_new_lesson);
    } // setupUi

    void retranslateUi(QDialog *admin_tchmanage_new_lesson)
    {
        admin_tchmanage_new_lesson->setWindowTitle(QApplication::translate("admin_tchmanage_new_lesson", "Dialog", 0));
        ok->setText(QApplication::translate("admin_tchmanage_new_lesson", "ok", 0));
        name_label->setText(QApplication::translate("admin_tchmanage_new_lesson", "Name", 0));
        cancel->setText(QApplication::translate("admin_tchmanage_new_lesson", "cancel", 0));
        lid_label->setText(QApplication::translate("admin_tchmanage_new_lesson", "LID", 0));
        grade_label->setText(QApplication::translate("admin_tchmanage_new_lesson", "Grade", 0));
        optp_label->setText(QApplication::translate("admin_tchmanage_new_lesson", "Optional?", 0));
        credit_label->setText(QApplication::translate("admin_tchmanage_new_lesson", "Credit", 0));
    } // retranslateUi

};

namespace Ui {
    class admin_tchmanage_new_lesson: public Ui_admin_tchmanage_new_lesson {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADMIN_TCHMANAGE_NEW_LESSON_H
