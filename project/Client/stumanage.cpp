#include "stumanage.h"
#include "ui_stumanage.h"
#include "def.h"
#include "block_encapsules.h"
using std::string;
using std::endl;
using sstream=std::stringstream;


defvar MAX_ROW_COUNT=100;



stumanage::stumanage(QWidget *parent, std::string sid) :
    QMainWindow(parent),
    ui(new Ui::stumanage)
{
    ui->setupUi(this);

    connect(sky::tc, SIGNAL(freeze()), this, SLOT(go_frozen()));
    connect(sky::tc, SIGNAL(melt()), this, SLOT(go_melt()));


    set_opacity(ui->blkframe, this, 0);

    ui->lessont->setSelectionBehavior ( QAbstractItemView::SelectRows);

    ui->lessont->horizontalHeader()->setStretchLastSection(true);

    toSky(encapsule_in_query_block("sid:"+sid+",\nask:ALL_INFORMATION"));

    Var blk=fromSky();

    parse_student_property_block(blk);

    toSky(encapsule_in_query_block("sid:"+sid+",\n""ask:LESSON_TABLE"));

    blk=fromSky();

    set_my_lesson_table(blk);

}

stumanage::~stumanage()
{
    delete ui;
}



Func(wordeq, Var word, Var supposed){return word==supposed;};
#define ____L_O_V____ \
    X(sid)\
    X(name)\
    X(school)\
    X(klass)\
    X(grade)\
    X(gpa)

#define X(Y) Func(Y##p, Var wd){return wordeq(wd, #Y);};

____L_O_V____

#undef X

void stumanage::parse_student_property_block(std::__1::string blk)
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

void stumanage::set_my_lesson_table(std::__1::string blk)
{

    ui->lessont->clearContents();
    QStringList hd1;
    hd1<<"LID"<<"Name"<<"Credit"<<"Grade"<<"optional?"<<"teachers"<<"Score";
    this->hd1_len=hd1.length();

    ui->lessont->setColumnCount(hd1.count());
    ui->lessont->setRowCount(100);

    ui->lessont->setHorizontalHeaderLabels(hd1);

    svec sv;
    csv_split(blk, sv);

    unsigned long count=0;
    Var row=0;
    while(count<sv.size()){
        for_through(col, 0, this->hd1_len, 1){
            Var str=sv[count];++count;
            ui->lessont->setItem(row, col, new QTableWidgetItem(QString::fromStdString(str)));
        }
        ++row;
    }

}

void stumanage::set_applicable_lesson_table(std::__1::string blk)
{
    ui->lessont->clearContents();
    QStringList hd2;
    hd2<<"LID"<<"Name"<<"Credit"<<"Grade"<<"optional?"<<"teachers";
    this->hd2_len=hd2.length();
    ui->lessont->setRowCount(100);
    ui->lessont->setColumnCount(hd2.count());

    ui->lessont->setHorizontalHeaderLabels(hd2);


    svec sv;
    csv_split(blk, sv);

    unsigned long count=0;
    Var row=0;
    while(count<sv.size()){
        for_through(col, 0, this->hd2_len, 1){
            Var str=sv[count];++count;
            ui->lessont->setItem(row, col, new QTableWidgetItem(QString::fromStdString(str)));
        }
        ++row;
    }
}

void stumanage::on_addl_clicked()
{
    generic_one_breath(ui->addl, this, 1, 0.2, 200);

    static Var plusp=false;
    plusp=not plusp;
    ui->lessont->clearContents();

    string blk;

    Var sid=ui->sid->text().toStdString();

    if(plusp){

        toSky(encapsule_in_query_block("sid:"+sid+",\n""ask:APPLICABLE_TABLE"));

        blk=fromSky();

        set_applicable_lesson_table(blk);

    }

    else{

        toSky(encapsule_in_query_block("sid:"+sid+",\n""ask:LESSON_TABLE"));

        blk=fromSky();

        set_my_lesson_table(blk);

    }



}

void stumanage::on_add_clicked()
{


    generic_one_breath(ui->add, this, 1, 0.2, 200);

    Var sid=ui->sid->text().toStdString();
    for_through(row, 0, MAX_ROW_COUNT, 1){
        Var the_item=ui->lessont->item(row, 0);
        if(the_item!=nullptr and the_item->isSelected()){

            toSky(encapsule_in_code_block("as-for student "+sid+" do add-lesson that{l:"\
                                          +the_item->text().toStdString()+"}"));

        }
    }



}

Func(colon_to_bar, Var raw){
    for_in(ch, raw)  {
        if(ch==':')
            ch='-';
    }return raw;
};

void stumanage::on_del_clicked()
{

    generic_one_breath(ui->del, this, 1, 0.2, 200);

    Var sid=ui->sid->text().toStdString();
    for_through(row, 0, MAX_ROW_COUNT, 1){
        Var the_item=ui->lessont->item(row, 0);
        if(the_item!=nullptr and the_item->isSelected()){

            toSky(encapsule_in_code_block("as-for student "+sid+" do delete-lesson that{l:"\
                                          +the_item->text().toStdString()+"}"));

        }
    }

    toSky(encapsule_in_query_block("sid:"+sid+",\n""ask:LESSON_TABLE"));

    Var blk=fromSky();

    set_my_lesson_table(blk);

}

void stumanage::on_filter_textChanged(const QString &arg1)
{
    Var str=colon_to_bar(arg1.toStdString());

    Var sid=ui->sid->text().toStdString();
    toSky(encapsule_in_query_block("sid:"+sid+",\n""xstr:\""+str+"\",\n""ask:APPLICABLE_TABLE_X"));

    Var blk=fromSky();

    set_applicable_lesson_table(blk);

}

#define ____L_O_V____ \
    X(add)\
    X(addl)\
    X(lessont)\
    X(del)\
    X(filter)

void stumanage::go_frozen()
{
#define X(name) ui->name->setEnabled(false);
    ____L_O_V____
        #undef X
            generic_appear(ui->blkframe, this, 2000)         ;
}

void stumanage::go_melt()
{
#define X(name) ui->name->setEnabled(true);
    ____L_O_V____
        #undef X
            generic_appear(ui->blkframe, this, 2000)            ;
}
#undef LOV
