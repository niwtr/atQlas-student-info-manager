#include "admin_stmanager_detail.h"
#include "ui_admin_stmanager_detail.h"
#include "xparser.h"
#include "common_animation.h"
#include "TIME_MACHINE.h"
#define bridge X::X.compile_do_block
enum head{
    LID,
    name,
    credit,
    grade,
    teachers
};

auto table_setter=[](auto & target, auto container){


    auto cell_setter=[&](auto row, auto col, auto str){
        auto beautify=[&](){
            auto tc=new QColor(219, 230, 234);
            target->item(row, col)->setTextColor(*tc);
            auto fnt=new QFont("Courier");
            target->item(row, col)->setFont(*fnt);
        };

        target->setItem(row, col, new QTableWidgetItem(QString::fromStdString(str)));
        beautify();
    };

    auto row=0;
    for(auto it : container){
        for (int col=LID;col<=teachers;col=col+1){


            switch(col){
            case LID:{
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

auto table_refresher=[](auto __this, auto ui){

    std::vector<defschool::lesson_sptr> opl;
    std::vector<defschool::lesson_sptr> rel;
    __this->stu->get_required_lessons(rel);
    __this->stu->get_optional_lessons(opl);
    table_setter(ui->req_lesson, rel);
    table_setter(ui->opt_lesson, opl);
};
admin_stmanager_detail::admin_stmanager_detail(bool frozen, QWidget *parent, QWidget * the_parent,  defschool::stu_sptr stuptr, int mode) :
    QWidget(parent),
    ui(new Ui::admin_stmanager_detail)
{
    ui->setupUi(this);

    if(not frozen){
        ui->frame->setStyleSheet("border-image:url(:/atqres_ico/admin_stmanager.png)");
    }
    else{
        ui->frame->setStyleSheet("border-image:url(:/atqres_ico/wadmin_stmanager.png)");
    }


    this->the_parent=the_parent;
    appear(this);
    this->stu=stuptr;
    this->setWindowTitle("AtQlas Alpha");
    setAttribute(Qt::WA_AcceptTouchEvents);
    grabGesture(Qt::PinchGesture);
    qApp->installEventFilter(this);

    ui->lineEdit_4->setText(QString::fromStdString(stuptr->getSID()));
    ui->lineEdit_4->setReadOnly(true);
    ui->name_edit->setText(QString::fromStdString(stuptr->getname()));
    ui->school_edit->setText(QString::fromStdString(stuptr->getschool()));
    ui->klass_edit->setText(QString::fromStdString(stuptr->getklass()));
    ui->grade_edit->setText(QString::fromStdString(stuptr->getgrade_s()));
    ui->opt_lesson->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->req_lesson->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->opt_lesson->setParent(this);
    ui->req_lesson->setParent(this);
    ui->recommendation->hide();

    auto pal=ui->lesson_chooser_edit->palette();
    pal.setColor(QPalette::Text,QColor(219,230,234));
    ui->lesson_chooser_edit->setPalette(pal);

    constexpr auto normal_cell_width=100;
    constexpr auto large_cell_width=2*normal_cell_width;
    constexpr auto normal_cell_height=20;
    constexpr auto table_width=2*large_cell_width+3*normal_cell_width;
    constexpr auto table_height=normal_cell_height*5;
    constexpr auto table_max_count=10;


    QStringList hd1, hd2;
    hd1<<"Optional Lessons"<<"Name"<<"Credit"<<"Grade"<<"Teachers";

    hd2<<"Required Lessons"<<"Name"<<"Credit"<<"Grade"<<"Teachers";

    auto set_table_cell_size=[&](auto target){


        for(auto row=0;row<target->rowCount();row++){
            for(auto col=0;col<target->columnCount();col++){

                if(col==name or col==teachers)
                    [&]()
                {target->setColumnWidth(col, large_cell_width);
                    target->setRowHeight(row, normal_cell_height);}();
                else
                [&]()
                {target->setColumnWidth(col, normal_cell_width);
                    target->setRowHeight(row, normal_cell_height);}();
            }
        }
    };

    ui->opt_lesson->setColumnCount(hd1.count());
    ui->opt_lesson->setRowCount(table_max_count);
    set_table_cell_size(ui->opt_lesson);
    ui->opt_lesson->setGeometry(QRect(0, 200, table_width, table_height));


    ui->req_lesson->setColumnCount(hd2.count());
    ui->req_lesson->setRowCount(table_max_count);
    set_table_cell_size(ui->req_lesson);
    ui->req_lesson->setGeometry(QRect(0, 200+table_height+20, table_width, table_height));



    std::vector<lesson_sptr> opl;
    std::vector<lesson_sptr> rel;
    stuptr->get_required_lessons(rel);
    stuptr->get_optional_lessons(opl);
    table_setter(ui->req_lesson, rel);
    table_setter(ui->opt_lesson, opl);


    ui->opt_lesson->setHorizontalHeaderLabels(hd1);
    ui->req_lesson->setHorizontalHeaderLabels(hd2);
    QHeaderView* headerView = ui->opt_lesson->verticalHeader();
    QHeaderView* headerView2= ui->req_lesson->verticalHeader();
    headerView->setHidden(true);
    headerView2->setHidden(true);

    if(mode==INSTANT_MODE){
        ui->lesson_chooser_edit->hide();
        ui->add_lesson_button->hide();
        ui->delete_lesson_button->hide();
        //cowsay:更改所有的为read_only.
    }

}




admin_stmanager_detail::~admin_stmanager_detail()
{
    delete ui;
}

bool admin_stmanager_detail::event(QEvent * event){
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}


bool admin_stmanager_detail::admin_stmanager_detail::gestureEvent(QGestureEvent *event)
{
    if (event->gesture(Qt::PinchGesture))
    {
        appear(this->the_parent);
        fade_out(this);
    }
    return true;

}
auto jump_out_alert_window=[](std::string str){
    QMessageBox::critical(0 ,
                          "critical message" , QString::fromStdString(str),
                          QMessageBox::Ok | QMessageBox::Default ,
                          QMessageBox::Cancel | QMessageBox::Escape , 	0 );

};
auto handle_modifiation_on_property=[](auto ui, auto slot, auto editor)    {

    //auto & cb_bind=defschool::karacter::cb_binding;
    auto the_student_sid=ui->lineEdit_4->text().toStdString();
    auto the_content=editor->text().toStdString();

    bridge("as-for student "+the_student_sid+" do modify-property "+slot+" "+the_content, true);

};

auto get_the_student=[](auto ui){
    auto & cb_bind=defschool::karacter::cb_binding;
    auto the_sid=ui->lineEdit_4->text().toStdString();
    return cb_bind->get_student_of_sid(the_sid);
};
auto get_the_student_sid=[](auto ui){
    auto the_sid=ui->lineEdit_4->text().toStdString();
    return the_sid;
};

void admin_stmanager_detail::on_name_edit_returnPressed()
{
    handle_modifiation_on_property(ui,"name",ui->name_edit);
}

void admin_stmanager_detail::on_grade_edit_returnPressed()
{
    handle_modifiation_on_property(ui,"grade",ui->grade_edit);
}

void admin_stmanager_detail::on_school_edit_returnPressed()
{
    handle_modifiation_on_property(ui,"school",ui->school_edit);
}

void admin_stmanager_detail::on_klass_edit_returnPressed()
{
    handle_modifiation_on_property(ui,"class",ui->klass_edit);
}



void admin_stmanager_detail::on_add_lesson_button_clicked()
{

    auto the_lid=ui->lesson_chooser_edit->text().toStdString();


    auto the_student=get_the_student(ui);

    if(the_student->has_lesson(the_lid)){
        jump_out_alert_window("Already have that lesson.");
        return;
    }
    if(the_student->admin_add_lesson(the_lid))
    {
        std::vector<lesson_sptr> opl;
        std::vector<lesson_sptr> rel;
        get_the_student(ui)->get_required_lessons(rel);
        get_the_student(ui)->get_optional_lessons(opl);
        table_setter(ui->req_lesson, rel);
        table_setter(ui->opt_lesson, opl);
        //cowsay: this indeed works.
    }
    else{
        jump_out_alert_window("No such lesson found.");
    }



}

void admin_stmanager_detail::on_lesson_chooser_edit_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        ui->recommendation->hide();
        return;
    }
    auto the_str=arg1.toStdString();
    std::vector<defschool::lesson_sptr> lcontainer;
    //lb_bind->v_lesson_assoc(the_str, 0, lcontainer);//cowsay
    xparser::parse_and_assoc_base(the_str, lcontainer);
    if (not lcontainer.empty()){
        ui->recommendation->clear();
        int row=0;
        for (auto the_lesson : lcontainer){
            ui->recommendation->insertItem(0, QString::fromStdString(the_lesson->getLID()));
            row++;
        }

        std::string x="ADD THEM ALL!";
        ui->recommendation->insertItem(row, QString::fromStdString(x));
        row++;

        auto geo=ui->lesson_chooser_edit->geometry();

        geo.setY(ui->lesson_chooser_edit->y()+geo.height());
        geo.setHeight((row+1)*21);

        ui->recommendation->setGeometry(geo);
        ui->recommendation->show();
    }

    else{
        ui->recommendation->hide();
    }
}

void admin_stmanager_detail::on_recommendation_itemClicked(QListWidgetItem *item)
{
    if(item==nullptr)
        return;
    auto lid=item->text();
    if(lid.toStdString()=="ADD THEM ALL!"){
        for(auto i=0;i<(ui->recommendation->count()-1);i++){
            auto the_recommended_lid=ui->recommendation->item(i)->text().toStdString();
            std::cout<<the_recommended_lid<<std::endl;
            auto the_student=get_the_student(ui);
            if(not the_student->has_lesson(the_recommended_lid))

                bridge("as-for student "+\
                                      get_the_student_sid(ui)+\
                                      " do add-lesson! that {l:"+the_recommended_lid+"}", true);

        }
        table_refresher(this, ui);
        ui->recommendation->hide();
        return;
    }


    //auto lid=item->text();
    ui->lesson_chooser_edit->setText(lid);

    ui->recommendation->hide();

}

void admin_stmanager_detail::on_delete_lesson_button_clicked()
{
    constexpr auto LID_POS=0;
    for(int row=0;;row++){
        if(ui->opt_lesson->item(row, 0));
        else break;
        for(int col=LID;col<=teachers;col++){
           if(ui->opt_lesson->item(row, col)->isSelected()){
                auto lid=ui->opt_lesson->item(row, LID_POS)->text().toStdString();

                bridge("as-for student "+stu->getSID()+" do delete-lesson that {l:"\
                                      +lid+"}", true);

           }
        }
    }
    ui->opt_lesson->clearContents();
    std::vector<lesson_sptr> opl;
    stu->get_optional_lessons(opl);
    table_setter(ui->opt_lesson, opl);


}
