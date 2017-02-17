#include "admin_tchmanager_detail.h"
#include "ui_admin_tchmanager_detail.h"
#include "common_animation.h"
#include "TIME_MACHINE.h"
#define bridge X::X.compile_do_block
enum head{
    LID,
    name,
    credit,
    grade,
    students
};
auto jump_out_alert_window=[](std::string str){
    QMessageBox::critical(0 ,
                          "critical message" , QString::fromStdString(str),
                          QMessageBox::Ok | QMessageBox::Default ,
                          QMessageBox::Cancel | QMessageBox::Escape , 	0 );

};
auto table_setter=[](auto & target, auto & container){


    auto cell_setter=[&](auto row, auto col, auto str){
        target->setItem(row, col, new QTableWidgetItem(QString::fromStdString(str)));
        auto beautify_this_cell=[&](){
            QColor color(219, 230, 234);
            target->item(row, col)->setTextColor(color);
            auto fnt=new QFont("Courier");
            target->item(row, col)->setFont(*fnt);
        };
        beautify_this_cell();
    };

    auto row=0;
    for(auto it : container){
        for (int col=LID;col<=students;col=col+1){


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
                auto gra=it->getcredit();
                std::stringstream ss;
                ss<<gra;
                std::string str;
                ss>>str;
                cell_setter(row, col, str);
            }break;
            case students:{
                std::stringstream ss;
                std::string student_str;
                for(auto tit : it->get_student_binding()){
                    ss<<tit->getname();
                    ss<<", ";
                }
                ss>>student_str;
                cell_setter(row, col, student_str);
            }break;

            }
        }
        row++;
    }
};
admin_tchmanager_detail::admin_tchmanager_detail(bool frozen, QWidget *parent, QWidget * the_parent, defschool::tec_sptr tecptr) :
    QMainWindow(parent),
    ui(new Ui::admin_tchmanager_detail)
{
    ui->setupUi(this);

    if(not frozen){
        ui->frame->setStyleSheet("border-image:url(:/atqres_ico/admin_stmanager.png)");
    }
    else{
        ui->frame->setStyleSheet("border-image:url(:/atqres_ico/wadmin_stmanager.png)");
    }



     ui->centralwidget->setWindowTitle("AtQlas Alpha");
    this->the_parent=the_parent;
    appear(this);
    this->tch=tecptr;
    setAttribute(Qt::WA_AcceptTouchEvents);
    grabGesture(Qt::PinchGesture);
    qApp->installEventFilter(this);
    ui->tid_displayer->setText(QString::fromStdString(tecptr->getTID()));
    ui->tid_displayer->setReadOnly(true);
    ui->name_edit->setText(QString::fromStdString(tecptr->getname()));
    ui->school_edit->setText(QString::fromStdString(tecptr->getschool()));

    ui->recommendation->hide();
    constexpr auto normal_cell_width=100;
    constexpr auto large_cell_width=2*normal_cell_width;
    constexpr auto normal_cell_height=20;
//    constexpr auto table_width=2*large_cell_width+3*normal_cell_width;
    constexpr auto table_max_count=10;
    constexpr auto table_height=normal_cell_height*table_max_count;

    QStringList hd;
    hd<<"Lessons"<<"Name"<<"Credit"<<"Grade"<<"Students";

    auto set_table_cell_size=[&](auto target){


        for(auto row=0;row<target->rowCount();row++){
            for(auto col=0;col<target->columnCount();col++){



                if(col==name or col==students)
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

    ui->lesson_table->setColumnCount(hd.count());
    ui->lesson_table->setRowCount(table_max_count);
    set_table_cell_size(ui->lesson_table);


    auto frame_geo=ui->frame->geometry();

    constexpr auto ratio=0.73;
    auto table_x=frame_geo.width()*(1-ratio)/2;
    auto table_width=frame_geo.width()-2*table_x;
    auto table_y=frame_geo.height()*(1-ratio)/2+100;
    ui->lesson_table->setGeometry(QRect(table_x,table_y, table_width, table_height));

    auto monoize_and_whitify=[&](auto & target){
        auto pal=target->palette();
        pal.setColor(QPalette::Text,QColor(219,230,234));
        target->setPalette(pal);
        auto fnt=new QFont("PT Mono");
        target->setFont(*fnt);
    };

    monoize_and_whitify(ui->tid_displayer);
    monoize_and_whitify(ui->name_edit);
    monoize_and_whitify(ui->school_edit);

    auto align_x=[&](auto slot){
        auto __geo=slot->geometry();
        __geo.setX(table_x);
        slot->setGeometry(__geo);
    };

    align_x(ui->tid_displayer);
    align_x(ui->school_edit);
    align_x(ui->name_edit);


    std::vector<defschool::lesson_sptr> lsc;
    tecptr->get_my_lessons(lsc);//cowsay

    table_setter(ui->lesson_table, lsc);


    ui->lesson_table->setHorizontalHeaderLabels(hd);
    QHeaderView* headerView = ui->lesson_table->horizontalHeader();

    //headerView->setHidden(true);
    connect(headerView, SIGNAL(sectionClicked(int)), this, SLOT(sortf()));


}

auto handle_modifiation_on_property=[](auto ui, auto slot, auto editor)    {

    //auto & cb_bind=defschool::karacter::cb_binding;
    auto the_teacher_tid=ui->tid_displayer->text().toStdString();
    auto the_content=editor->text().toStdString();
    bridge("as-for student "+the_teacher_tid+" do modify-property "+slot+" "+the_content, true);
};

auto get_the_teacher=[](auto ui){
    auto & cb_bind=defschool::karacter::cb_binding;
    auto the_tid=ui->tid_displayer->text().toStdString();
    return cb_bind->get_teacher_of_tid(the_tid);
};


auto get_the_teacher_tid=[](auto ui){
    return ui->tid_displayer->text().toStdString();
};

admin_tchmanager_detail::~admin_tchmanager_detail()
{
    delete ui;
}

bool admin_tchmanager_detail::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}

bool admin_tchmanager_detail::gestureEvent(QGestureEvent *event)
{
        if (event->gesture(Qt::PinchGesture))
        {
            appear(this->the_parent);
            fade_out(this);
        }
        return true;

}



void admin_tchmanager_detail::on_name_edit_returnPressed()
{
    handle_modifiation_on_property(ui,"name",ui->name_edit);
}

void admin_tchmanager_detail::on_school_edit_returnPressed()
{
    handle_modifiation_on_property(ui,"school",ui->school_edit);

}

void admin_tchmanager_detail::on_add_lesson_button_clicked()
{
    auto the_lid=ui->lesson_chooser_edit->text().toStdString();


    auto the_teacher=get_the_teacher(ui);

    if(the_teacher->has_lesson(the_lid)){
        jump_out_alert_window("Already have that lesson.");
        return;
    }
    if(the_teacher->add_to_my_lesson(the_lid))
    {
        std::vector<defschool::lesson_sptr> lc;
        get_the_teacher(ui)->get_my_lessons(lc);
        table_setter(ui->lesson_table, lc);
    }
    else{
        jump_out_alert_window("No such lesson found.");
    }


}

void admin_tchmanager_detail::on_lesson_chooser_edit_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        ui->recommendation->hide();
        return;
    }
    auto the_str=arg1.toStdString();
    std::vector<defschool::lesson_sptr> lcontainer;
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
        geo.setHeight(row<5?(row+1)*21:80);

        ui->recommendation->setGeometry(geo);
        ui->recommendation->show();
    }

    else{
        ui->recommendation->hide();
    }
}
auto refresh_table=[](auto ui){
    std::vector<defschool::lesson_sptr> lc;
    get_the_teacher(ui)->get_my_lessons(lc);
    table_setter(ui->lesson_table, lc);
};

