#include "tch_manage.h"
#include "ui_tch_manage.h"
#include "tch_manager_detail.h"
#include "common_animation.h"
#include <QTime>
#include "login.h"
#include "TIME_MACHINE.h"
#define bridge X::X.compile_do_block
enum head{
    lid,
    name,
    credit,
    grade,
    optp,
    students,
};

auto __morph_things=[](auto ui, auto f1, auto f2, auto pred){
    pred?f1(ui->add_all_button):f2(ui->add_all_button);
    pred?f1(ui->add_selected_button):f2(ui->add_selected_button);
    pred?f1(ui->filter_edit):f2(ui->filter_edit);
    (not pred)?f1(ui->delete_button):f2(ui->delete_button);
    (not pred)?f1(ui->pushButton):f2(ui->pushButton);
    (not pred)?f1(ui->refresh):f2(ui->refresh);
};
auto hide_things=[](auto ui, auto __this){
    __morph_things(ui,[&](auto thing){generic_appear(thing, __this, 500);},
    [&](auto thing){generic_fade(thing, __this, 500);},
    false);
};
auto show_things=[](auto ui, auto __this){
    __morph_things(ui,[&](auto thing){generic_appear(thing, __this, 500);},
    [&](auto thing){generic_fade(thing, __this, 500);},
    true);
};


auto lesson_table_setter=[](auto & target, auto container){
    target->clearContents();

    auto cell_setter=[&](auto row, auto col, auto str){
        target->setItem(row, col, new QTableWidgetItem(QString::fromStdString(str)));
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
        for (int col=lid;col<=students;col=col+1){
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
            case students:{
                std::stringstream ss;
                std::string stu_str;
                for(auto tit : it->get_student_binding()){

                    ss<<tit->getname();
                    ss<<", ";
                }
                ss>>stu_str;
                cell_setter(row, col, stu_str);
            }break;
            }
        }
        row++;
    }
};

tch_manage::tch_manage(QWidget *parent, defschool::tec_sptr tch) :
    QMainWindow(parent),
    ui(new Ui::tch_manage)
{
    ui->setupUi(this);

    //for qgesture.
    setAttribute(Qt::WA_AcceptTouchEvents);
    //grabGesture(Qt::SwipeGesture);
    //grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);

    qApp->installEventFilter(this);

    this->tch=tch;
    ui->centralwidget->setWindowTitle("AtQlas Alpha");
    ui->name_label->setText(QString::fromStdString(tch->getname()));
    ui->tid_label->setText(QString::fromStdString(tch->getTID()));
    ui->school_label->setText(QString::fromStdString(tch->getschool()));
    QStringList hd;
    hd<<"LID"<<"Name"<<"Credit"<<"Grade"<<"optional?"<<"Students";
    //
    ui->lesson_table->setColumnCount(hd.count());
    ui->lesson_table->setRowCount(table_max_count);
    ui->lesson_table->setMouseTracking(true);

    hide_things(ui, this);

    std::vector<defschool::lesson_sptr> my_lessons;
    tch->get_my_lessons(my_lessons);
    lesson_table_setter(ui->lesson_table, my_lessons);


    auto f=[&](auto slot){
        slot->setParent(this);
    };

    f(ui->name_label);
    f(ui->name_label_2);
    f(ui->school_label);
    f(ui->school_label_2);
    f(ui->tid_label);
    f(ui->tid_label_2);


    auto frame_geo=ui->frame->geometry();

    constexpr auto ratio=0.73;
    auto table_x=frame_geo.width()*(1-ratio)/2;
    auto table_width=frame_geo.width()-2*table_x;
    auto table_y=frame_geo.height()*(1-ratio)/2;
    auto table_height=frame_geo.height()-2*table_y-200;
    table_y+=150;
    ui->lesson_table->setGeometry(QRect(table_x,table_y, table_width, table_height));

    ui->lesson_table->setParent(this);
    ui->lesson_table->horizontalHeader()->setStretchLastSection(true);

    ui->lesson_table->setHorizontalHeaderLabels(hd);
    QHeaderView* headerView1 = ui->lesson_table->horizontalHeader();
    //headerView1->setHidden(true);
    connect(headerView1, SIGNAL(sectionClicked(int)), this, SLOT(sortf(int)));
}

bool tch_manage::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}
bool tch_manage::gestureEvent(QGestureEvent *event)
{
    constexpr auto LID_POSITION=0;
    if (event->gesture(Qt::PinchGesture))
    {

        auto cr=ui->lesson_table->currentRow();
        if(not ui->lesson_table->item(cr, LID_POSITION)){
            return false;
        }

        auto the_lid=ui->lesson_table->item(cr, LID_POSITION)->text().toStdString();
        auto the_lesson=defschool::karacter::lb_binding->lesson_assoc(the_lid);
        if(the_lesson!=nullptr){
            fade(this);
            auto  tmd=new tch_manager_detail(0,this, this->tch, the_lesson);
            tmd->show();
        }

    }
    return true;
}

tch_manage::~tch_manage()
{
    delete ui;
}



