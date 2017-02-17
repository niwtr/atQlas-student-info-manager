#ifndef ADMIN_TCHMANAGE_NEW_LESSON_H
#define ADMIN_TCHMANAGE_NEW_LESSON_H

#include <QDialog>

#include<string>
#include<vector>
#include<iostream>
#include<map>
#include<QGestureEvent>
#include "defschool.h"
#include <qstringlist>
#include <qstring>
#include <iostream>
#include <sstream>
#include <QMessageBox>
#include <QMenu>
#include<QGesture>
#include<QEvent>

namespace Ui {
class admin_tchmanage_new_lesson;
}

class admin_tchmanage_new_lesson : public QDialog
{
    Q_OBJECT

public:
    explicit admin_tchmanage_new_lesson(QWidget *parent = 0);
    ~admin_tchmanage_new_lesson();

private slots:
    void on_ok_clicked();

    void on_cancel_clicked();

private:
    Ui::admin_tchmanage_new_lesson *ui;
};

#endif // ADMIN_TCHMANAGE_NEW_LESSON_H
