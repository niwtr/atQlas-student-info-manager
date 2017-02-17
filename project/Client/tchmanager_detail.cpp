#include "tchmanager_detail.h"
#include "ui_tchmanager_detail.h"


#include "def.h"
#include "block_encapsules.h"

defvar MAX_ROW_COUNT=100;

static Var table_is_ready=false;

using std::string;
using std::endl;
using sstream=std::stringstream;
using svec=std::vector<std::string>;


tchmanager_detail::tchmanager_detail(QWidget *parent, std::__1::string lid) :
    QMainWindow(parent),
    ui(new Ui::tchmanager_detail)
{
    ui->setupUi(this);


    ui->lessont->horizontalHeader()->setStretchLastSection(true);

    toSky(encapsule_in_query_block("lid:"+lid+",\n""ask:ALL_INFORMATION"));

    Var blk=fromSky();

    parse_lesson_property_block(blk);

    toSky(encapsule_in_query_block("lid:"+lid+",\n""ask:STU_TABLE"));

    blk=fromSky();

    set_students_table(blk);


    table_is_ready=true;



}

tchmanager_detail::~tchmanager_detail()
{
    delete ui;
}






Func(wordeq, Var word, Var supposed){return word==supposed;};
#define ____L_O_V____ \
    X(lid)\
    X(name)\
    X(credit)\
    X(grade)\
    X(optp)


#define X(Y) Func(Y##p, Var wd){return wordeq(wd, #Y);};

____L_O_V____

#undef X

void tchmanager_detail::parse_lesson_property_block(std::string blk)
{
    svec sv;
    csv_split(blk, sv);
    for_in(entry, sv){
        svec subsv;
        split_by_colon(entry, subsv);
        Var_ref title=subsv[0];
        Var content=QString::fromStdString(subsv[1]);
#define X(Y) elif(Y##p(title))ui->Y->setText(content);
        if(false);

        ____L_O_V____

        #undef X

    }

}

#undef ____L_O_V____

void tchmanager_detail::set_students_table(std::__1::string blk)
{
    table_is_ready=false;
    ui->lessont->clearContents();
    QStringList hd1;
    hd1<<"SID"<<"Name"<<"Result";
    Var hd1_len=hd1.length();

    ui->lessont->setColumnCount(hd1.count());
    ui->lessont->setRowCount(MAX_ROW_COUNT);

    ui->lessont->setHorizontalHeaderLabels(hd1);

    svec sv;
    csv_split(blk, sv);

    unsigned long count=0;
    Var row=0;
    while(count<sv.size()){
        for_through(col, 0, hd1_len, 1){
            Var str=sv[count];++count;
            ui->lessont->setItem(row, col, new QTableWidgetItem(QString::fromStdString(str)));
        }
        ++row;
    }

    table_is_ready=true;
}






void tchmanager_detail::on_lessont_cellChanged(int row, int column)
{

    if(not table_is_ready)return;
    smoothed_generic_one_breath(ui->lessont, this, 1,0,1000);
    if(column==2){
        Var it=ui->lessont->item(row, column);
        if(it!=nullptr){
            Var newv=it->text().toStdString();
            Var the_stuid=ui->lessont->item(row, 0)->text().toStdString();
            Var lid=ui->lid->text().toStdString();

            toSky(encapsule_in_code_block("as-for student "+the_stuid+" do set-score! "+lid+" "+newv));

            toSky(encapsule_in_query_block("lid:"+lid+",\n""ask:STU_TABLE"));

            Var blk=fromSky();

            set_students_table(blk);

        }

    }

}

void tchmanager_detail::on_refresh_clicked()
{

    smoothed_generic_one_breath(ui->refresh, this, 1,0,1000);

    Var lid=ui->lid->text().toStdString();

    table_is_ready=false;

    toSky(encapsule_in_query_block("lid:"+lid+",\n""ask:ALL_INFORMATION"));

    Var blk=fromSky();

    parse_lesson_property_block(blk);

    toSky(encapsule_in_query_block("lid:"+lid+",\n""ask:STU_TABLE"));

    blk=fromSky();

    set_students_table(blk);


    table_is_ready=true;
}
