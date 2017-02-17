#include "admin_manager_lesson_finder.h"
#include "ui_admin_manager_lesson_finder.h"

admin_manager_lesson_finder::admin_manager_lesson_finder(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::admin_manager_lesson_finder)
{
    ui->setupUi(this);
}

admin_manager_lesson_finder::~admin_manager_lesson_finder()
{
    delete ui;
}
