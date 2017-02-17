#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>

namespace Ui {
class login;
}

class login : public QMainWindow
{
    Q_OBJECT

public:
     Ui::login *ui;
    explicit login(QWidget *parent = 0);
    ~login();

private slots:


    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();
private:

       int mode=0;  //0 for login, 1 for creating.
       int current=0; //0 for new, 1 for old.
};

#endif // LOGIN_H
