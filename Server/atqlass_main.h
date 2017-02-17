#ifndef ATQLASS_MAIN_H
#define ATQLASS_MAIN_H

#include <QMainWindow>
#include<iostream>
#include <QGestureEvent>
#include<QEvent>
//#include "login.h"
namespace Ui {
class atqlass_main;
}

class atqlass_main : public QMainWindow
{
    Q_OBJECT

public:
    explicit atqlass_main(QWidget *parent = 0);
    bool event(QEvent * event);
    bool gestureEvent(QGestureEvent *event);
    ~atqlass_main();


//    login * __nextWindow=nullptr;
private slots:
    //void on_pushButton_clicked();



private:
    Ui::atqlass_main *ui;
};

#endif // ATQLASS_MAIN_H
