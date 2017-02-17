#ifndef INTERVAL_MOVIE_H
#define INTERVAL_MOVIE_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <string>
namespace Ui {
class interval_movie;
}

class interval_movie : public QWidget
{
    Q_OBJECT

public:
    explicit interval_movie(QWidget *parent = 0, int mode=0);
    QLabel* lb;
    QMovie* mv;
    ~interval_movie();
public slots:
    void frame_handler(int cf){if(cf==mv->frameCount()-1){mv->stop(); emit closing();this->close();}}
private:
    Ui::interval_movie *ui;

signals:
    void closing();
};

#endif // INTERVAL_MOVIE_H
