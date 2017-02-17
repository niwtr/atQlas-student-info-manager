#ifndef ADMIN_LESSON_DB_H
#define ADMIN_LESSON_DB_H

#include <QMainWindow>
#include "defschool.h"
#include <qstringlist>
#include <qstring>
#include <iostream>
#include <sstream>
#include <QMessageBox>
#include <QMenu>
#include<QGesture>
#include<QEvent>
#include<vector>
#include"xparser.h"
namespace Ui {
class admin_lesson_db;
}

class admin_lesson_db : public QMainWindow
{
    Q_OBJECT

public:


    static constexpr auto normal_cell_width=100;
    //constexpr auto large_cell_width=2*normal_cell_width;
    static constexpr auto normal_cell_height=22;
    static constexpr auto table_max_count=20;
    static constexpr auto table_height=normal_cell_height*table_max_count;

    explicit admin_lesson_db(QWidget *parent = 0, bool frozen=false);
    bool table_is_ready=false;
    ~admin_lesson_db();

private slots:
    void on_search_edit_textChanged(const QString &arg1);

    void on_result_table_cellEntered(int row, int column);

    void on_list_all_clicked();

    void on_result_table_cellChanged(int row, int column);

    void on_result_table_cellClicked(int row, int column);

    void on_new_button_clicked();

    void on_delete_button_clicked();


    void on_search_edit_returnPressed();

    void on_pushButton_clicked();

private:
    Ui::admin_lesson_db *ui;
};

#endif // ADMIN_LESSON_DB_H
