#ifndef ADMIN_STMANAGER_H
#define ADMIN_STMANAGER_H

#include <QMainWindow>
#include<string>
#include<vector>
#include<iostream>
#include<map>
#include "admin_stmanager_detail.h"
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
//#include<QGestureEvent>
namespace Ui {


class admin_stmanager;



}
class admin_stmanager : public QMainWindow
{
    Q_OBJECT

public:
        Ui::admin_stmanager *ui;//i moved this one here.
    explicit admin_stmanager(QWidget *parent = 0, bool frozen=false);
    ~admin_stmanager();

    const std::string fw_not_found_error_string="ME:FW_NOT_FOUND";
    std::string fake_widget_get(int row, int col);
    bool fake_widget_set(int row, int col, std::string content);

    bool fake_widget_is_ready=false; //mark for wether the widget is in use.
    //this should be set false during initialization of that fake.
    bool event(QEvent *event);
    bool gestureEvent(QGestureEvent *event);
    template<typename Fn>
    void set_cell_and_fake(int row, int col, Fn method);



private slots:
    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_tableWidget_cellChanged(int row, int column);


    void on_tableWidget_cellClicked(int row, int column);


    void on_filter_edit_textChanged(const QString &arg1);



    void on_back_clicked();

    void on_new_stu_clicked();

    void on_OK_clicked();

    void on_NO_clicked();

private:


    std::map<std::string, std::string> fake_widget;


    void show_menu_on_cell(void);
};



#endif // ADMIN_STMANAGER_H
