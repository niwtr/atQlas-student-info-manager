#include "atqlas_main.h"
#include <QApplication>
#include "def.h"
#include <string>
#include <vector>
#include "csv.h"
#include <iostream>
#include "talkclient.h"
#include "login.h"
#include <QTimer>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    sky::tc->hello_server();

    //login w;
    atQlas_main w;
    w.show();

    return a.exec();
}
