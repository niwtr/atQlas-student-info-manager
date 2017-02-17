#include "admin_stmanager.h"
#include "ui_admin_stmanager.h"
#include "stu_manage.h"
#include "QPropertyAnimation"
#include "common_animation.h"
#include "admin_cockpit.h"
#include <QColor>
#include "TIME_MACHINE.h"
#include "sortapi.h"
using std::cout;
using std::endl;
constexpr auto  SID_POSITION=0;
enum head{
    sid=0,
    name=1,
    school=2,
    grade=3,
    klass=4
};

template<typename Fn>
void admin_stmanager::set_cell_and_fake(int row, int col, Fn method){
    auto & the_widget=ui->tableWidget;
    auto the_content=method();
    the_widget->setItem(row, col, new QTableWidgetItem(QString::fromStdString(the_content)));
    this->fake_widget_set(row, col, the_content);

    auto beautify=[&](){
        auto tc=new QColor(219, 230, 234);
        the_widget->item(row, col)->setTextColor(*tc);
        auto fnt=new QFont("Courier");
        the_widget->item(row, col)->setFont(*fnt);
    };

    beautify();

}

auto list_all_stu_of_container=[](auto self, auto & container){
    self->fake_widget_is_ready=false;
    self->ui->tableWidget->clearContents();
    auto row=0;

    /*set the cell of that table and also the same cell in the fake_widget.*/

    for(auto the_student : container){
        for(int col=sid;col<=klass;col=col+1){
            switch(col){
            case sid:{self->set_cell_and_fake(row, col, [&](){return the_student->getSID();});}break;
            case name:{self->set_cell_and_fake(row, col, [&](){return the_student->getname();});}break;
            case school:{self->set_cell_and_fake(row, col, [&](){return the_student->getschool();});}break;
            case grade:{self->set_cell_and_fake(row, col,
                                                [&]()
                {std::stringstream ss;
                    ss<<the_student->getgrade();
                    std::string str;
                    ss>>str;return str;});}break;
            case klass:{self->set_cell_and_fake(row, col, [&](){return the_student->getklass();});}break;
            }
        }
        row++;
    }
    self->fake_widget_is_ready=true;
};



admin_stmanager::admin_stmanager(QWidget *parent, bool frozen) :
    QMainWindow(parent),
    ui(new Ui::admin_stmanager)
{


    ui->setupUi(this);

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
    setAttribute(Qt::WA_AcceptTouchEvents);
    //setAttribute(Qt::WA_TouchPadAcceptSingleTouchEvents);

    grabGesture(Qt::SwipeGesture);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);

    qApp->installEventFilter(this);

    this->fake_widget_is_ready=false;


    QStringList headers;
    headers<<"SID"<<"Name"<<"School"<<"Grade"<<"Class";

    ui->tableWidget->setColumnCount(headers.count());
    ui->tableWidget->setRowCount(40);



    auto frame_geo=ui->frame->geometry();

    constexpr auto ratio=0.73;
    auto table_x=frame_geo.width()*(1-ratio)/2;
    auto table_width=frame_geo.width()-2*table_x;
    auto table_y=frame_geo.height()*(1-ratio)/2+30;
    auto table_height=frame_geo.height()-2*table_y;

    ui->tableWidget->setGeometry(QRect(table_x,table_y, table_width, table_height));
    set_opacity(ui->back, this, 0.7);

    /*get_binding*/
    auto & stbinding=defschool::karacter::cb_binding->get_students_binding();

    std::vector<defschool::stu_sptr> vc;
    for(auto & the_pair : stbinding){
        vc.push_back(the_pair.second);
    }
    list_all_stu_of_container(this, vc);

    //set the header.
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    //QHeaderView* headerView = ui->tableWidget->horizontalHeader();
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //initialization finished.
    this->fake_widget_is_ready=true;
}

admin_stmanager::~admin_stmanager()
{
    delete ui;
}
auto jump_out_alert_window=[](std::string str){
    QMessageBox::critical(0 ,
                          "critical message" , QString::fromStdString(str),
                          QMessageBox::Ok | QMessageBox::Default ,
                          QMessageBox::Cancel | QMessageBox::Escape , 	0 );

};

