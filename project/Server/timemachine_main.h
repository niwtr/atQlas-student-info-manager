#ifndef TIMEMACHINE_MAIN_H
#define TIMEMACHINE_MAIN_H

#include <QMainWindow>
#include "TIME_MACHINE.h"
#include <QDateTime>
#include <QTime>
#include <QGestureEvent>
namespace Ui {
class timemachine_main;
}

class timemachine_main : public QMainWindow
{
    Q_OBJECT

public:
    explicit timemachine_main(QWidget *parent = 0, bool frozen=false);
    ~timemachine_main();

    bool frozen;


    bool time_label_freezing=false;

    bool already_showing=false;

    void freeze_time_label();
    inline void melt_time_label(){this->time_label_freezing=false;}



    Ui::timemachine_main *ui;

private:


private slots:
    void show_time_in_label();
    void on_hourd_valueChanged(int value);
    void on_mind_valueChanged(int value);
    void on_secd_valueChanged(int value);
    void on_cancel_clicked();
    void on_travel_clicked();
    void on_del_clicked();
    void on_show_frag_clicked();
    void on_fragmentWidget_cellDoubleClicked(int row, int column);
    void on_delpiece_clicked();
    void on_delpiece_2_clicked();
    void on_addpiece_clicked();
    void on_back_clicked();
    void on_melt_clicked();
};

#endif // TIMEMACHINE_MAIN_H
