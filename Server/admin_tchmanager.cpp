#include "admin_tchmanager.h"
#include "ui_admin_tchmanager.h"
#include "admin_cockpit.h"
#include "TIME_MACHINE.h"
#define bridge X::X.compile_do_block
#define def X::X.compile_definition_block
/*````````````````````````````Unix KISS````````````````````````````````*/
enum head{
    tid,
    name,
    school,
    lessons,
};

auto list_all_in_the_container=[](auto self, auto & container){
    self->table_is_ready=false;
    self->ui->tableWidget->clearContents();
    auto row=0;

    /*set the cell of that table and also the same cell in the fake_widget.*/

    for(auto the_teacher : container){
        for(int col=tid;col<=lessons;col=col+1){


            switch(col){
            case tid:{self->cell_setter(row, col, [&](){return the_teacher->getTID();});}break;
            case name:{self->cell_setter(row, col, [&](){return the_teacher->getname();});}break;
            case school:{self->cell_setter(row, col, [&](){return the_teacher->getschool();});}break;
            case lessons:{self->cell_setter(row, col,
                                            [&]()
                {
                    std::string if_is_too_much="<...>";
                    auto the_lessons=the_teacher->gettlesson();
                    if(the_lessons.size()==1){
                        return the_lessons[0];
                    }
                    else{
                        return if_is_too_much;
                    } //there is too much of the lessons and thus cannot be directly displayed.
                });}break;
            }
        }
        row++;
    }
    self->table_is_ready=true;
};

admin_tchmanager::admin_tchmanager(QWidget *parent, bool frozen) :
    QMainWindow(parent),
    ui(new Ui::admin_tchmanager)
{
    ui->setupUi(this);




    setAttribute(Qt::WA_AcceptTouchEvents);

    grabGesture(Qt::SwipeGesture);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);


    qApp->installEventFilter(this);




    if(not frozen){
        ui->frame->setStyleSheet("border-image:url(:/atqres_ico/admin_stmanager.png)");
        ui->frame_2->setStyleSheet("border-image:url(:/atqres_ico/admin_stmanager.png)");
    }
    else{
        ui->frame->setStyleSheet("border-image:url(:/atqres_ico/wadmin_stmanager.png)");
        ui->frame_2->setStyleSheet("border-image:url(:/atqres_ico/wadmin_stmanager.png)");
    }

    ui->frame_2->hide();
     ui->centralwidget->setWindowTitle("AtQlas Alpha");

    QStringList headers;
    headers<<"TID"<<"Name"<<"School"<<"Lessons";


    set_opacity(ui->back, this, 0.7);
    ui->tableWidget->setColumnCount(headers.count());
    ui->tableWidget->setRowCount(40);

    auto frame_geo=ui->frame->geometry();

    constexpr auto ratio=0.73;
    auto table_x=frame_geo.width()*(1-ratio)/2;
    auto table_width=frame_geo.width()-2*table_x;
    auto table_y=frame_geo.height()*(1-ratio)/2+30;
    auto table_height=frame_geo.height()-2*table_y;

    ui->tableWidget->setGeometry(QRect(table_x,table_y, table_width, table_height));


    auto & tcbinding=defschool::karacter::cb_binding->get_teachers_binding();
    std::vector<defschool::tec_sptr> vc;
    for(auto & the_pair : tcbinding){
        vc.push_back(the_pair.second);
    }

    list_all_in_the_container(this, vc);
    //set the header.
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    QHeaderView* headerView = ui->tableWidget->verticalHeader();
    headerView->setHidden(true);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //initialization finished.
}
auto jump_out_alert_window=[](std::string str){
    QMessageBox::critical(0 ,
                          "critical message" , QString::fromStdString(str),
                          QMessageBox::Ok | QMessageBox::Default ,
                          QMessageBox::Cancel | QMessageBox::Escape , 	0 );

};
admin_tchmanager::~admin_tchmanager()
{
    delete ui;
}

bool admin_tchmanager::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}

bool admin_tchmanager::gestureEvent(QGestureEvent *event)
{
    constexpr auto TID_POSITION=0;
    if (event->gesture(Qt::PinchGesture))
    {

        auto cr=ui->tableWidget->currentRow();
        if(not ui->tableWidget->item(cr, TID_POSITION)){
            jump_out_alert_window("no teacher selected.");
            return false;
        }

        auto the_tid=ui->tableWidget->item(cr, TID_POSITION)->text().toStdString();
        auto the_teacher=defschool::karacter::cb_binding->get_teacher_of_tid(the_tid);
        if(the_teacher!=nullptr){
            auto  asd=new admin_tchmanager_detail(FROZENP, 0, this, the_teacher);
            asd->show();
        }

    }

    return true;
}


template<typename f>
void admin_tchmanager::cell_setter(int row, int col, f method)
{
    auto & the_widget=ui->tableWidget;
    auto the_content=method();
    the_widget->setItem(row, col, new QTableWidgetItem(QString::fromStdString(the_content)));


    auto beautify=[&](){
        auto tc=new QColor(219, 230, 234);
        the_widget->item(row, col)->setTextColor(*tc);
        auto fnt=new QFont("Courier");
        the_widget->item(row, col)->setFont(*fnt);
    };

    beautify();
}

