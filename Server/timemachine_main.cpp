#include "timemachine_main.h"
#include "ui_timemachine_main.h"
#include <QDateTime>
#include <QTimer>
#include <string>
#include <sstream>
#include "common_animation.h"
#include "admin_cockpit.h"
#include <qmovie>
#include <vector>
#include "interval_movie.h"
using std::cout;
using std::endl;

static QDateTime dt=QDateTime::currentDateTime();
static bool showing_table=false;
static long pieceFrame_frag_seq;
timemachine_main::timemachine_main(QWidget *parent, bool frozen) :
    QMainWindow(parent),
    ui(new Ui::timemachine_main)
{

    ui->setupUi(this);
    this->frozen=frozen;

    setAttribute(Qt::WA_AcceptTouchEvents);
    setAttribute(Qt::WA_TouchPadAcceptSingleTouchEvents);

    grabGesture(Qt::SwipeGesture);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);

    qApp->installEventFilter(this);

    if(not frozen){
        ui->frame->setStyleSheet("border-image:url(:/atqres_ico/originalclock.png)");
        ui->frame_2->setStyleSheet("border-image:url(:/atqres_ico/originalclock.png)");
    }
    else{
        ui->frame->setStyleSheet("border-image:url(:/atqres_ico/afterclock.png)");
        ui->frame_2->setStyleSheet("border-image:url(:/atqres_ico/afterclock.png)");
    }


    if(not frozen)
        ui->melt->setStyleSheet("border-image:url(:/atqres_ico/evolve.png)");
    else
        ui->melt->setStyleSheet("border-image:url(:/atqres_ico/melt.png)");

    QTimer * time_clock=new QTimer(this);
    connect(time_clock, SIGNAL(timeout()), this, SLOT(show_time_in_label()));
    time_clock->start(1000);


    QStringList headers;
    headers<<"Sequence"<<"Fragment"<<"Pieces";


    QStringList pheaders;
    pheaders<<"Sequence"<<"Category"<<"Original Block";

    ui->fragmentWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->pieceWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->fragmentWidget->setColumnCount(headers.count());
    ui->fragmentWidget->setRowCount(40);


    ui->pieceWidget->setColumnCount(pheaders.count());
    ui->pieceWidget->setRowCount(40);

    ui->fragmentWidget->setHorizontalHeaderLabels(headers);
    QHeaderView* headerView = ui->fragmentWidget->verticalHeader();
    headerView->setHidden(true);
    ui->fragmentWidget->horizontalHeader()->setStretchLastSection(true);


    ui->pieceWidget->setHorizontalHeaderLabels(pheaders);
    QHeaderView* pheaderView = ui->pieceWidget->verticalHeader();
    pheaderView->setHidden(true);
    ui->pieceWidget->horizontalHeader()->setStretchLastSection(true);


    set_opacity(ui->cancel, this, 0);
    set_opacity(ui->travel, this, 0);
    set_opacity(ui->hourd, this, 0.4);
    set_opacity(ui->mind, this, 0.4);
    set_opacity(ui->secd, this, 0.4);

    set_opacity(ui->frame_3, this, 0);
    ui->frame_3->hide();


    ui->frame_2->hide();
    ui->delpiece->hide();
    ui->addpiece->hide();
    ui->del->hide();



}

timemachine_main::~timemachine_main()
{
    delete ui;
}



void timemachine_main::freeze_time_label()
{

    static QDateTime tm;
    QTime u;
    if(not time_label_freezing){
        this->time_label_freezing=true;
        tm=QDateTime::currentDateTime();
        dt=tm;
    }
    else{
        ;
    }
}

void timemachine_main::show_time_in_label()
{
    if(not frozen){
        if(not this->time_label_freezing){
            QDateTime tm=QDateTime::currentDateTime();
            QString str =tm.toString("hh:mm:ss");//yyyy-MM-dd
            ui->curtime->setText(str);
        }
    }
    else{
        if(not this->time_label_freezing){
            auto str=QDateTime::fromTime_t(X::X.get_global_time()).toString("hh:mm:ss");//yyyy-MM-dd
            ui->curtime->setText(str);
        }
    }
}

auto show_button=[](auto __this, auto duration){
    if(not __this->already_showing and not showing_table){
        generic_appear(__this->ui->cancel, __this, duration);
        generic_appear(__this->ui->travel, __this, duration);
        generic_appear(__this->ui->melt, __this, duration);
        __this->already_showing=true;
    }
};

