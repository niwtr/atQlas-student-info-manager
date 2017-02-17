#include "tch_manager_detail.h"
#include "ui_tch_manager_detail.h"
#include "admin_stmanager_detail.h"
#include "tch_manage.h"
#include "common_animation.h"
#include "TIME_MACHINE.h"
#define bridge X::X.compile_do_block
enum head{
    sid,
    name,
    result
};
auto jump_out_alert_window=[](std::string str){
    QMessageBox::critical(0 ,
                          "critical message" , QString::fromStdString(str),
                          QMessageBox::Ok | QMessageBox::Default ,
                          QMessageBox::Cancel | QMessageBox::Escape , 	0 );

};
auto stu_table_setter=[](auto & target, auto container, auto lsptr){
    target->clearContents();

    auto cell_setter=[&](auto row, auto col, auto str){
        target->setItem(row, col, new QTableWidgetItem(QString::fromStdString(str)));
    };
    auto int_cell_setter=[&](auto row, auto col, auto the_int){
        std::stringstream ss;
        ss<<the_int;
        std::string str;
        ss>>str;
        cell_setter(row, col, str);
    };
    auto row=0;
    for(auto it : container){
        for (int col=sid;col<=result;col=col+1){
            switch(col){
            case sid:{
                cell_setter(row, col, it->getSID());
            }break;
            case name:{
                cell_setter(row, col, it->getname());
            }break;
            case result:
            {
                int_cell_setter(row, col, it->get_score_of_lesson(lsptr->getLID()));
            }break;
            }
        }
        row++;
    }
};
tch_manager_detail::tch_manager_detail(QWidget *parent,QWidget * the_parent, defschool::tec_sptr tch, defschool::lesson_sptr ls) :
    QMainWindow(parent),
    ui(new Ui::tch_manager_detail)
{
    ui->setupUi(this);
    this->the_parent=the_parent;
    appear(this);
    this->ls=ls;
    this->tch=tch;
    auto __itos=[](int i){
        std::stringstream ss;
        std::string str;
        ss<<i;
        ss>>str;
        return str;
    };

    //for qgesture.
    setAttribute(Qt::WA_AcceptTouchEvents);
    grabGesture(Qt::SwipeGesture);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);

    qApp->installEventFilter(this);

    ui->centralwidget->setWindowTitle("AtQlas Alpha");
    /*init*/
    ui->lid_label->setText(QString::fromStdString(ls->getLID()));
    ui->name_label->setText(QString::fromStdString(ls->getname()));
    ui->grade_label->setText(QString::fromStdString(__itos(ls->getgrade())));
    ui->credit_label->setText(QString::fromStdString(__itos(ls->getcredit())));
    if(ls->getopt_p())
        ui->optp_label->setText("Yes");
    else
        ui->optp_label->setText("No");

    QStringList hd;
    hd<<"SID"<<"Name"<<"Result";
    //
    ui->stu_table->setColumnCount(hd.count());
    ui->stu_table->setRowCount(table_max_count);
    ui->stu_table->setMouseTracking(true);


    auto stc=ls->get_student_binding();
    table_is_ready=false;
    stu_table_setter(ui->stu_table, stc, ls);
    table_is_ready=true;
    ui->stu_table->setHorizontalHeaderLabels(hd);
    QHeaderView* headerView1 = ui->stu_table->horizontalHeader();
    //headerView1->setHidden(true);
connect(headerView1, SIGNAL(sectionClicked(int)),this, SLOT(sortf(int)));
    auto f=[&](auto slot){
        slot->setParent(this);
    };
    f(ui->stu_table);
    f(ui->label);
    f(ui->label_2);
    f(ui->label_3);
    f(ui->label_4);
    f(ui->label_5);
    f(ui->grade_label);
    f(ui->name_label);
    f(ui->lid_label);
    f(ui->credit_label);
    f(ui->optp_label);
    ui->stu_table->horizontalHeader()->setStretchLastSection(true);
}

tch_manager_detail::~tch_manager_detail()
{
    delete ui;
}

bool tch_manager_detail::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}

bool tch_manager_detail::gestureEvent(QGestureEvent *event)
{
    if (event->gesture(Qt::PinchGesture))
    {
        appear(this->the_parent);
        fade_out(this);//on this should be closed.
    }
    return true;
}

void tch_manager_detail::on_stu_table_cellDoubleClicked(int row, int column)
{
    if (column==result)return;
    constexpr auto SID_COL=0;
    auto the_item=ui->stu_table->item(row, SID_COL);
    if(the_item){
        auto the_sid=the_item->text().toStdString();
        auto the_student=defschool::karacter::cb_binding->get_student_of_sid(the_sid);
        auto a_s_m=new admin_stmanager_detail(FROZENP,0,this, the_student, INSTANT_MODE);
        a_s_m->show();
        this->close();
    }
    else{
        ;
    }
}

void tch_manager_detail::on_stu_table_cellChanged(int row, int column)
{
if(table_is_ready)
    smoothed_generic_one_breath(ui->stu_table, this, 1,0,1000);

    if(column==result){
        constexpr auto SID_COL=0;
        auto the_item=ui->stu_table->item(row, SID_COL);
        if(the_item){
            std::string new_result;
            new_result=ui->stu_table->item(row, column)->text().toStdString();


            auto the_sid=the_item->text().toStdString();
            //auto the_student=defschool::karacter::cb_binding->get_student_of_sid(the_sid);
            try{
           // this->tch->set_stu_lesson_score(the_student, ls->getLID(), new_result );

            bridge("as-for student "+the_sid+" do set-score! "+ls->getLID()+" "+new_result, true);
            }
            catch(defschool::negative_score_error & e){
                jump_out_alert_window(std::string(e.what()));
            }
        }

    }
}

void tch_manager_detail::on_refresh_clicked()
{

    smoothed_generic_one_breath(ui->refresh, this, 1,0,1000);
table_is_ready=false;
    auto stc=ls->get_student_binding();
    stu_table_setter(ui->stu_table, stc, ls);
    table_is_ready=true;
}
