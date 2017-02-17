#ifndef KOMPILER_H
#define KOMPILER_H

#include <QMainWindow>
#include <QSyntaxHighlighter>
#include "TIME_MACHINE.h"
namespace Ui {
class kompiler;
}

class kompiler : public QMainWindow
{
    Q_OBJECT

public:
    explicit kompiler(QWidget *parent = 0);
    ~kompiler();

private slots:
    void on_compile_clicked();
    void on_exit_clicked();


    void on_compile_only_clicked();

signals:
    void closed();


private:
    Ui::kompiler *ui;
};



#endif // KOMPILER_H
