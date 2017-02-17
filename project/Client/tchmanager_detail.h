#ifndef TCHMANAGER_DETAIL_H
#define TCHMANAGER_DETAIL_H

#include <QMainWindow>
#include <string>
#include "talkclient.h"
#include <vector>
#include "csv.h"
#include "common_animation.h"
using svec=std::vector<std::string>;
namespace Ui {
class tchmanager_detail;
}

class tchmanager_detail : public QMainWindow
{
    Q_OBJECT

public:
    explicit tchmanager_detail(QWidget *parent = 0, std::string lid="");
    ~tchmanager_detail();

    void parse_lesson_property_block(std::string blk);

    void set_students_table(std::string blk);

private slots:
    void on_lessont_cellChanged(int row, int column);

    void on_refresh_clicked();

private:
    Ui::tchmanager_detail *ui;
};

#endif // TCHMANAGER_DETAIL_H
