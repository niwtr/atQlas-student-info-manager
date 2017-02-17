#include "admin_cockpit.h"
#include "ui_admin_cockpit.h"
#include "admin_stmanager.h"
#include "admin_lesson_db.h"
#include "admin_tchmanager.h"
#include "common_animation.h"
#include "timemachine_main.h"
#include "capsule.h"
#include "kompiler.h"
#include "login.h"
admin_cockpit::admin_cockpit(QWidget *parent, bool frozen) :
    QMainWindow(parent),
    ui(new Ui::admin_cockpit)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
     setWindowFlags(windowFlags()&~Qt::WindowCloseButtonHint&~Qt::WindowContextHelpButtonHint);

     if(not frozen){
         ui->frame->setStyleSheet("border-image:url(:/atqres_ico/cockpit.jpg)");

            ui->pushButton_5->setStyleSheet("border-image:url(:/atqres_ico/thunderga.png)");
     }
     else {
         ui->frame->setStyleSheet("border-image:url(:/atqres_ico/wcockpit.jpg)");
      ui->pushButton_5->setStyleSheet("border-image:url(:/atqres_ico/piece.png)");
     }




    abstract_jump(ui->pushButton, 10, QEasingCurve::OutCurve);


    abstract_jump(ui->pushButton_2, 10, QEasingCurve::OutCurve);
    abstract_jump(ui->pushButton_3, 10, QEasingCurve::OutCurve);
    abstract_jump(ui->pushButton_4, 10, QEasingCurve::OutCurve);
    abstract_jump(ui->pushButton_5, 10, QEasingCurve::OutCurve);
    abstract_jump(ui->pushButton_6, 10, QEasingCurve::OutCurve);



    generic_breath(ui->pushButton,this, 0.8, 0.3, 2000);


    generic_breath(ui->pushButton_2,this, 0.8, 0.3, 2000);
    generic_breath(ui->pushButton_3,this, 0.8, 0.3, 2000);
    generic_breath(ui->pushButton_4,this, 0.8, 0.3, 2000);
    generic_breath(ui->pushButton_5,this, 0.8, 0.3, 2000);
    generic_breath(ui->pushButton_6,this, 0.8, 0.3, 2000);


    set_opacity(ui->back, this, 0.8);

}

admin_cockpit::~admin_cockpit()
{
    delete ui;
}

void admin_cockpit::on_pushButton_clicked()
{
    auto astm=new admin_stmanager(this, FROZENP);
    auto ani=scaler_once_animation_object(ui->pushButton, 10,QEasingCurve::OutCurve, 100);
    ani->start();
    connect(ani, SIGNAL(finished()), this, SLOT(close()));
    connect(ani, SIGNAL(finished()), astm, SLOT(show()));
}

void admin_cockpit::on_pushButton_3_clicked()
{
    auto alb=new admin_lesson_db(0, FROZENP);
    auto ani=scaler_once_animation_object(ui->pushButton_3, 10,QEasingCurve::OutCurve, 100);
    ani->start();
    connect(ani, SIGNAL(finished()), this, SLOT(close()));
    connect(ani, SIGNAL(finished()), alb, SLOT(show()));
}

void admin_cockpit::on_pushButton_2_clicked()
{
    auto atm=new admin_tchmanager(this, FROZENP);
    auto ani=scaler_once_animation_object(ui->pushButton_2, 10,QEasingCurve::OutCurve, 100);
    ani->start();
    connect(ani, SIGNAL(finished()), this, SLOT(close()));
    connect(ani, SIGNAL(finished()), atm, SLOT(show()));
}

void admin_cockpit::on_back_clicked()
{
    smoothed_generic_one_breath(ui->back, this, 0.8, 0, 500);
    auto l=new login();
    auto ani= fade_close(this, 2500);
    connect(ani, SIGNAL(finished()), l, SLOT(show()));
    csleep(1000);
}

void admin_cockpit::on_pushButton_4_clicked()
{
    auto tm=new timemachine_main(0,FROZENP);
    auto ani=scaler_once_animation_object(ui->pushButton_4, 10,QEasingCurve::OutCurve, 100);
    ani->start();
    connect(ani, SIGNAL(finished()), this, SLOT(close()));
    connect(ani, SIGNAL(finished()), tm, SLOT(show()));
}

void admin_cockpit::on_pushButton_5_clicked()
{
    if(FROZENP){

    auto tm=new capsule(0,X::X.get_global_time());
    auto ani=scaler_once_animation_object(ui->pushButton_5, 10,QEasingCurve::OutCurve, 100);
    ani->start();
    connect(ani, SIGNAL(finished()), this, SLOT(close()));
    connect(ani, SIGNAL(finished()), tm, SLOT(show()));
    }
    else{
        auto kp=new kompiler();
        auto ani=scaler_once_animation_object(ui->pushButton_5, 10,QEasingCurve::OutCurve, 100);
        ani->start();
        connect(ani, SIGNAL(finished()), this, SLOT(close()));
        connect(ani, SIGNAL(finished()), kp, SLOT(show()));
    }
}

void admin_cockpit::on_pushButton_6_clicked()
{
    auto ani=scaler_once_animation_object(ui->pushButton_6, 10,QEasingCurve::OutCurve, 100);
    ani->start();
    X::X.dump_to_xfile();
}
