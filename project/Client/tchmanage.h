#ifndef TCHMANAGE_H
#define TCHMANAGE_H

#include <QMainWindow>
#include <string>
#include "talkclient.h"
#include <vector>
#include "csv.h"
#include "common_animation.h"
namespace Ui {
class tchmanage;
}

class tchmanage : public QMainWindow
{
    Q_OBJECT

public:
        Ui::tchmanage *ui;
    explicit tchmanage(QWidget *parent = 0, std::__1::string tid="");
    ~tchmanage();

    void parse_teacher_property_block(std::string blk);

    void set_my_lesson_table(std::string blk);

    void set_available_lesson_table(std::string blk);
private slots:
    void on_addl_clicked();

    void on_add_clicked();

    void on_filter_textChanged(const QString &arg1);

    void on_detail_clicked();

    void on_del_clicked();

    void go_frozen();
    void go_melt();

private:

};

#endif // TCHMANAGE_H
