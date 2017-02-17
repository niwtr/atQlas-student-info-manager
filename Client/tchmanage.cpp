#include "tchmanage.h"
#include "ui_tchmanage.h"
#include "talkclient.h"
#include "def.h"
#include "block_encapsules.h"
#include "tchmanager_detail.h"
using std::string;
using std::endl;
using sstream=std::stringstream;
using svec=std::vector<std::string>;


constexpr Var MAX_ROW_COUNT=100;


Func(__morph_things, Var ui, Var f1, Var f2, Var pred){
    pred?f1(ui->add):f2(ui->add);
    pred?f1(ui->filter):f2(ui->filter);
    (not pred)?f1(ui->del):f2(ui->del);
};
Func(hide_things, Var ui, Var __this){
    __morph_things(ui,[&](Var thing){generic_appear(thing, __this, 500);},
    [&](Var thing){generic_fade(thing, __this, 500);},
    false);
};
Func(show_things, Var ui, Var __this){
    __morph_things(ui,[&](Var thing){generic_appear(thing, __this, 500);},
    [&](Var thing){generic_fade(thing, __this, 500);},
    true);
};



tchmanage::tchmanage(QWidget *parent, std::__1::string tid) :
    QMainWindow(parent),
    ui(new Ui::tchmanage)
{
    ui->setupUi(this);

    set_opacity(ui->blkframe, this, 0);

    connect(sky::tc, SIGNAL(freeze()), this, SLOT(go_frozen()));
    connect(sky::tc, SIGNAL(melt()), this, SLOT(go_melt()));

    ui->lessont->setSelectionBehavior ( QAbstractItemView::SelectRows);

    ui->lessont->horizontalHeader()->setStretchLastSection(true);


    toSky(encapsule_in_query_block("tid:"+tid+",\n""ask:ALL_INFORMATION"));

    Var blk=fromSky();

    parse_teacher_property_block(blk);

    toSky(encapsule_in_query_block("tid:"+tid+",\n""ask:LESSON_TABLE"));

    blk=fromSky();

    set_my_lesson_table(blk);

    hide_things(this->ui, this);

}



tchmanage::~tchmanage()
{
    delete ui;
}


Func(wordeq, Var word, Var supposed){return word==supposed;};
#define ____L_O_V____ \
    X(tid)\
    X(name)\
    X(school)


#define X(Y) Func(Y##p, Var wd){return wordeq(wd, #Y);};
____L_O_V____
#undef X

void tchmanage::parse_teacher_property_block(std::__1::string blk)
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

void tchmanage::set_my_lesson_table(std::__1::string blk)
{

    ui->lessont->clearContents();
    QStringList hd1;
    hd1<<"LID"<<"Name"<<"Credit"<<"Grade"<<"optional?"<<"students";
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

}

void tchmanage::set_available_lesson_table(std::__1::string blk)
{


    ui->lessont->clearContents();
    QStringList hd2;
    hd2<<"LID"<<"Name"<<"Credit"<<"Grade"<<"optional?"<<"students";
    Var hd2_len=hd2.length();


    ui->lessont->setColumnCount(hd2.count());
    ui->lessont->setRowCount(MAX_ROW_COUNT);

    ui->lessont->setHorizontalHeaderLabels(hd2);


    svec sv;
    csv_split(blk, sv);

    unsigned long count=0;
    Var row=0;
    while(count<sv.size()){
        for_through(col, 0, hd2_len, 1){
            Var str=sv[count];++count;
            ui->lessont->setItem(row, col, new QTableWidgetItem(QString::fromStdString(str)));
        }
        ++row;
    }

}











void tchmanage::on_addl_clicked()
{


    generic_one_breath(ui->lessont ,this, 1, 0, 1000);
    generic_one_breath(ui->addl, this, 1,0,1000);

    static Var plusp=false;

    plusp=not plusp;

    ui->lessont->clearContents();

    Var tid=ui->tid->text().toStdString();
    string blk;
    if(plusp){

        toSky(encapsule_in_query_block("tid:"+tid+",\n""ask:AVAILABLE_TABLE"));

        blk=fromSky();

        set_available_lesson_table(blk);

        show_things(this->ui, this);
    }

    else{

        toSky(encapsule_in_query_block("tid:"+tid+",\n""ask:LESSON_TABLE"));

        blk=fromSky();

        set_my_lesson_table(blk);

        hide_things(this->ui, this);

    }

}

void tchmanage::on_add_clicked()
{

    generic_one_breath(ui->add, this, 1,0,1000);

    Var tid=ui->tid->text().toStdString();
    for_through(row, 0, MAX_ROW_COUNT, 1){
        Var the_item=ui->lessont->item(row, 0);
        if(the_item!=nullptr and the_item->isSelected()){

            toSky(encapsule_in_code_block("as-for teacher "+tid+" do add-lesson that{l:"\
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


void tchmanage::on_filter_textChanged(const QString &arg1)
{
    Var str=colon_to_bar(arg1.toStdString());

    Var tid=ui->tid->text().toStdString();
    toSky(encapsule_in_query_block("tid:"+tid+",\n""xstr:\""+str+"\",\n""ask:AVAILABLE_TABLE_X"));

    Var blk=fromSky();

    set_available_lesson_table(blk);
}

void tchmanage::on_detail_clicked()
{
    generic_one_breath(ui->detail, this, 1, 0.5, 1000);

    for_through(row, 0, MAX_ROW_COUNT, 1){
        Var it=ui->lessont->item(row,0);
        if(it!=nullptr and it->isSelected()){
            Var lid=it->text().toStdString();
            Var tmd=new tchmanager_detail(0, lid);
            tmd->show();break;
        }
    }
}

void tchmanage::on_del_clicked()
{

    generic_one_breath(ui->del, this, 1, 0.5, 1000);

    Var tid=ui->tid->text().toStdString();
    for_through(row, 0, MAX_ROW_COUNT, 1){
        Var the_item=ui->lessont->item(row, 0);
        if(the_item!=nullptr and the_item->isSelected()){

            toSky(encapsule_in_code_block("as-for teacher "+tid+" do delete-lesson that{l:"\
                                          +the_item->text().toStdString()+"}"));

        }
    }

    toSky(encapsule_in_query_block("tid:"+tid+",\n""ask:LESSON_TABLE"));

    Var blk=fromSky();

    set_my_lesson_table(blk);

    generic_one_breath(ui->lessont, this, 1, 0.5, 1000);

}


#define ____L_O_V____ \
    X(del)\
    X(add)\
    X(addl)\
    X(lessont)\
    X(detail)\
    X(filter)


void tchmanage::go_frozen()
{
    generic_appear(ui->blkframe, this, 2000);

#define X(name) ui->name->setEnabled(false);
    ____L_O_V____
        #undef X

}

void tchmanage::go_melt()
{
    generic_fade(ui->blkframe, this, 2000);

#define X(name) ui->name->setEnabled(true);
    ____L_O_V____
        #undef X
}

#undef ____L_O_V____
