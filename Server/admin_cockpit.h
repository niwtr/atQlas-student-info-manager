#ifndef ADMIN_COCKPIT_H
#define ADMIN_COCKPIT_H

#include <QMainWindow>
#include "def.h"
namespace Ui {
class admin_cockpit;
}

class admin_cockpit : public QMainWindow
{
    Q_OBJECT

public:
    explicit admin_cockpit(QWidget *parent = 0, bool frozen=false);
    ~admin_cockpit();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_back_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::admin_cockpit *ui;
};

#endif // ADMIN_COCKPIT_H