bool admin_stmanager::event(QEvent * event){
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}


bool admin_stmanager::gestureEvent(QGestureEvent *event)
{


    if (event->gesture(Qt::PinchGesture))
    {

        auto cr=ui->tableWidget->currentRow();
        if(not ui->tableWidget->item(cr, SID_POSITION)){
            jump_out_alert_window("no student selected.");
            return false;
        }

        auto the_sid=ui->tableWidget->item(cr, SID_POSITION)->text().toStdString();
        auto the_student=defschool::karacter::cb_binding->get_student_of_sid(the_sid);
        if(the_student!=nullptr){
            fade(this);
            auto  asd=new admin_stmanager_detail(FROZENP, 0,this, the_student);
            asd->show();

        }

    }

    return true;
}



auto make_fw_address=[](int row, int col){
    using std::string;
    using std::stringstream;
    stringstream ss;
    ss<<row<<";"<<col;
    string the_address;
    ss>>the_address;
    return the_address;
};

std::string admin_stmanager::fake_widget_get(int row, int col)
{
    auto it = this->fake_widget.find(make_fw_address(row, col));
    if (it!=this->fake_widget.end()){
        return it->second;
    }
    else{
        return fw_not_found_error_string;
    }
}

bool admin_stmanager::fake_widget_set(int row, int col, std::string content)
{

    this->fake_widget.insert({make_fw_address(row, col), content});
    return true;

}


void admin_stmanager::on_tableWidget_cellDoubleClicked(int row, int column)
{
    if(not ui->tableWidget->item(row, SID_POSITION)){
        //the user input something in an empty line.
        //cowsay: improve this guy..
        jump_out_alert_window("This is an empty box, if you want to create a record, go for \"creation\".");
        return;
    }

    //in a used line.

    if (column==sid){
        jump_out_alert_window("The SID of student is non-mutatable.");
    }

}

void admin_stmanager::on_tableWidget_cellChanged(int row, int column)
{
    if (not this->fake_widget_is_ready)
        return ;        //return when the fake widget is not ready.


    if(not ui->tableWidget->item(row, SID_POSITION))
        return;         //return when the current cell is empty.

    //in a used line.
    auto the_sid=ui->tableWidget->item(row, SID_POSITION)->text().toStdString();
    auto the_content=ui->tableWidget->item(row, column)->text().toStdString();
    //auto & cb_bind=defschool::karacter::cb_binding;

    /* handle the modification.
     * able to analyze the case wether the modification succeed.
     * */
    auto handle_modification=[&](auto sid, auto slot, auto content){

        X::X.compile_do_block("as-for student "+sid+" do modify-property "+slot+" "+content, true);

    };

    auto roll_back_current_cell=[&](auto row, auto column){
        ui->tableWidget->item(row, column)->\
                setText(QString::fromStdString(this->fake_widget_get(row, column)));
    };

    switch (column)
    {
    case sid:{roll_back_current_cell(row, column);}break;   //reassigning the sid is illegal.
    case name:{handle_modification(the_sid, "name", the_content);}break;
    case school:{handle_modification(the_sid, "school", the_content);}break;
    case grade:{handle_modification(the_sid, "grade", the_content);}break;
    case klass:{handle_modification(the_sid, "class", the_content);}break;
    }
}


void admin_stmanager::on_tableWidget_cellClicked(int row, int column)
{

    if(qApp->mouseButtons()==Qt::LeftButton)return;
    else if(qApp->mouseButtons()==Qt::RightButton){
        if(not ui->tableWidget->item(row, SID_POSITION))
            return;         //return when the current cell is empty.
        this->show_menu_on_cell();
    }
    column*=2;//

}

void admin_stmanager::show_menu_on_cell()
{
}


void admin_stmanager::on_filter_edit_textChanged(const QString &arg1)
{
    std::vector<defschool::stu_sptr> vc;
    xparser::parse_and_assoc_base(arg1.toStdString(), vc);
    list_all_stu_of_container(this, vc);
}