auto cell_setter=[](auto __this, auto & capsule){


    auto row=0;auto col=0;
    auto & the_widget=__this->ui->fragmentWidget;
    //set the cells.
    auto beautify=[&](){
        auto tc=new QColor(219, 230, 234);
        the_widget->item(row, col)->setTextColor(*tc);
        auto fnt=new QFont("Courier");
        the_widget->item(row, col)->setFont(*fnt);
    };
    for (auto & frag : capsule){
        for(col=0;col<3;col++){
            if(col==0){
                auto t=frag.first;
                std::stringstream ss;
                ss<<t;
                std::string str;
                ss>>str;
                the_widget->setItem(row, col, new QTableWidgetItem(QString::fromStdString(str)));
                beautify();
            }
            else if(col==1){
                auto qdt=QDateTime::fromTime_t(frag.first).toString();
                the_widget->setItem(row, col, new QTableWidgetItem(qdt));
                beautify();
            }
            else if(col==2){
                auto size=frag.second.size();
                std::stringstream ss;
                ss<<size;
                std::string str;
                ss>>str;
                the_widget->setItem(row, col, new QTableWidgetItem(QString::fromStdString(str)));
                beautify();
            }
        }
        row++;
    }
};




void timemachine_main::on_hourd_valueChanged(int value)
{
    show_button(this, 500);


    this->freeze_time_label();
    auto _time=dt.time();

    _time.setHMS(value, _time.minute(),_time.second());

    dt.setTime(_time);

    QString str =dt.toString("hh:mm:ss");//yyyy-MM-dd
    ui->curtime->setText(str);

}

void timemachine_main::on_mind_valueChanged(int value)
{

    show_button(this, 500);

    this->freeze_time_label();
    auto _time=dt.time();
    _time.setHMS(_time.hour(), value ,_time.second());
    dt.setTime(_time);

    QString str =dt.toString("hh:mm:ss");//yyyy-MM-dd
    ui->curtime->setText(str);
}

void timemachine_main::on_secd_valueChanged(int value)
{
    show_button(this, 500);


    this->freeze_time_label();
    auto _time=dt.time();
    _time.setHMS(_time.hour(), _time.minute() , value);
    dt.setTime(_time);

    QString str =dt.toString("hh:mm:ss");//yyyy-MM-dd
    ui->curtime->setText(str);
}


auto disappear_button=[](auto __this, auto duration){

    if( __this->already_showing){
        __this->already_showing=false;
        generic_fade(__this->ui->cancel, __this, duration);
        generic_fade(__this->ui->travel, __this, duration);
        generic_fade(__this->ui->melt, __this, duration);
    }

};

void timemachine_main::on_cancel_clicked()
{
    melt_time_label();
    disappear_button(this, 500);

}




void timemachine_main::on_travel_clicked()
{
    ui->travel->setParent(this);
    smoothed_generic_one_breath(ui->travel, this, 1, 0, 1000);
    ui->travel->hide();
    auto the_time=dt.toTime_t();

    X::X.travel_to_time(the_time);

    auto l=new admin_cockpit(0, FROZENP);

    //auto any=travel_jump(this, 1000);
    //connect(any, SIGNAL(finished()), any, SLOT(start()));

    fade_close(this, 2000);
    //connect(ani, SIGNAL(finished()), l, SLOT(show()));
    //csleep(2000);
    auto mov=new interval_movie(0,1);
    mov->show();
    connect(mov, SIGNAL(closing()), l, SLOT(show()));
}

void timemachine_main::on_del_clicked()
{
    generic_one_breath(ui->del, this, 1, 0, 500);
    constexpr auto seq_pos=0;
    std::vector<long> vfrag={};
    for(int row=0;;row++){
        if(ui->fragmentWidget->item(row, 0));
        else break;


        for(int col=0;col<3;col++){
            if(ui->fragmentWidget->item(row, col)->isSelected()){
                auto seq=std::stoi(ui->fragmentWidget->item(row, seq_pos)->text().toStdString());
                cout<<"del: "<<seq<<endl;
                vfrag.push_back(seq);break;

            }
        }
    }

    for(auto seqn : vfrag){
        X::X.delete_time_fragment(seqn);
    }



    ui->fragmentWidget->clearContents();
    cell_setter(this, X::X.get_capsule());

}

