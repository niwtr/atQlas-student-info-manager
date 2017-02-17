#ifndef TALKSERVER_H
#define TALKSERVER_H
#include <map>
#include <vector>
#include <QtNetwork>
#include "defschool.h"
#include "def.h"
#include <string>
#include <QThread>
//#include "common_animation.h"
using std::map;
using std::vector;
using std::string;
using svec=vector<std::string>;
class client_info {
     __PBUILDERF(std::string, username, private)//port采用默认端口。
     __PBUILDERF(unsigned int, ttl, private)

     public:
         QHostAddress ipaddr;
         client_info(std::string ipstr){
             this->ipaddr=QHostAddress(QString::fromStdString(ipstr));
             setttl(10);
         }

};


class talkserver {

    __PBUILDERF(string, _current_working_seq, private)

public:


    explicit talkserver();

    void yell(string blk);

    void reg_new_client(string userid, string ipstr);

    void talk_to_client(string userid, std::__1::string info_blk);//使用ID来标识

    void talk_to_addr(string addr, std::string info_blk);

    void interpreter(string blk);

    void proc_blk_hello(std::string blk);

    void proc_blk_login(std::string blk);

    void proc_blk_query(std::string blk);
    void handle_student_case(std::string domain, std::__1::string sid, std::string xstr="");
    void handle_teacher_case(std::string domain, std::string tid, std::string xstr="");
    void handle_lesson_case(std::string domain, std::string lid, std::string xstr="");

    void proc_blk_code(std::string blk);



    void status_cycle(); //服务循环。
protected:

       map<string, client_info> clientmap;

private:



    int default_send_port=45453;
    int default_recv_port=45454;
    int default_mouth_port=45452;

    QUdpSocket * tone;
    QUdpSocket * ear;

    QUdpSocket * mouth;


};


class talkthread : public QThread, public talkserver{

    Q_OBJECT

protected:
    void run(){
        status_cycle();
    }

private slots:
   void uptate_client_time();
};

#define toSky(X) this->talk_to_client(this->get_current_working_seq(),X)



#endif // TALKSERVER_H
