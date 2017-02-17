#include "admin_lesson_db.h"
#include "ui_admin_lesson_db.h"
#include "admin_lesson_db_new_lesson.h"
#include <QCursor>
#include <QScrollBar>
#include <QColor>
#include <QFont>
#include "common_animation.h"
#include "admin_cockpit.h"
#include "TIME_MACHINE.h"
enum head{
    lid,
    name,
    credit,
    grade,
    optp,
    students,
    teachers,
};

admin_lesson_db::admin_lesson_db(QWidget *parent, bool frozen) :
    QMainWindow(parent),
    ui(new Ui::admin_lesson_db)
{
    ui->setupUi(this);


    if(not frozen){
        ui->frame->setStyleSheet("border-image:url(:/atqres_ico/blue_land.png)");

    }
    else
    {
        ui->frame->setStyleSheet("border-image:url(:/atqres_ico/wblue_land.png)");
    }


    ui->centralwidget->setWindowTitle("AtQlas Alpha");
    ui->search_edit->setPlaceholderText("Type here to filter...");
    auto global=ui->frame->geometry();
    constexpr auto spacing=50;

    QStringList hd;
    hd<<"LID"<<"Name"<<"Credit"<<"Grade"<<"optional?"<<"students"<<"teachers";

    ui->result_table->setColumnCount(hd.count());
    ui->result_table->setRowCount(table_max_count);
    ui->result_table->setMouseTracking(true);


    ui->result_table->setGeometry(QRect(spacing, 150, global.width()-2*spacing, table_height));
    //set the header.
    ui->result_table->setHorizontalHeaderLabels(hd);
    QHeaderView* headerView = ui->result_table->verticalHeader();
    headerView->setHidden(true);
    ui->result_table->setShowGrid(false);

    set_opacity(ui->result_table, this, 0.7);
    set_opacity(ui->line, this, 0.6);
    set_opacity(ui->name_label, this, 0.8);
    set_opacity(ui->search_edit, this, 0.7);
    ui->name_label->hide();
}

auto jump_out_alert_window=[](std::string str){
    QMessageBox::critical(0 ,
                          "critical message" , QString::fromStdString(str),
                          QMessageBox::Ok | QMessageBox::Default ,
                          QMessageBox::Cancel | QMessageBox::Escape , 	0 );

};

admin_lesson_db::~admin_lesson_db()
{
    delete ui;
}

auto list_all_lessons=[](auto ui, auto & container, auto self){
    self->table_is_ready=false;
    const std::string alas="<...>";
    auto set_cell=[&](auto row, auto col, auto str){
        ui->result_table->setItem(row, col, new QTableWidgetItem(QString::fromStdString(str)));

    };

    auto set_int_cell=[&](auto row, auto col, auto the_int){
        std::stringstream ss;
        std::string str;
        ss<<the_int;
        ss>>str;
        set_cell(row, col, str);
    };

    auto set_bool_cell=[&](auto row, auto col, auto the_bool){
        const std::string y="Yes";
        const std::string n="no";
        if(the_bool)
            set_cell(row, col, y);
        else
            set_cell(row, col, n);
    };


    auto __blue=new QColor(44,194,248);
    auto __pink=new QColor(253, 125, 128);

    auto cycle=[&](auto & the_container){
        auto row=0;
        auto current_color=__blue;
        for (auto the_lesson : the_container){
            current_color=the_lesson->getopt_p()?__blue:__pink;
            for(int col=lid;col<=teachers;col++){
                auto cell_beautify=[&](auto color){
                    ui->result_table->item(row, col)->setBackgroundColor(*color);
                    auto fnt=new QFont("PT Mono");
                    ui->result_table->item(row, col)->setTextAlignment(Qt::AlignCenter);
                    ui->result_table->item(row, col)->setFont(*fnt);
                };
                switch (col){
                case lid:
                    set_cell(row, col, the_lesson->getLID());
                    cell_beautify(current_color);
                    break;
                case name:
                    set_cell(row, col, the_lesson->getname());
                    cell_beautify(current_color);
                    break;
                case credit:
                    set_int_cell(row, col, the_lesson->getcredit());
                    cell_beautify(current_color);
                    break;
                case grade:
                    set_int_cell(row, col, the_lesson->getgrade());
                    cell_beautify(current_color);
                    break;
                case optp:
                    set_bool_cell(row, col, the_lesson->getopt_p());
                    cell_beautify(current_color);
                    break;
                case students:/*fall through*/

                case teachers:
                    set_cell(row, col, alas);
                    cell_beautify(current_color);
                    break;
                }
            }
            row++;
        }
        self->table_is_ready=true;
    };


    if (container.empty()){
        ;
    }
    else
        cycle(container);

};

