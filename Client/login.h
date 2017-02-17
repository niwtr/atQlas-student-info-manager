#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include "talkclient.h"
#include "common_animation.h"
namespace Ui {
class login;
}

class login : public QMainWindow
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();
  Ui::login *ui;
private slots:
    void on_ok_clicked();

    void on_back_clicked();

private:

};

#endif // LOGIN_H
