#include "stu_manage.h"
#include "ui_stu_manage.h"
#include "common_animation.h"
#include "login.h"
#include "TIME_MACHINE.h"
enum head1{
    lid,
    name,
    credit,
    grade,
    optp,
    teachers,
    score,
};


auto choose_table_setter=[](auto & target, auto & container){


    auto cell_setter=[&](auto row, auto col, auto str){
        target->setItem(row, col, new QTableWidgetItem(QString::fromStdString(str)));

        auto beautify=[&](){
            auto tc=new QColor(219, 230, 234);
            target->item(row, col)->setTextColor(*tc);
            auto fnt=new QFont("Courier");
            target->item(row, col)->setFont(*fnt);
        };beautify();
    };
    auto bool_cell_setter=[&](auto row, auto col, auto the_bool){
        const std::string y="Yes";
        const std::string n="no";
        if(the_bool)
            cell_setter(row, col, y);
        else
            cell_setter(row, col, n);
    };
    auto row=0;
    for(auto it : container){
        for (int col=lid;col<=teachers;col=col+1){
            switch(col){
            case lid:{
                cell_setter(row, col, it->getLID());
            }break;
            case name:{
                cell_setter(row, col, it->getname());
            }break;
            case credit:{
                auto cre=it->getcredit();
                std::stringstream ss;
                ss<<cre;
                std::string str;
                ss>>str;
                cell_setter(row, col, str);
            }break;
            case grade:{
                auto gra=it->getgrade();
                std::stringstream ss;
                ss<<gra;
                std::string str;
                ss>>str;
                cell_setter(row, col, str);
            }break;
            case optp:{
                auto optp=it->getopt_p();
                bool_cell_setter(row, col, optp);

            }break;
            case teachers:{
                std::stringstream ss;
                std::string teacher_str;
                for(auto tit : it->get_teacher_binding()){
                    ss<<tit->getname();
                    ss<<", ";
                }
                ss>>teacher_str;
                cell_setter(row, col, teacher_str);
            }break;
            }
        }
        row++;
    }
};

auto current_table_setter=[](auto stu, auto & target, auto container){

    auto __blue=new QColor(46,164,168);
    auto __pink=new QColor(253, 125, 128);

    auto cell_setter=[&](auto row, auto col, auto str){
        target->setItem(row, col, new QTableWidgetItem(QString::fromStdString(str)));
        auto beautify=[&](){

            auto tc=new QColor(219, 230, 234);
            target->item(row, col)->setTextColor(*tc);
            auto fnt=new QFont("Courier");
            target->item(row, col)->setFont(*fnt);
        };
        beautify();
    };


    auto bool_cell_setter=[&](auto row, auto col, auto the_bool){
        const std::string y="Yes";
        const std::string n="no";
        if(the_bool)
            cell_setter(row, col, y);
        else
            cell_setter(row, col, n);
    };


    auto row=0;
    for(auto it : container){
        auto cur_color=it->getopt_p()?__blue:__pink;
        for (int col=lid;col<=score;col=col+1){

            auto colorize=[&](){
                target->item(row, col)->setBackgroundColor(*cur_color);
            };

            switch(col){
            case lid:{
                cell_setter(row, col, it->getLID());
                colorize();
            }break;
            case name:{
                cell_setter(row, col, it->getname());
                colorize();
            }break;
            case credit:{
                auto cre=it->getcredit();
                std::stringstream ss;
                ss<<cre;
                std::string str;
                ss>>str;
                cell_setter(row, col, str);
                colorize();
            }break;
            case grade:{
                auto gra=it->getgrade();
                std::stringstream ss;
                ss<<gra;
                std::string str;
                ss>>str;
                cell_setter(row, col, str);
                colorize();
            }break;
            case teachers:{
                std::stringstream ss;
                std::string teacher_str;
                for(auto tit : it->get_teacher_binding()){
                    ss<<tit->getname();
                    ss<<", ";
                }
                ss>>teacher_str;
                cell_setter(row, col, teacher_str);
                colorize();
            }break;
            case optp:{
                bool_cell_setter(row, col, it->getopt_p());
                colorize();
            }break;
            case score:{
                auto the_lid=it->getLID();
                auto the_score=stu->get_score_of_lesson(the_lid);
                std::stringstream ss;
                ss<<the_score;
                std::string str;
                ss>>str;
                cell_setter(row, col, str);
                colorize();
            }break;
            }
        }
        row++;
    }
};
auto update_current_table=[](auto stu, auto & target){
    target->clearContents();
    std::vector<defschool::lesson_sptr> current;
    stu->get_required_lessons(current);
    stu->get_optional_lessons(current);
    current_table_setter(stu,target, current);
};

