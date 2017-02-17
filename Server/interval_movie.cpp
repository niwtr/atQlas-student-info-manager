#include "interval_movie.h"
#include "ui_interval_movie.h"
#include <QLabel>
#include <QMovie>
#include "common_animation.h"
#include<iostream>
#include <QtMultimedia/QSound>
#include <QtMultimedia/QMediaPlayer>
using std::cout;
interval_movie::interval_movie(QWidget *parent, int mode) :
    QWidget(parent),
    ui(new Ui::interval_movie)
{

    ui->setupUi(this);
    appear(this);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowFlags(windowFlags()&~Qt::WindowCloseButtonHint&~Qt::WindowContextHelpButtonHint);


    this->lb = new QLabel(this);
    lb->setStyleSheet("background-color: transparent;");
    lb->setGeometry(0, 0, 800, 600);

    QSound::play(":/atqres_ico/travel.wav");



    if(mode==0)
        mv=new QMovie(":/atqres_ico/melt.gif");
    else
        mv=new QMovie(":/atqres_ico/freeze.gif");
    lb->setMovie(mv);
    lb->setScaledContents(true);
    mv->start();
    connect(mv, SIGNAL(frameChanged(int)), this, SLOT(frame_handler(int)));
    //connect(mv, SIGNAL)

}

interval_movie::~interval_movie()
{
    delete ui;
}
