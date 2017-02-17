#include "capsule.h"
#include "ui_capsule.h"
#include "kompiler.h"
#include "common_animation.h"
#include "admin_cockpit.h"

auto piece_cell_setter=[](auto __this, auto & frag){
    auto row=0;auto col=0;
    auto & the_widget=__this->ui->pieceWidget;
    //set the cells.
    auto beautify=[&](){
        the_widget->item(row, col)->setTextAlignment(Qt::AlignCenter);
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
capsule::capsule(QWidget *parent, long fragseq) :
    QMainWindow(parent),
    ui(new Ui::capsule)
{

    ui->setupUi(this);

    this->fragseq=fragseq;
    std::cout<<fragseq<<std::endl;

    QStringList pheaders;
    pheaders<<"Sequence"<<"Category"<<"Original Block";

    ui->pieceWidget->setColumnCount(pheaders.count());
    ui->pieceWidget->setRowCount(100);

    ui->pieceWidget->setHorizontalHeaderLabels(pheaders);
    QHeaderView* pheaderView = ui->pieceWidget->verticalHeader();
    pheaderView->setHidden(true);
    ui->pieceWidget->horizontalHeader()->setStretchLastSection(true);

    ui->pieceWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto & frag=X::X.get_fragment_by_time(fragseq);

    piece_cell_setter(this, frag);



}


capsule::~capsule()
{
    delete ui;
}

void capsule::on_del_clicked()
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
    auto & frag=X::X.get_fragment_by_time(fragseq);
    piece_cell_setter(this, frag);


}

void capsule::on_add_clicked()
{
    auto km=new kompiler();
    km->show();
    connect(km, SIGNAL(closed()), this, SLOT(update_cells()));
}

void capsule::update_cells()
{

    ui->pieceWidget->clearContents();
    auto & frag=X::X.get_fragment_by_time(fragseq);
    piece_cell_setter(this, frag);
}

void capsule::on_pushButton_clicked()
{

    smoothed_generic_one_breath(ui->pushButton, this, 0.8, 0, 1000);
    auto l=new admin_cockpit(0, FROZENP);
    auto ani= fade_close(this, 1000);
    connect(ani, SIGNAL(finished()), l, SLOT(show()));
    csleep(1000);

}
