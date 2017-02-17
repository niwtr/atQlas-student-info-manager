#include "login.h"
#include "admin_cockpit.h"
#include "stu_manage.h"
#include "tch_manage.h"
#include "ui_login.h"
#include <iostream>
#include <QString>
#include <string>
#include <account.h>
#include<QPropertyAnimation>
#include "common_animation.h"
#include "TIME_MACHINE.h"
login::login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_QuitOnClose);

    setAttribute(Qt::WA_TranslucentBackground);
    ui->centralwidget->setWindowTitle("AtQlas Alpha");

    set_opacity(ui->label_4, this, 0);
    set_opacity(ui->lineEdit_3,this, 0);

    generic_appear(ui->lineEdit, this, 1000);
    generic_appear(ui->lineEdit_2, this, 1000);
    generic_move_in(ui->lineEdit,  200, 1000);
    generic_move_in(ui->lineEdit_2,  200, 1000);
    generic_move_in(ui->lineEdit_3,  200, 1000);

}

login::~login()
{
    delete ui;
}

auto smooth_out=[](auto __this){
    generic_move_out(__this->ui->lineEdit,  200, 1000);
    generic_move_out(__this->ui->lineEdit_2,  200, 1000);
    generic_move_out(__this->ui->lineEdit_3,  200, 1000);
    generic_fade(__this->ui->lineEdit, __this, 1000);
    generic_fade(__this->ui->lineEdit_2, __this, 1000);
    csleep(1000);
};

void login::on_pushButton_2_clicked()
{

    smoothed_generic_one_breath(ui->pushButton_2, this, 1, 0, 1000);


    using std::string;
    using std::cout;
    using std::endl;
    auto flag=false;
    if(not ui->lineEdit->text().isEmpty())
    {
        if (not ui->lineEdit_2->text().isEmpty()){
            QString name, passwd;
            name=ui->lineEdit->text();
            passwd=ui->lineEdit_2->text();
            string stdname, stdpasswd;
            stdname=name.toStdString();
            stdpasswd=passwd.toStdString();
            cout<<"name: "<<stdname<<"Password: "<<stdpasswd<<endl;

            if(mode==0){  //login an old user.
                auto user=defschool::adb_binding->manage_login(stdname, stdpasswd);
                switch(user->getpriority()){
                case defschool::UNKNOWN_PTY:{
                    ui->label_3->setText("Error password or name.");
                }break;
                case defschool::ADMIN_PTY:{

                    ui->label_3->setText("hello, admin!");
                    smooth_out(this);
                    auto adc=new admin_cockpit(0, FROZENP);
                    adc->show();
                    flag=true;
                } break;
                case defschool::TEC_PTY:{
                    ui->label_3->setText("Hello, teacher!");
                    smooth_out(this);
                    auto ttc=new tch_manage(0, defschool::adb_binding->bind_to_teacher(user->getname()));
                    ttc->show();
                    flag=true;
                }break;
                case defschool::STU_PTY:{
                    ui->label_3->setText("hello, student.");
                    smooth_out(this);
                    auto stc=new stu_manage(0, defschool::adb_binding->bind_to_student(user->getname()));
                    stc->show();
                    flag=true;

                }break;
                }
            }
            else if(mode==1){   //sign in a new user.
                bool flag=false;
                if(not ui->lineEdit_3->text().isEmpty()){
                    auto stdverify=ui->lineEdit_3->text().toStdString();
                    if(stdpasswd==stdverify){
                        //pass.
                        if(defschool::adb_binding->adb_lookup(stdname)){
                            ui->label_3->setText("User already exsists, try login.");
                        }
                        //the student do not exsist.
                        else{

                            //found student.
                            if(defschool::karacter::cb_binding->get_student_of_sid(stdname)!=nullptr){
                                defschool::adb_binding->reg_new_user(stdname, stdpasswd, defschool::STU_PTY);
                                flag=true;
                            }
                            //found teacher.
                            else if(defschool::karacter::cb_binding->get_teacher_of_tid(stdname)!=nullptr){
                                defschool::adb_binding->reg_new_user(stdname, stdpasswd, defschool::TEC_PTY);
                                flag=true;
                            }
                            //found nil.
                            else {
                                ui->label_3->setText("Contact your school manager.");
                            }

                        }

                        if(flag){
                            ui->label_3->setText("Sign in success.");
                        }
                    }
                    else{
                        ui->label_3->setText("Password error.");
                    }
                }
            }
        }
        else{
            ui->label_3->setText("Input password!");
        }

    }
    else{
        ui->label_3->setText("Input username!");
    }
    if(flag){
        //smoothed_generic_one_breath(ui->pushButton_2, this, 1,0,1000);n
        this->close();
    }
}

void login::on_pushButton_clicked()
{
    smoothed_generic_one_breath(ui->pushButton, this, 1, 0, 1000);
    this->close();
}

void login::on_pushButton_3_clicked()
{
    smoothed_generic_one_breath(ui->pushButton_3, this, 1, 0, 1000);
    if(current==0){
    ui->pushButton_3->setStyleSheet("border-image:url(:/atqres_ico/uarrow.png)");
    this->mode=1;
    generic_appear(ui->lineEdit_3, this, 1000);
    generic_appear(ui->label_4, this, 1000);
    current=1;
    }
    else if(current==1){
    ui->pushButton_3->setStyleSheet("border-image:url(:/atqres_ico/login_plus.png)");
    this->mode=0;
    generic_fade(ui->lineEdit_3,this, 1000);
    generic_fade(ui->label_4, this, 1000);
    current=0;
    }

}