void timemachine_main::on_show_frag_clicked()
{
    generic_one_breath(ui->show_frag, this, 1, 0, 500);
    csleep(250);
    if(not showing_table){

        ui->show_frag->setStyleSheet("border-image:url(:/atqres_ico/close.png)");
        disappear_button(this, 500);
        ui->del->show();
        set_opacity(ui->del, this, 0);
        generic_appear(ui->del, this, 500);
        ui->frame_3->show();
        set_opacity(ui->frame_3, this, 0);
        generic_appear(ui->frame_3, this, 1000);

        cell_setter(this, X::X.get_capsule());
        showing_table=true;
    }
    else{
        ui->show_frag->setStyleSheet("border-image:url(:/atqres_ico/capsule.png)");

        generic_fade(ui->del, this, 500);
        //generic_fade(ui->fragmentWidget, this, 1000);
        generic_fade(ui->frame_3,this, 1000);
        csleep(1000);
        //ui->fragmentWidget->hide();
        ui->frame_3->hide();
        ui->del->hide();
        showing_table=false;
    }
}
auto piece_cell_setter=[](auto __this, auto & frag){
    auto row=0;auto col=0;
    auto & the_widget=__this->ui->pieceWidget;
    //set the cells.
    auto beautify=[&](){
        auto tc=new QColor(219, 230, 234);
        the_widget->item(row, col)->setTextColor(*tc);
        auto fnt=new QFont("Courier");
        the_widget->item(row, col)->setFont(*fnt);
    };
    for (auto & piece : frag.second){
        for(col=0;col<3;col++){
            if(col==0){
                std::stringstream ss;
                ss<<piece.get_seq();
                std::string s;
                ss>>s;
                the_widget->setItem(row, col, new QTableWidgetItem(QString::fromStdString(s)));
                beautify();
            }
            else if(col==1){
                the_widget->setItem(row, col, new QTableWidgetItem(QString::fromStdString(piece.get_category())));
                beautify();
            }
            else {
                the_widget->setItem(row, col, new QTableWidgetItem(QString::fromStdString(piece.get_block())));
                beautify();
            }
        }
        row++;
    }


};


void timemachine_main::on_fragmentWidget_cellDoubleClicked(int row, int column)
{
    ui->frame_3->hide();
    ui->frame_2->show();
    set_opacity(ui->frame_2, this, 0);
    generic_appear(ui->frame_2, this, 500);
    ui->pieceWidget->clearContents();

    if(ui->fragmentWidget->item(row, column)){
        constexpr auto seq_pos=0;
        auto fragseq=std::stoi(ui->fragmentWidget->item(row, seq_pos)->text().toStdString());

        auto & frag=X::X.get_fragment_by_time(fragseq);

        pieceFrame_frag_seq=fragseq;

        piece_cell_setter(this, frag);


    }



}

void timemachine_main::on_delpiece_clicked()
{

    constexpr auto seq_pos=0;
    std::vector<long> vfrag={};
    for(int row=0;;row++){
        if(ui->pieceWidget->item(row, 0));
        else break;


        for(int col=0;col<3;col++){
            if(ui->pieceWidget->item(row, col)->isSelected()){
                auto seq=std::stoi(ui->pieceWidget->item(row, seq_pos)->text().toStdString());

                vfrag.push_back(seq);break;

            }
        }
    }

    for(auto seqn : vfrag){
        X::X.delete_time_piece(seqn);
    }



    ui->pieceWidget->clearContents();
    auto & frag=X::X.get_fragment_by_time(pieceFrame_frag_seq);
    piece_cell_setter(this, frag);



}

void timemachine_main::on_delpiece_2_clicked()
{
    //smoothed_generic_one_breath(ui->delpiece_2, this, 1, 0, 500);
    generic_fade(ui->frame_2, this, 500);
    csleep(500);
    ui->frame_2->hide();

}

void timemachine_main::on_addpiece_clicked()
{
    constexpr auto seq_pos=0;
    long seq;
    for(int row=0;;row++){
        if(ui->pieceWidget->item(row, 0));
        else break;


        for(int col=0;col<3;col++){
            if(ui->pieceWidget->item(row, col)->isSelected()){
                seq=std::stoi(ui->pieceWidget->item(row, seq_pos)->text().toStdString());
                break;
            }
        }
    }

    //editor now appears.

}

void timemachine_main::on_back_clicked()
{
    smoothed_generic_one_breath(ui->back, this, 0.8, 0, 500);
    auto l=new admin_cockpit(0, FROZENP);
    auto ani= fade_close(this, 1000);
    connect(ani, SIGNAL(finished()), l, SLOT(show()));
    csleep(1000);
}

void timemachine_main::on_melt_clicked()
{


    smoothed_generic_one_breath(ui->melt, this, 0.8, 0, 1000);
    X::X.melt();
    X::X.update_time();//in order to update time.
    X::X.travel_to_time(X::X.get_global_time());
    X::X.melt();
    //auto any=travel_jump(this, 1000);
    //connect(any, SIGNAL(finished()), any, SLOT(start()));
        auto l=new admin_cockpit(0, FROZENP);
      fade_close(this, 3000);
     //connect(ani, SIGNAL(finished()), l, SLOT(show()));
     auto mov=new interval_movie(0,0);
     mov->show();
     connect(mov, SIGNAL(closing()), l, SLOT(show()));


}

