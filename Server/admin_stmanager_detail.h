#ifndef ADMIN_STMANAGER_DETAIL_H
#define ADMIN_STMANAGER_DETAIL_H

#include <QWidget>
#include "defschool.h"
#include <qstringlist>
#include <qstring>
#include <iostream>
#include <sstream>
#include <QMessageBox>
#include <vector>
#include<QGestureEvent>
#include<QListWidgetItem>

#include"admin_stmanager.h"

#include<QGesture>
#include<QEvent>
#include"QPropertyAnimation"
//#include<QGestureEvent>


constexpr auto NORMAL_MODE=0;
constexpr auto INSTANT_MODE=1;
namespace Ui {
class admin_stmanager_detail;
}



class admin_stmanager_detail : public QWidget
{
     using lesson_sptr=std::shared_ptr<defschool::lesson>;
    Q_OBJECT

public:
        QWidget * the_parent;

    explicit admin_stmanager_detail(bool frozen=false, QWidget *parent =0, QWidget * the_parent=0, defschool::stu_sptr stuptr=nullptr, int mode=NORMAL_MODE);
    ~admin_stmanager_detail();
    bool event(QEvent *event);
    bool gestureEvent(QGestureEvent *event);
    bool widget_is_ready=false;
    defschool::stu_sptr stu;
private slots:

    void on_name_edit_returnPressed();

    void on_grade_edit_returnPressed();

    void on_school_edit_returnPressed();

    void on_klass_edit_returnPressed();

    void on_add_lesson_button_clicked();

    void on_lesson_chooser_edit_textChanged(const QString &arg1);

    void on_recommendation_itemClicked(QListWidgetItem *item);

    void on_delete_lesson_button_clicked();

private:
    Ui::admin_stmanager_detail *ui;
};


#endif // ADMIN_STMANAGER_DETAIL_H
