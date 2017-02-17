#ifndef TALKCLIENT_H
#define TALKCLIENT_H

#include <QtNetwork>
#include <QString>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include "csv.h"
using std::endl;
using std::cout;
using std::string;
using svec=std::vector<string>;

#define _toSky(X) sky::tc->talk_to_server(X)
#define fromSky() sky::tc->receive_msg()
#define SEQ sky::tc->getSeq()


#define toSky(X) _toSky(SEQ+"\n"+X)

namespace sky {

using sstream=std::stringstream;
class talkclient;

class talkthread;
extern talkclient * tc;

}

class sky::talkclient : public QObject{

    Q_OBJECT

    __PBUILDERF(int, seq, private)
    __PBUILDERF(string, Seq, private)

public:


     explicit talkclient(QHostAddress servip=QHostAddress::LocalHost,qint64 sendport=45454); //creator.
    //~talkclient();

      void talk_to_server(std::string msg);

      void hello_server();


      std::string receive_msg();


private:
      bool connection_is_healthy;

      bool recvp;
      QHostAddress servip;
      qint64 sendport;

      QUdpSocket * tone;
      QUdpSocket * ear;
      QUdpSocket * nose;
      QString msg;
private slots:
      void sniff();
      void disconnection_alert();

signals:
      void freeze();
      void melt();
};



#endif // TALKCLIENT_H
