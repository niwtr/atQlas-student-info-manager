#include "admin_lesson_db_new_lesson.h"
#include "ui_admin_tchmanage_new_lesson.h"
#include "TIME_MACHINE.h"
admin_tchmanage_new_lesson::admin_tchmanage_new_lesson(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::admin_tchmanage_new_lesson)
{
    ui->setupUi(this);
     //ui->centralwidget->setWindowTitle("AtQlas Alpha");
}

admin_tchmanage_new_lesson::~admin_tchmanage_new_lesson()
{
    delete ui;
}


auto jump_out_alert_window=[](std::string str){
    QMessageBox::critical(0 ,
                          "critical message" , QString::fromStdString(str),
                          QMessageBox::Ok | QMessageBox::Default ,
                          QMessageBox::Cancel | QMessageBox::Escape , 	0 );

};
auto chkempty=[](auto & target, auto & label){
    auto c=target->text().toStdString();
    if(c.empty())
    {
        jump_out_alert_window(label->text().toStdString()+" to be set.");
        return false;
    }
    return true;
};

void admin_tchmanage_new_lesson::on_ok_clicked()
{
    auto f=[](auto & target){
        return target->text().toStdString();
    };

    if(chkempty(ui->lid_edit, ui->lid_label) and
            chkempty(ui->name_edit, ui->name_label) and
            chkempty(ui->credit_edit, ui->credit_label) and
            chkempty(ui->grade_edit, ui->grade_label) and
            chkempty(ui->optp_edit, ui->optp_label)){
        auto lid=f(ui->lid_edit);
        auto name=f(ui->name_edit);
        auto credit=(f(ui->credit_edit));
        auto grade=(f(ui->grade_edit));
        auto optps=f(ui->optp_edit);

    X::X.compile_definition_block("lid:"+lid+","
                                  "name:"+name+","
                                  "credit:"+credit+","
                                  "grade:"+grade+","
                                  "optp:"+optps, "lesson",
                                  true);

    this->close();
    }
}

void admin_tchmanage_new_lesson::on_cancel_clicked()
{
    this->close();
}
