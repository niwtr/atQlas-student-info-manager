#include "atqlass_main.h"
#include "ui_atqlass_main.h"
#include "login.h"
#include "QPropertyAnimation"
#include "common_animation.h"

#include <qmovie.h>
atqlass_main::atqlass_main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::atqlass_main)
{
    ui->setupUi(this);


    setAttribute(Qt::WA_AcceptTouchEvents);
    //setAttribute(Qt::WA_TouchPadAcceptSingleTouchEvents);

    grabGesture(Qt::SwipeGesture);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);

    qApp->installEventFilter(this);




    generic_breath(ui->label, this, 1, 0, 2000);
    generic_breath(ui->label_2, this, 1, 0, 4000);
    abstract_hangout(ui->label_2,20, 10000);
    ui->centralWidget->setWindowTitle("AtQlas Alpha");
}



bool atqlass_main::event(QEvent * event){
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}


bool atqlass_main::gestureEvent(QGestureEvent *event)
{
    static login * __next_window=nullptr;//meant to avoid duplacating a window.

    if (event->gesture(Qt::PanGesture) and __next_window==nullptr)
    {
        __next_window=new login();
        this->close();
        __next_window->show();
    }

    return true;
}

atqlass_main::~atqlass_main()
{
    delete ui;
}



