#include "talkclient.h"
#include <QByteArray>
#include <string>
#include "common_animation.h"
#include <cstdlib>
#include <QTimer>
#include <QMessageBox>
sky::talkclient::talkclient(QHostAddress ip, qint64 iport)
{
    tone = new QUdpSocket();
    ear = new QUdpSocket();
    nose = new QUdpSocket();

    ear->bind(ip, 45453);
    nose->bind(ip, 45452);

    this->servip=ip;
    this->sendport=iport;

}

void sky::talkclient::talk_to_server(std::__1::string msg)
{
    if(recvp)recvp=false;
    Var data=QByteArray::fromStdString(msg);
    cout<<"Sent data:"<<endl<<data.data()<<endl;
    tone->writeDatagram(data.data(), data.size(), servip, sendport);
}

void sky::talkclient::hello_server()
{
    Var seq=rand()%5000;


    sstream ss;
    ss<<seq;
    string Seq;
    ss>>Seq;
    this->setSeq(Seq);
    string blk=string("[hello]\n")+string("random seq:")+Seq+
            string(",\n")+string("address:")+"127.0.0.1"+string("\n[\\hello]");

    toSky(blk);

    Var flag=false; svec vs;string str="";

    while(not flag){

    Var block=fromSky();

    if(block=="")break;

    split_by_ret(block, vs);

    if(vs[0]=="0")flag=true;

    str=vs[2];

}

    if(str=="respect"){
        this->setseq(seq);

        Var tt=new QTimer();
        connect(tt, SIGNAL(timeout()), this, SLOT(sniff()));
        tt->start(50);
    }

    else hello_server();


}




std::__1::string sky::talkclient::receive_msg()
{
    connection_is_healthy=true;

    Var qt=new QTimer(this);
    connect(qt, SIGNAL(timeout()), this, SLOT(disconnection_alert()));
    connect(qt, SIGNAL(timeout()), qt, SLOT(stop()));
    qt->start(1000);

    while(not ear->hasPendingDatagrams()){
        if(not connection_is_healthy)return "";
        else csleep(10);
    }
        QByteArray datagram;   //用于存放接收的数据
        datagram.resize(ear->pendingDatagramSize()); //让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
        ear->readDatagram(datagram.data(),datagram.size());  //读取数据包，将其存放在datagram中
        Var msg=std::string(datagram.data());

        qt->stop();
        return msg;

}

void sky::talkclient::sniff()
{
    if(nose->hasPendingDatagrams()){
    QByteArray datagram;   //用于存放接收的数据
    datagram.resize(nose->pendingDatagramSize()); //让datagram的大小为等待处理的数据报的大小，这样才能接收到完整的数据
    nose->readDatagram(datagram.data(),datagram.size());  //读取数据包，将其存放在datagram中
    Var msgg=std::string(datagram.data());
    if(msgg=="freeze"){
        emit freeze();
    }
    else if(msgg=="melt"){
        emit melt();
    }

    }
}

void sky::talkclient::disconnection_alert()
{
    auto jump_out_alert_window=[](std::string str){
        QMessageBox::critical(0 ,
                              "critical message" , QString::fromStdString(str),
                              QMessageBox::Ok | QMessageBox::Default ,
                              QMessageBox::Cancel | QMessageBox::Escape , 	0 );

    };
    this->connection_is_healthy=false;
    jump_out_alert_window("The server has no response.");
    //cout<<"server no response"<<endl;
}

Var sky::tc=new sky::talkclient();
