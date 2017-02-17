#include "kompiler.h"
#include "ui_kompiler.h"
#include "TIME_MACHINE.h"
#include "admin_cockpit.h"
#include <sstream>
#include <QChar>
kompiler::kompiler(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::kompiler)
{
    ui->setupUi(this);
}

kompiler::~kompiler()
{
    delete ui;
}

void kompiler::on_compile_clicked()
{

    auto & outs=X::X.output;
    outs.str("");

    auto text=ui->editor->toPlainText().toStdString();
    std::stringstream ss;
    ss<<text;
    X::X.instant_stream_compile(ss, true);

    ui->compiler_output->clear();

    char s[500];
    while(not outs.eof()){
        outs.getline(s,256);
        std::string str(s);
        ui->compiler_output->textCursor().insertText(QString::fromStdString(str)+"\n");

    }

       outs.clear();
}

void kompiler::on_exit_clicked()
{
    emit closed();
    if(not FROZENP){
        auto atm=new admin_cockpit(0, FROZENP);
        atm->show();
    }


    this->close();

}

void kompiler::on_compile_only_clicked()
{
    auto & outs=X::X.output;
    outs.str("");

    auto text=ui->editor->toPlainText().toStdString();
    std::stringstream ss;
    ss<<text;
    X::X.instant_stream_compile(ss, false);

    ui->compiler_output->clear();

    char s[500];
    while(not outs.eof()){
        outs.getline(s,256);
        std::string str(s);
        ui->compiler_output->textCursor().insertText(QString::fromStdString(str)+"\n");

    }

       outs.clear();
}
