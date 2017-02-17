#ifndef TCH_MANAGER_DETAIL_H
#define TCH_MANAGER_DETAIL_H

#include <QMainWindow>
#include<string>
#include<vector>
#include<iostream>
#include<map>
#include <exception>
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
#include <QPropertyAnimation>
#include "sortapi.h"
namespace Ui {
class tch_manager_detail;
}

class tch_manager_detail : public QMainWindow
{
    Q_OBJECT

public:
    QWidget * the_parent;
    defschool::lesson_sptr ls;
    defschool::tec_sptr tch;
    explicit tch_manager_detail(QWidget *parent = 0, QWidget * the_parent=nullptr,defschool::tec_sptr tch=nullptr, defschool::lesson_sptr ls=nullptr);
    static constexpr auto normal_cell_width=100;
    //constexpr auto large_cell_width=2*normal_cell_width;
    static constexpr auto normal_cell_height=20;
    static constexpr auto table_max_count=20;
    static constexpr auto table_height=normal_cell_height*table_max_count;
    ~tch_manager_detail();

    bool event(QEvent *event);
    bool gestureEvent(QGestureEvent *event);

    bool table_is_ready=false;



private slots:
    void on_stu_table_cellDoubleClicked(int row, int column);

    void on_stu_table_cellChanged(int row, int column);


    void sortf(int logical){
        auto & stub=this->ls->get_student_binding();
        static bool ascendp=false;
        ascendp=not ascendp;

        switch(logical){
        case 0:{
            sort_by_slot(stub, [](auto stu){return stu->getSID();}, ascendp);
        }break;

        case 1:{
            sort_by_slot(stub,[](auto stu){return stu->getname();}, ascendp);
        }break;
        case 2:{
            sort_by_slot(stub, [=](auto stu){return stu->get_score_of_lesson(ls->getLID());},
            ascendp);
        }break;}}

    void on_refresh_clicked();

private:
    Ui::tch_manager_detail *ui;
};



#endif // TCH_MANAGER_DETAIL_H
