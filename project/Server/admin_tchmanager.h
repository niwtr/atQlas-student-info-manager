#ifndef ADMIN_TCHMANAGER_H
#define ADMIN_TCHMANAGER_H

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
#include"admin_tchmanager_detail.h"
namespace Ui {
class admin_tchmanager;

}

class admin_tchmanager : public QMainWindow
{
    Q_OBJECT

public:
        Ui::admin_tchmanager *ui;
    explicit admin_tchmanager(QWidget *parent = 0, bool frozen=false);
    ~admin_tchmanager();

        bool table_is_ready=false;
        bool event(QEvent *event);
        bool gestureEvent(QGestureEvent *event);

        template<typename f>
        void cell_setter(int row, int col, f slot);
private slots:
        void on_tableWidget_cellChanged(int row, int column);

        void on_filter_edit_textChanged(const QString &arg1);

        void on_back_clicked();

        void on_new_tch_clicked();

        void on_OK_2_clicked();

        void on_NO_2_clicked();

private:

};



#endif // ADMIN_TCHMANAGER_H
