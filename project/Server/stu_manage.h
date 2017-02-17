#ifndef STU_MANAGE_H
#define STU_MANAGE_H

#include <QMainWindow>
#include<string>
#include<vector>
#include<iostream>
#include<map>

#include<QGestureEvent>

#include"xparser.h"
#include "defschool.h"
#include <qstringlist>
#include <qstring>
#include <iostream>
#include <sstream>
#include <QMessageBox>
#include <QMenu>
#include<QGesture>
#include<QEvent>
#include "sortapi.h"
namespace Ui {
class stu_manage;
}

class stu_manage : public QMainWindow
{
    Q_OBJECT

public:
    explicit stu_manage(QWidget *parent = 0, defschool::stu_sptr stu=nullptr);
    ~stu_manage();
    static constexpr auto normal_cell_width=100;
    static constexpr auto normal_cell_height=20;
    static constexpr auto table_max_count=20;
    static constexpr auto table_height=normal_cell_height*table_max_count;
    defschool::stu_sptr stu;



private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_delete_course_button_clicked();

    void on_drawer_currentChanged(int index);

    void on_back_clicked();

    void sortf(int logical){
        static bool ascendp=false;ascendp=not ascendp;
        enum{lid=0, name, credit, grade, optp, teacher,score};
        auto & cont=stu->get_stu_lesson();
        switch(logical){
        case lid:{
            sort_by_slot(cont, [](auto the_pair){return the_pair.first;}, ascendp);

        }break;
        case name:
            sort_by_slot(cont, [=](auto the_pair){return stu->assoc_lesson_base(the_pair.first)->getname();},ascendp);
            break;

        case credit:
            sort_by_slot(cont, [=](auto the_pair){return stu->assoc_lesson_base(the_pair.first)->getcredit();},ascendp);
            break;
        case grade:
            sort_by_slot(cont, [=](auto the_pair){return stu->assoc_lesson_base(the_pair.first)->getgrade();},ascendp);
            break;
        case optp:
            sort_by_slot(cont, [=](auto the_pair){return stu->assoc_lesson_base(the_pair.first)->getopt_p();},ascendp);
            break;
        case teacher:break;
        case score:
            sort_by_slot(cont, [=](auto the_pair){return stu->get_score_of_lesson(the_pair.first);},ascendp);
            break;
        default:break;
        }

    }

    void on_delete_course_button_2_clicked();

private:
    Ui::stu_manage *ui;
};

#endif // STU_MANAGE_H