stu_manage::stu_manage(QWidget *parent, defschool::stu_sptr stu) :
    QMainWindow(parent),
    ui(new Ui::stu_manage)
{
    ui->setupUi(this);
    this->stu=stu;
     ui->centralwidget->setWindowTitle("AtQlas Alpha");

    auto f=[&](auto slot){
        slot->setParent(this);
    };

    f(ui->drawer);
    f(ui->gpa_label);
    f(ui->grade_label);
    f(ui->name_label);
    f(ui->school_label);
    f(ui->klass_label);
    f(ui->sid_label);
    f(ui->gpa_label_2);
    f(ui->grade_label_2);
    f(ui->name_label_2);
    f(ui->school_label_2);
    f(ui->klass_label_2);
    f(ui->sid_label_2);


    ui->name_label->setText(QString::fromStdString(stu->getname()));
    ui->sid_label->setText(QString::fromStdString(stu->getSID()));
    ui->school_label->setText(QString::fromStdString(stu->getschool()));
    ui->grade_label->setText(QString::fromStdString(stu->getgrade_s()));
    ui->klass_label->setText(QString::fromStdString(stu->getklass()));
    ui->gpa_label->setText(QString::fromStdString(stu->get_GPA_s()));
    QStringList hd1;
    hd1<<"LID"<<"Name"<<"Credit"<<"Grade"<<"optional?"<<"teachers"<<"Score";
    QStringList hd2;
    hd2<<"LID"<<"Name"<<"Credit"<<"Grade"<<"optional?"<<"teachers";
    //
    ui->current_lesson_table->setColumnCount(hd1.count());
    ui->current_lesson_table->setRowCount(table_max_count);
    ui->current_lesson_table->setMouseTracking(true);

    ui->choose_lesson_table->setColumnCount(hd2.count());
    ui->choose_lesson_table->setRowCount(table_max_count);
    ui->current_lesson_table->setMouseTracking(true);
    //

    //ui->lineEdit->hide();
    set_opacity(ui->lineEdit, this, 0);


    auto lb_bind=defschool::karacter::lb_binding;

    std::vector<defschool::lesson_sptr> applicable;
    lb_bind->get_applicable_lessons(stu, applicable);
    choose_table_setter(ui->choose_lesson_table, applicable);

    std::vector<defschool::lesson_sptr> current;
    stu->get_required_lessons(current);
    stu->get_optional_lessons(current);
    current_table_setter(stu, ui->current_lesson_table, current);


    ui->current_lesson_table->setHorizontalHeaderLabels(hd1);
    QHeaderView* headerView1 = ui->current_lesson_table->horizontalHeader();

    connect(headerView1, SIGNAL(sectionClicked(int)), this,SLOT(sortf(int)));
    //headerView1->setHidden(true);
    ui->choose_lesson_table->setHorizontalHeaderLabels(hd2);
    QHeaderView* headerView2 = ui->choose_lesson_table->horizontalHeader();
    //headerView2->setHidden(true);

}

stu_manage::~stu_manage()
{
    delete ui;
}

void stu_manage::on_lineEdit_textChanged(const QString &arg1)
{
    //smoothed_generic_one_breath(ui->drawer,this, 1, 0.2, 200);
    ui->choose_lesson_table->clearContents();
    std::vector<defschool::lesson_sptr> vl;

    xparser::parse_and_assoc_applicable_lesson(arg1.toStdString(), this->stu, vl);
    choose_table_setter(ui->choose_lesson_table, vl);
}

void stu_manage::on_pushButton_clicked()
{
    smoothed_generic_one_breath(ui->pushButton, this, 1, 0.2, 200);
    std::vector<std::string> sv;
    constexpr auto col=lid;
    for(auto row=0;;row++){
        auto it=ui->choose_lesson_table->item(row, col);
        if(it){
            sv.push_back(it->text().toStdString());
        }
        else{
            break;
        }
    }
//    this->stu->choose_lesson(sv); //add that vector to the stu.
#define bridge X::X.compile_do_block
    for(auto s : sv){
        bridge("as-for student "+stu->getSID()+" do add-lesson that {l:"+s+"}", true);
    }
    update_current_table(this->stu, ui->current_lesson_table);
}

void stu_manage::on_pushButton_2_clicked()
{
    smoothed_generic_one_breath(ui->pushButton, this, 1, 0.2, 200);
    constexpr auto col=lid;
    std::vector<std::string> sv;
    for(auto row=0;;row++){
        auto it=ui->choose_lesson_table->item(row, col);
        if(it){
            auto flag=false;
            for(int cc=lid;cc<teachers;cc++){
                if(ui->choose_lesson_table->item(row, cc)->isSelected()){
                    flag=true;
                    break;
                }
            }
            if(flag){
                sv.push_back(it->text().toStdString());
            }
            else;
        }
        else break;
    }

    //this->stu->choose_lesson(sv);
    for(auto s : sv){
        bridge("as-for student "+stu->getSID()+" do add-lesson that {l:"+s+"}", true);
    }
    update_current_table(this->stu, ui->current_lesson_table);
}

void stu_manage::on_delete_course_button_clicked()
{
    generic_one_breath(ui->delete_course_button, this, 1, 0.5, 1000);


    constexpr auto LID_POS=0;
    for(int row=0;;row++){
        if(ui->current_lesson_table->item(row, 0));
        else break;
        for(int col=lid;col<score;col++){
           if(ui->current_lesson_table->item(row, col)->isSelected()){

                auto lid=ui->current_lesson_table->item(row, LID_POS)->text().toStdString();
                //stu->delete_opt_lesson(lid);
                bridge("as-for student "+stu->getSID()+" do delete-lesson that {l:"+lid+"}", true);

           }
        }
    }
    update_current_table(stu, ui->current_lesson_table);
}

void stu_manage::on_drawer_currentChanged(int index)
{
    if(index==0){
        //ui->lineEdit->hide();
        set_opacity(ui->lineEdit, this, 0);
    }
    else if(index==1){
        generic_appear(ui->lineEdit, this, 1000);
        //ui->lineEdit->show();
    }

    //smoothed_generic_one_breath(ui->drawer, this, 1,0,1000);
}

void stu_manage::on_back_clicked()
{
    smoothed_generic_one_breath(ui->back, this, 0.8, 0, 1000);
    auto l=new login();
    auto ani= fade_close(this, 2500);
    connect(ani, SIGNAL(finished()), l, SLOT(show()));
    csleep(1000);

}

void stu_manage::on_delete_course_button_2_clicked()
{
    smoothed_generic_one_breath(ui->delete_course_button_2, this, 0.8, 0, 1000);

    update_current_table(stu, ui->current_lesson_table);
}