void admin_lesson_db::on_search_edit_textChanged(const QString &arg1)
{

    ui->result_table->clearContents();
    std::cout<<"text changed: "<<arg1.toStdString()<<std::endl;
    std::vector<defschool::lesson_sptr> the_match;
    xparser::parse_and_assoc_base(arg1.toStdString(), the_match);
    if(not the_match.empty()){
        list_all_lessons(ui, the_match, this);
    }
}

void admin_lesson_db::on_result_table_cellEntered(int row, int column)
{

    auto lb_bind=defschool::karacter::lb_binding;
    constexpr auto LID_POS=0;

    if(not ui->result_table->item(row, LID_POS)){
        return;
    }

    auto the_id=ui->result_table->item(row, LID_POS)->text().toStdString();
    bool flag=false;
    ui->name_label->hide();

    auto f=[&](auto fn1, auto fn2){
        auto the_lesson=lb_bind->lesson_assoc(the_id);
        if(the_lesson!=nullptr){
            auto  stu_binding=fn1(the_lesson);
            std::string let;
            for(auto ptr : stu_binding){
                let.append(fn2(ptr)+":"+ptr->getname()+";\n");
            }
            ui->name_label->setText(QString::fromStdString(let));
            flag=true;
        }
    };

    switch(column){

    case students:
    {
        f([](auto lesson){return lesson->get_student_binding();},\
        [](auto stu_ptr){return stu_ptr->getSID();});
        break;
    }
    case teachers:{
        f([](auto lesson){return lesson->get_teacher_binding();},\
        [](auto tch_ptr){return tch_ptr->getTID();});
        break;
    }
    }

    if(flag){
        auto mouse=QCursor::pos();
        ui->name_label->setGeometry(QRect(mouse.x()-200, mouse.y()-100, 131, 121));
        ui->name_label->show();
    }


}

void admin_lesson_db::on_list_all_clicked()
{
    scaler_once(ui->list_all, 10, QEasingCurve::OutCurve, 100);
    generic_one_breath(ui->list_all, this, 1, 0.2, 500);
    smoothed_generic_one_breath(ui->result_table, this, 0.7, 0, 1000);
    auto lb_binding=defschool::karacter::lb_binding->get_lessons();
    std::vector<defschool::lesson_sptr> l_container;
    for(auto the_pair : lb_binding){
        l_container.push_back(the_pair.second);
    }
    list_all_lessons(ui, l_container, this);
}
auto modify_lesson_of_cell=[](auto ui, auto row, auto col){
    auto lb_bind=defschool::karacter::lb_binding;
    constexpr auto LID_COL=0;
    if(not ui->result_table->item(row, LID_COL)){
        jump_out_alert_window("Use add-lesson button to add a new record.");
        return;         //return when the current cell is empty.
    }
    auto the_lid=ui->result_table->item(row, LID_COL)->text().toStdString();
    auto the_content=ui->result_table->item(row, col)->text().toStdString();
    auto the_lesson=lb_bind->lesson_assoc(the_lid);
    /*
    auto boolean_parser=[](std::string s){
        if (s=="yes" or s=="Yes" or s=="y" or s=="Y") {
            return true;
        }
        else if (s=="no" or s=="No" or s=="n" or s=="N"){
            return false;
        }
        return true;
    };
*/
    auto generator=[=](auto slot, auto content){
      return "as-for lesson "+the_lesson->getLID()+" do modify-property "+slot+" "+content;
    };

    switch(col){
    case name:
        X::X.compile_do_block(generator("name", the_content),true);
        break;
    case credit:
        X::X.compile_do_block(generator("credit", the_content),true);
        break;
    case grade:
        X::X.compile_do_block(generator("grade", the_content), true);
        break;
    case optp:
        X::X.compile_do_block(generator("optp", the_content), true);
        break;
    default:
        break;
    }
/* old:
 * the_lesson->setname(the_content);break;
 * the_lesson->setcredit(std::stoi(the_content));break;
 * the_lesson->setgrade(std::stoi(the_content));break;
 * the_lesson->setopt_p(boolean_parser(the_content));break;
 * */

};

