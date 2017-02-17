#include "atqlas_main.h"
#include "ui_atqlas_main.h"
#include "login.h"
atQlas_main::atQlas_main(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::atQlas_main)
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

atQlas_main::~atQlas_main()
{
    delete ui;
}



bool atQlas_main::event(QEvent * event){
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}


bool atQlas_main::gestureEvent(QGestureEvent *event)
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
