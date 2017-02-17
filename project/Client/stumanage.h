#ifndef STUMANAGE_H
#define STUMANAGE_H

#include <QMainWindow>
#include <string>
#include "talkclient.h"
#include <vector>
#include "csv.h"
#include "common_animation.h"
using svec=std::vector<std::string>;
namespace Ui {
class stumanage;
}

class stumanage : public QMainWindow
{
    Q_OBJECT

public:
    explicit stumanage(QWidget *parent = 0, std::__1::string sid="");
    ~stumanage();

    void parse_student_property_block(std::string blk);

    void set_my_lesson_table(std::string blk);

    void set_applicable_lesson_table(std::string blk);


private slots:
    void on_addl_clicked();

    void on_add_clicked();

    void on_del_clicked();

    void on_filter_textChanged(const QString &arg1);

    void go_frozen();
    void go_melt();

private:
    Ui::stumanage *ui;

    short hd1_len;
    short hd2_len;

};

#endif // STUMANAGE_H
