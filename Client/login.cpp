#include "login.h"
#include "ui_login.h"
#include "stumanage.h"
#include "tchmanage.h"
#include "def.h"
#include "block_encapsules.h"
#include <sstream>
#include <iostream>


using std::cout;using std::endl;
using std::endl;
login::login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);


    set_opacity(ui->label_3, this, 0);
    generic_appear(ui->nameedit, this, 1000);
    generic_appear(ui->passedit, this, 1000);
    generic_move_in(ui->nameedit,  200, 1000);
    generic_move_in(ui->passedit,  200, 1000);

}

login::~login()
{
    delete ui;
}


Func(smooth_out, Var __this){
    generic_move_out(__this->ui->nameedit,  200, 1000);
    generic_move_out(__this->ui->passedit,  200, 1000);
    generic_fade(__this->ui->nameedit, __this, 1000);
    generic_fade(__this->ui->passedit, __this, 1000);
    csleep(1000);
};

void login::on_ok_clicked()
{
    Var passwd=ui->passedit->text().toStdString();
    Var name=ui->nameedit->text().toStdString();
    std::stringstream ss;


    toSky(encapsule_in_login_block("name:"+name+",\n""passwd:"+passwd));

    Var repl=fromSky();

    if(repl=="STU_PTY"){
        this->ui->label_3->setText("Hi, dude!");
        generic_appear(ui->label_3, this,  1000);
        smooth_out(this);
        Var sm=new stumanage(0, name);
        sm->show();
        this->close();
    }
    elif(repl=="TCH_PTY"){
        this->ui->label_3->setText("Hi, teacher!");
        generic_appear(ui->label_3, this,  1000);
        smooth_out(this);
        Var tm=new tchmanage(0, name);
        tm->show();
        this->close();
    }
    else
    {
        this->ui->label_3->setText("Error passwd or username.");
        smoothed_generic_one_breath(ui->label_3, this,0, 1, 2000);
    }

}

void login::on_back_clicked()
{
    smoothed_generic_one_breath(ui->back, this, 1, 0, 500);
    exit(0);
}