void admin_lesson_db::on_result_table_cellChanged(int row, int column)
{
    const auto LID_COL=0;
    if(column==LID_COL and this->table_is_ready==true){
        jump_out_alert_window("The lid is non-mutatable.");return;

    }
    if(this->table_is_ready==true)
        modify_lesson_of_cell(ui, row, column);
}


void admin_lesson_db::on_result_table_cellClicked(int row, int column)
{

    const auto LID_COL=0;
    if(column==LID_COL){
        jump_out_alert_window("The LID is non-mutatable.");return;
    }
    row=row*2;//to get away from alert;
}

void admin_lesson_db::on_new_button_clicked()
{
    generic_one_breath(ui->new_button, this, 1, 0.2, 1000);
    scaler_once(ui->new_button, 10, QEasingCurve::OutCurve, 100);
    auto atnl=new admin_tchmanage_new_lesson();
    atnl->show();
}

void admin_lesson_db::on_delete_button_clicked()
{
    generic_one_breath(ui->delete_button, this, 1, 0.2, 500);
    scaler_once(ui->delete_button, 10, QEasingCurve::OutCurve, 100);
    smoothed_generic_one_breath(ui->result_table, this, 0.7, 0, 1000);


    constexpr auto LID_POS=0;
    for(int row=0;;row++){
        if(ui->result_table->item(row, 0));
        else break;
        for(int col=lid;col<teachers;col++){
            if(ui->result_table->item(row, col)->isSelected()){

                auto lid=ui->result_table->item(row, LID_POS)->text().toStdString();
                X::X.compile_do_block("as-for lesson "+lid+" do delete", true);
            }
        }
    }
    //以下内容在重构的时候望更改。
    ui->result_table->clearContents();
    if(ui->search_edit->text().isEmpty()){
        auto lb_binding=defschool::karacter::lb_binding->get_lessons();
        std::vector<defschool::lesson_sptr> l_container;
        for(auto the_pair : lb_binding){
            l_container.push_back(the_pair.second);
        }
        list_all_lessons(ui, l_container, this);
    }
    else{
        auto arg1=ui->search_edit->text();
        ui->result_table->clearContents();
        std::vector<defschool::lesson_sptr> the_match;
        xparser::parse_and_assoc_base(arg1.toStdString(), the_match);
        if(not the_match.empty()){
            list_all_lessons(ui, the_match, this);
        }
    }
}

void admin_lesson_db::on_search_edit_returnPressed()
{
    smoothed_generic_one_breath(ui->result_table, this, 0.7, 0, 1000);
    ui->result_table->clearContents();
    auto arg1=ui->search_edit->text();
    std::cout<<"text changed: "<<arg1.toStdString()<<std::endl;
    std::vector<defschool::lesson_sptr> the_match;
    xparser::parse_and_assoc_base(arg1.toStdString(), the_match);
    if(not the_match.empty()){
        list_all_lessons(ui, the_match, this);
    }
}

void admin_lesson_db::on_pushButton_clicked()
{
    smoothed_generic_one_breath(ui->pushButton, this, 0.8, 0, 1000);
    auto l=new admin_cockpit(0, FROZENP);
    auto ani= fade_close(this, 2500);
    connect(ani, SIGNAL(finished()), l, SLOT(show()));
    csleep(1000);
}
