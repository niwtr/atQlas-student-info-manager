#ifndef ADMIN_TCHMANAGER_DETAIL_H
#define ADMIN_TCHMANAGER_DETAIL_H

#include <QMainWindow>
#include "defschool.h"
#include "xparser.h"
#include <qstringlist>
#include <qstring>
#include <iostream>
#include <sstream>
#include <QMessageBox>
#include <vector>
#include<QGestureEvent>
#include<QListWidgetItem>
#include<QPushButton>
#include<QGesture>
#include<QEvent>
#include "common_animation.h"
namespace Ui {
class admin_tchmanager_detail;
}

class admin_tchmanager_detail : public QMainWindow
{
    Q_OBJECT

public:
    explicit admin_tchmanager_detail(bool frozen=false, QWidget *parent = 0, QWidget *the_parent=0, defschool::tec_sptr tecptr=nullptr);
    ~admin_tchmanager_detail();

    defschool::tec_sptr tch;
    QWidget * the_parent;
    bool event(QEvent *event);
    bool gestureEvent(QGestureEvent *event);

private slots:
    void on_name_edit_returnPressed();

    void on_school_edit_returnPressed();

    void on_add_lesson_button_clicked();

    void on_lesson_chooser_edit_textChanged(const QString &arg1);

    void on_recommendation_itemClicked(QListWidgetItem *item);

    void on_delete_button_clicked();



private:
    Ui::admin_tchmanager_detail *ui;
};



#endif // ADMIN_TCHMANAGER_DETAIL_H
