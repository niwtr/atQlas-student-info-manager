#ifndef TCH_MANAGE_H
#define TCH_MANAGE_H

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
#include <functional>
namespace Ui {
class tch_manage;

constexpr static auto namedit_pos=QRect(460, 30, 250, 30);
constexpr static auto idedit_pos=QRect(460, 90, 250, 30);
constexpr static auto schooledit_pos=QRect(460, 150, 250, 30);
}

class tch_manage : public QMainWindow
{
    Q_OBJECT

public:
    explicit tch_manage(QWidget *parent = 0, defschool::tec_sptr tch=nullptr);
    static constexpr auto normal_cell_width=100;
    //constexpr auto large_cell_width=2*normal_cell_width;
    static constexpr auto normal_cell_height=20;
    static constexpr auto table_max_count=20;
    static constexpr auto table_height=normal_cell_height*table_max_count;
    defschool::tec_sptr tch;

    bool event(QEvent *event);
    bool gestureEvent(QGestureEvent *event);

    ~tch_manage();

private slots:


    void on_add_new_button_clicked();

    void on_filter_edit_textChanged(const QString &arg1);

    void on_add_all_button_clicked();

    void on_add_selected_button_clicked();

    void on_delete_button_clicked();

    void on_back_clicked();

    void on_pushButton_clicked();

    void sortf(int logical){
#define THE_LESSON defschool::karacter::lb_binding->lesson_assoc(lid)
#define sort_in_container(X,Y) sort_by_slot(this->tch->get_lesson_ref(),X,Y)
#define slot(slt) THE_LESSON->get##slt;
#define sort_slot(name)\
    sort_in_container([](auto lid){return slot(name());},ascendp)
#define case_in_slot(name)\
    case name:{\
    sort_slot(name);\
    }break;

         //you will never understand this.
        static bool ascendp=false;
        ascendp=not ascendp;
        enum{LID=0, name, credit, grade, opt_p};
        switch(logical){
        case_in_slot(LID);
        case_in_slot(name);
        case_in_slot(credit);
        case_in_slot(grade);
        case_in_slot(opt_p);
        }


#undef the_lesson
#undef sort_in_container
#undef slot
#undef sort_slot
#undef case_in_slot
    }

    void on_refresh_clicked();

private:
    Ui::tch_manage *ui;
};

#endif // TCH_MANAGE_H
