#ifndef CAPSULE_H
#define CAPSULE_H

#include <QMainWindow>
#include "TIME_MACHINE.h"
namespace Ui {
class capsule;
}

class capsule : public QMainWindow
{
    Q_OBJECT

public:
    explicit capsule(QWidget *parent = 0, long fragseq=0);
    ~capsule();

Ui::capsule *ui;
    long fragseq;
private slots:
    void on_del_clicked();

    void on_add_clicked();

    void update_cells();

    void on_pushButton_clicked();

private:

};

#endif // CAPSULE_H