void tch_manage::on_add_new_button_clicked()
{
    generic_one_breath(ui->lesson_table,this, 1, 0, 1000);
    generic_one_breath(ui->add_new_button, this, 1,0,1000);

    static auto chked=false;
    if(not chked){

        show_things(ui, this);

            csleep(500);
            ui->add_new_button->setStyleSheet("background:transparent;");
            ui->add_new_button->setStyleSheet("border-image:url(:/atqres_ico/rtnlite.png)");
        std::vector<defschool::lesson_sptr> lv;
        auto lb_binding=defschool::karacter::lb_binding->get_lessons();
        std::vector<defschool::lesson_sptr> l_container;
        for(auto the_pair : lb_binding){
            l_container.push_back(the_pair.second);
        }

        lesson_table_setter(ui->lesson_table, l_container);
        chked=not chked;
    }
    else{

        hide_things(ui, this);
            csleep(500);
            ui->add_new_button->setStyleSheet("background:transparent;");
            ui->add_new_button->setStyleSheet("border-image:url(:/atqres_ico/pluslite.png)");
        std::vector<defschool::lesson_sptr> my_lessons;
        tch->get_my_lessons(my_lessons);
        lesson_table_setter(ui->lesson_table, my_lessons);
        chked=not chked;
    }

}

void tch_manage::on_filter_edit_textChanged(const QString &arg1)
{
    generic_one_breath(ui->lesson_table,this, 1, 0.2, 2000);
    ui->lesson_table->clearContents();
    std::vector<defschool::lesson_sptr> the_match;
    xparser::parse_and_assoc_base(arg1.toStdString(), the_match);
    if(not the_match.empty()){
        lesson_table_setter(ui->lesson_table, the_match);
    }
}

void tch_manage::on_add_all_button_clicked()
{
    generic_one_breath(ui->add_all_button, this, 1,0,1000);

    std::vector<std::string> sv;
    constexpr auto col=lid;
    for(auto row=0;;row++){
        auto it=ui->lesson_table->item(row, col);
        if(it){
            sv.push_back(it->text().toStdString());
        }
        else{
            break;
        }
    }
    //this->tch->add_to_my_lesson(sv);
    for(auto s : sv){
        bridge("as-for teacher "+ this->tch->getTID()+" do add-lesson that {l:"+s+"}", true);
    }
}

void tch_manage::on_add_selected_button_clicked()
{
    generic_one_breath(ui->add_selected_button, this, 1,0,1000);
    constexpr auto col=lid;
    std::vector<std::string> sv;
    for(auto row=0;;row++){
        auto it=ui->lesson_table->item(row, col);
        if(it){
            auto flag=false;
            for(int cc=lid;cc<students;cc++){
                if(ui->lesson_table->item(row, cc)->isSelected()){
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

    //this->tch->add_to_my_lesson(sv);
    for(auto s : sv){
        bridge("as-for teacher "+ this->tch->getTID()+" do add-lesson that {l:"+s+"}", true);
    }

}

void tch_manage::on_delete_button_clicked()
{
    generic_one_breath(ui->delete_button, this, 1, 0.5, 1000);
    constexpr auto LID_POS=0;
    for(int row=0;;row++){
        if(ui->lesson_table->item(row, 0));
        else break;
        for(int col=lid;col<students;col++){
            if(ui->lesson_table->item(row, col)->isSelected()){

                auto lid=ui->lesson_table->item(row, LID_POS)->text().toStdString();
                //tch->delete_my_lesson(lid);
                bridge("as-for teacher "+tch->getTID()+" do delete-lesson that {l:"+lid+"}", true);

            }
        }
    }
    std::vector<defschool::lesson_sptr> my_lessons;
    tch->get_my_lessons(my_lessons);
    lesson_table_setter(ui->lesson_table, my_lessons);
}

void tch_manage::on_back_clicked()
{
    smoothed_generic_one_breath(ui->back, this, 0.8, 0, 1000);
    auto l=new login();
    auto ani= fade_close(this, 2500);
    connect(ani, SIGNAL(finished()), l, SLOT(show()));
    csleep(1000);
}



void tch_manage::on_pushButton_clicked()
{
    generic_one_breath(ui->pushButton, this, 1,0,1000);

    constexpr auto LID_POSITION=0;

        auto cr=ui->lesson_table->currentRow();
        if(not ui->lesson_table->item(cr, LID_POSITION)){
            return;
        }

        auto the_lid=ui->lesson_table->item(cr, LID_POSITION)->text().toStdString();
        auto the_lesson=defschool::karacter::lb_binding->lesson_assoc(the_lid);
        if(the_lesson!=nullptr){
                        fade(this);
            auto  tmd=new tch_manager_detail(0,this, this->tch, the_lesson);

            tmd->show();
        }

}

void tch_manage::on_refresh_clicked()
{
    generic_one_breath(ui->refresh, this, 1,0,1000);
    std::vector<defschool::lesson_sptr> my_lessons;
    tch->get_my_lessons(my_lessons);
    lesson_table_setter(ui->lesson_table, my_lessons);
}