void admin_tchmanager_detail::on_recommendation_itemClicked(QListWidgetItem *item)
{
    if(item==nullptr)
        return;

    auto lid=item->text();

    if(lid.toStdString()=="ADD THEM ALL!"){
        for(auto i=0;i<(ui->recommendation->count()-1);i++){
            auto the_recommended_lid=ui->recommendation->item(i)->text().toStdString();
            auto the_teacher=get_the_teacher(ui);
            if(not the_teacher->has_lesson(the_recommended_lid))
                //get_the_teacher(ui)->add_to_my_lesson(the_recommended_lid);

                bridge("as-for teacher "+\
                                      get_the_teacher_tid(ui)+\
                                      " do add-lesson that {l:"+the_recommended_lid+"}", true);

        }
        refresh_table(ui);
        ui->recommendation->hide();
        return;
    }

    ui->lesson_chooser_edit->setText(lid);

    ui->recommendation->hide();

}

void admin_tchmanager_detail::on_delete_button_clicked()
{
    constexpr auto LID_POS=0;
    for(int row=0;;row++){
        if(ui->lesson_table->item(row, 0));
        else break;
        for(int col=LID;col<students;col++){
           if(ui->lesson_table->item(row, col)->isSelected()){

                auto lid=ui->lesson_table->item(row, LID_POS)->text().toStdString();
               // tch->delete_my_lesson(lid);
                bridge("as-for teacher "+get_the_teacher_tid(ui)+" do delete-lesson that {l:"\
                                      +lid+"}", true);


           }
        }
    }

    std::vector<defschool::lesson_sptr> my_lessons;
    tch->get_my_lessons(my_lessons);
    ui->lesson_table->clearContents();
    table_setter(ui->lesson_table, my_lessons);
}