void admin_tchmanager::on_tableWidget_cellChanged(int row, int column)
{
    constexpr auto TID_POSITION=0;
    if (not this->table_is_ready)
        return ;        //return when the fake widget is not ready.


    if(not ui->tableWidget->item(row, TID_POSITION))
        return;         //return when the current cell is empty.

    //in a used line.
    auto the_tid=ui->tableWidget->item(row, TID_POSITION)->text().toStdString();
    auto the_content=ui->tableWidget->item(row, column)->text().toStdString();

    /* handle the modification.
         * able to analyze the case wether the modification succeed.
         * */
    auto handle_modification=[&](auto sid, auto slot, auto content){
        /*
        if (cb_bind->modify_teacher_property(sid, slot, content)){
            std::cout<<"modify success."<<std::endl;
        }
        else{
            //alert that the modification failed.
        }*/
        bridge("as-for teacher "+sid+" do modify-property "+slot+" "+content, true);
    };


    switch (column)
    {
    case tid:{break;}//roll_back_current_cell(row, column);}break;   //reassigning the sid is illegal.
    case name:{handle_modification(the_tid, "name", the_content);}break;
    case school:{handle_modification(the_tid, "school", the_content);}break;
    default:break;
    }


}

void admin_tchmanager::on_filter_edit_textChanged(const QString &arg1)
{
    std::vector<defschool::tec_sptr> vc;
    xparser::parse_and_assoc_base(arg1.toStdString(), vc);
    list_all_in_the_container(this, vc);
}

void admin_tchmanager::on_back_clicked()
{
    smoothed_generic_one_breath(ui->back, this, 0.8, 0, 500);
    auto l=new admin_cockpit();
    auto ani= fade_close(this, 1000);
    connect(ani, SIGNAL(finished()), l, SLOT(show()));
    //csleep(1000);
}



auto group_set_the_opacity=[](auto __this, auto opacity ){
    set_opacity(__this->ui->namedit, __this, opacity);
    set_opacity(__this->ui->idedit, __this, opacity);
    set_opacity(__this->ui->schooledit, __this, opacity);
    set_opacity(__this->ui->fillinchart, __this, opacity);
};


auto frame2_appear=[](auto __this, auto duration){

    constexpr auto namedit_pos=QRect(460, 90, 250, 30);
    constexpr auto idedit_pos=QRect(460, 150, 250, 30);
    constexpr auto schooledit_pos=QRect(460, 210, 250, 30);


    __this->ui->schooledit->setGeometry(schooledit_pos);
    __this->ui->idedit->setGeometry(idedit_pos);
    __this->ui->namedit->setGeometry(namedit_pos);

    generic_fade(__this->ui->tableWidget, __this, duration);
    generic_fade(__this->ui->filter_edit, __this, duration);
    generic_fade(__this->ui->back, __this, duration);
    generic_fade(__this->ui->new_tch, __this, duration);
    csleep(duration);
    __this->ui->tableWidget->hide();
    __this->ui->frame_2->show();
    group_set_the_opacity(__this, 0);
    generic_move_in(__this->ui->schooledit, 200, duration);
    generic_move_in(__this->ui->namedit, 200, duration);
    generic_move_in(__this->ui->idedit, 200, duration);

    generic_appear(__this->ui->schooledit, __this, duration);
    generic_appear(__this->ui->idedit, __this, duration);
    generic_appear(__this->ui->namedit, __this, duration);
    generic_appear(__this->ui->fillinchart, __this, duration);
};

auto frame2_fade=[](auto __this, auto duration)
{

    generic_move_out(__this->ui->schooledit, 200, duration);
    generic_move_out(__this->ui->namedit, 200, duration);
    generic_move_out(__this->ui->idedit, 200, duration);
    generic_fade(__this->ui->idedit, __this, duration);
    generic_fade(__this->ui->namedit, __this, duration);
    generic_fade(__this->ui->schooledit, __this, duration);
    generic_fade(__this->ui->fillinchart, __this, duration);
    csleep(duration);
    __this->ui->frame_2->hide();

    __this->ui->tableWidget->show();
    generic_appear(__this->ui->tableWidget, __this, duration);
    generic_appear(__this->ui->filter_edit, __this, duration);
    generic_appear(__this->ui->back, __this, duration);
    generic_appear(__this->ui->new_tch, __this, duration);


};

void admin_tchmanager::on_new_tch_clicked()
{
    smoothed_generic_one_breath(ui->new_tch, this, 1, 0, 500);
    frame2_appear(this, 1000);

}

void admin_tchmanager::on_OK_2_clicked()
{

    smoothed_generic_one_breath(ui->OK_2, this, 1, 0, 500);
    auto i=ui->idedit->text();
    auto n=ui->namedit->text();
    auto s=ui->schooledit->text();
    if((not i.isEmpty()) and (not n.isEmpty())
            and (not s.isEmpty())){
    //defschool::factory::createt(n.toStdString(), i.toStdString(), s.toStdString());
    frame2_fade(this, 1000);
    }  //and exit!



    def("tid:"+i.toStdString()+","+
                                  "name:"+n.toStdString()+","+
                                  "school:"+s.toStdString(),
                                  "teacher", true);

}

void admin_tchmanager::on_NO_2_clicked()
{
    smoothed_generic_one_breath(ui->NO_2, this, 1, 0, 500);
    frame2_fade(this, 1000);

}



