#ifndef ATQLAS_MAIN_H
#define ATQLAS_MAIN_H

#include <QMainWindow>
#include <QGestureEvent>
#include "common_animation.h"
namespace Ui {
class atQlas_main;
}

class atQlas_main : public QMainWindow
{
    Q_OBJECT

public:
    explicit atQlas_main(QWidget *parent = 0);
    bool event(QEvent * event);
    bool gestureEvent(QGestureEvent *event);
    ~atQlas_main();

private:
    Ui::atQlas_main *ui;
};

#endif // ATQLAS_MAIN_H