void admin_stmanager::on_back_clicked()
{
    smoothed_generic_one_breath(ui->back, this, 0.8, 0, 500);
    auto l=new admin_cockpit(0, FROZENP);
    auto ani= fade_close(this, 1000);
    connect(ani, SIGNAL(finished()), l, SLOT(show()));
    //csleep(1000);
}


auto group_set_the_opacity=[](auto __this, auto opacity ){
    set_opacity(__this->ui->namedit, __this, opacity);
    set_opacity(__this->ui->idedit, __this, opacity);
    set_opacity(__this->ui->schooledit, __this, opacity);
    set_opacity(__this->ui->classedit, __this, opacity);
    set_opacity(__this->ui->fillinchart, __this, opacity);
};


auto frame2_appear=[](auto __this, auto duration){


    constexpr  auto namedit_pos=QRect(460, 30, 250, 30);
    constexpr  auto idedit_pos=QRect(460, 90, 250, 30);
    constexpr  auto schooledit_pos=QRect(460, 150, 250, 30);
    constexpr  auto classedit_pos=QRect(460, 210, 250, 30);

    __this->ui->schooledit->setGeometry(schooledit_pos);
    __this->ui->classedit->setGeometry(classedit_pos);
    __this->ui->idedit->setGeometry(idedit_pos);
    __this->ui->namedit->setGeometry(namedit_pos);

    generic_fade(__this->ui->tableWidget, __this, duration);
    generic_fade(__this->ui->filter_edit, __this, duration);
    generic_fade(__this->ui->back, __this, duration);
    csleep(duration);
    __this->ui->tableWidget->hide();
    __this->ui->frame_2->show();
    group_set_the_opacity(__this, 0);
    generic_move_in(__this->ui->schooledit, 200, duration);
    generic_move_in(__this->ui->namedit, 200, duration);
    generic_move_in(__this->ui->idedit, 200, duration);
    generic_move_in(__this->ui->classedit, 200, duration);

    generic_appear(__this->ui->schooledit, __this, duration);
    generic_appear(__this->ui->idedit, __this, duration);
    generic_appear(__this->ui->namedit, __this, duration);
    generic_appear(__this->ui->classedit, __this, duration);
    generic_appear(__this->ui->fillinchart, __this, duration);
};

auto frame2_fade=[](auto __this, auto duration)
{

    generic_move_out(__this->ui->schooledit, 200, duration);
    generic_move_out(__this->ui->namedit, 200, duration);
    generic_move_out(__this->ui->idedit, 200, duration);
    generic_move_out(__this->ui->classedit, 200, duration);
    generic_fade(__this->ui->idedit, __this, duration);
    generic_fade(__this->ui->namedit, __this, duration);
    generic_fade(__this->ui->schooledit, __this, duration);
    generic_fade(__this->ui->classedit, __this, duration);
    generic_fade(__this->ui->fillinchart, __this, duration);
    csleep(duration);
    __this->ui->frame_2->hide();

    __this->ui->tableWidget->show();
    generic_appear(__this->ui->tableWidget, __this, duration);
    generic_appear(__this->ui->filter_edit, __this, duration);
    generic_appear(__this->ui->back, __this, duration);



};

void admin_stmanager::on_new_stu_clicked()
{
    smoothed_generic_one_breath(ui->new_stu, this, 1, 0, 500);
    frame2_appear(this, 1500);

}

void admin_stmanager::on_OK_clicked()
{

    smoothed_generic_one_breath(ui->OK, this, 1, 0, 500);
    auto i=ui->idedit->text();
    auto n=ui->namedit->text();
    auto s=ui->schooledit->text();
    auto c=ui->classedit->text();
    if((not i.isEmpty()) and (not n.isEmpty())
            and (not s.isEmpty()) and (not c.isEmpty())){

        X::X.compile_definition_block("sid:"+i.toStdString()+","+
                                      "name:"+n.toStdString()+","+
                                      "school:"+s.toStdString()+","+
                                      "class:"+c.toStdString(),
                                      "student", true);

        frame2_fade(this, 1000);
    }  //and exit!




}

void admin_stmanager::on_NO_clicked()
{
    smoothed_generic_one_breath(ui->NO, this, 1, 0, 500);
    frame2_fade(this, 1000);

}
