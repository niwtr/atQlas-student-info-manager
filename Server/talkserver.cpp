#include "talkserver.h"
#include <iostream>
#include "TIME_MACHINE.h"
#include <sstream>
#include "common_animation.h"

#include "account.h"
#include <QTimer>
using sstream=std::stringstream;
using std::pair;
talkserver::talkserver()
{
    tone=new QUdpSocket();
    //tone->bind(default_send_port, QUdpSocket::ShareAddress);
    ear=new QUdpSocket();
    ear->bind(default_recv_port, QUdpSocket::ShareAddress);

    mouth=new QUdpSocket();

    //X::cout<<"hello"<<X::endl;
}

void talkserver::yell(std::__1::string blk)
{
    Var byteblk=QByteArray(blk.data());
    if(this->mouth->writeDatagram(byteblk, QHostAddress::LocalHost, default_mouth_port)>-1)
        X::cout<<"yelled: "<<blk<<X::endl;
}

void talkserver::reg_new_client(std::__1::string userid, std::__1::string ipstr)
{
    Var client=client_info(ipstr);
    this->clientmap.insert(pair<string, client_info>(userid, client));
}

void talkserver::talk_to_client(std::__1::string userid, std::string info_blk)
{
    Var the_record=this->clientmap.find(userid);
    if(the_record==clientmap.end())return;

    Var c_addr=the_record->second.ipaddr;

    Var byteblk=QByteArray(info_blk.data());
    if(this->tone->writeDatagram(byteblk, c_addr, default_send_port)>-1)
        std::cout<<"Talking to client: "<<std::endl<<byteblk.data()<<std::endl;
    else std::cout<<"failed to write packet."<<std::endl;
}

void talkserver::talk_to_addr(std::__1::string addr, std::__1::string info_blk)
{
    Var byteblk=QByteArray(info_blk.data());
    this->tone->writeDatagram(byteblk, QHostAddress(QString::fromStdString(addr)) , default_send_port);
}

void talkserver::interpreter(std::__1::string blk)
{

    sstream ss;
    ss<<blk;

    char u[256];
    ss.getline(u, 256);


    string acc;
    bool in_block=false;
    string current_block="";
    bool packed=false;
    char s[256];
    //bool listening=true;  //wether we should receive a block label.
    while(!ss.eof()){
        ss.getline(s,256);

        string str(s);
        auto open_block=[&](string domain){
            in_block=true;
            current_block=domain;
        };
        auto close_block=[&](string domain){
            if(in_block and current_block==domain){
                in_block=false;
                packed=true;
            }
            else {;}//encounter unmatched closure.
        };


        if (str == "[hello]") {
            open_block("hello");
            continue;
        }
        elif (str == "[login]"){
            open_block("login");
            continue;
        }
        elif (str == "[query]"){
            open_block("query");
            continue;
        }
        elif(str=="[code]"){
            open_block("code");
            continue;
        }


        if(str=="[\\hello]"){
            close_block("hello");
        }
        elif(str=="[\\login]"){
            close_block("login");
        }
        elif(str=="[\\query]"){
            close_block("query");
        }
        elif(str=="[\\code]"){
            close_block("code");
        }



        if(in_block){
            acc.append(str);
        }
        if(packed){
            if(current_block=="hello")
                proc_blk_hello(acc);

            elif(current_block=="login"){
                proc_blk_login(acc);
            }
            elif(current_block=="query"){
                proc_blk_query(acc);
            }
            elif(current_block=="code"){
                proc_blk_code(acc);
                X::cout<<"handling code blocks"<<X::endl;
            }

            else ;

            packed=false;
            acc.clear();
        }
    }

    if(not acc.empty()) {
        if(current_block!="hello")
            proc_blk_hello(acc);
        elif(current_block=="login"){
            proc_blk_login(acc);
        }
        elif(current_block=="query"){
            proc_blk_query(acc);
        }
        elif(current_block=="code"){

            proc_blk_code(acc);
        }

        else ;
    }


}



void  talkserver::proc_blk_hello(std::__1::string blk)
{


    if(X::X.frozenp())return;

    svec sv;
    X::split_by_comma(blk, sv);

    string ipaddr;
    string new_seq;
    Var flag=0;


    for_in(codestr, sv){
        svec subsv;
        X::split_by_colon(codestr, subsv);
        Var feild=subsv[0];Var code=subsv[1];
        if(feild=="random seq"){

            if(this->clientmap.find(code)==clientmap.end()){
                flag=0;new_seq=code;
            }
            else {
                flag=1;
            }
        }
        elif(feild=="address"){
            ipaddr=code;flag+=10;
        }
    }

    string lab="0\n";
    if(flag==10){
        string block=lab+string("[hello]")+string("\nrespect\n")+string("[\\hello]");;
        reg_new_client(new_seq, ipaddr);
        talk_to_client(new_seq, block);
    }
    elif(flag==11){
        string block=lab+string("[hello]\n")+string("refuse\n")+string("[\\hello]");
        talk_to_addr(ipaddr, block);
    }

}

void talkserver::proc_blk_login(std::__1::string blk)
{
    svec sv;
    X::split_by_comma(blk, sv);

    string username="DEF";
    string passwd="DEF";

    for_in(codestr, sv){
        svec subsv;
        X::split_by_colon(codestr, subsv);
        Var feild=subsv[0];Var code=subsv[1];
        if(feild=="name"){
            username=code;
        }
        elif(feild=="passwd"){
            passwd=code;
        }



    }

    Var the_user=defschool::adb_binding->manage_login(username, passwd);

    Var PTY=the_user->getpriority();
    Var PTYS="UNKNOWN_PTY";
    switch(PTY){
    case (defschool::STU_PTY):
        PTYS="STU_PTY";break;
    case(defschool::TEC_PTY):
        PTYS="TCH_PTY";break;
    default:break;
    }


    toSky(PTYS);

}

Func(csv_split, Var_ref blk, Var_ref container){
    Var count=0;
    Var in_quote=false;
    Var acc=string("");
    for(Var ch : blk){
        if(ch=='\"'){
            in_quote=not in_quote;
        }
        elif(ch==',' and (not in_quote)){
                container.push_back(acc);++count;
                acc.clear();
        }
        else{
            unless((not in_quote) and ch=='\"')
            acc+=ch;
        }
    }
    if(not acc.empty())container.push_back(acc);
    return count;
};

void talkserver::proc_blk_query(std::__1::string blk)
{

    svec sv;
    //X::split_by_comma(blk, sv);
    csv_split(blk, sv);

    Var charp=0; //0 for stu, 1 for teacher.
    string domain="";
    string username="";
    string xstr="";
    for_in(str, sv){
        svec subsv;
        X::split_by_colon(str, subsv);

        Var feild=subsv[0];
        Var code=subsv[1];

        if(feild=="sid"){charp=0;username=code;}
        elif(feild=="tid"){charp=1;username=code;}
        elif(feild=="lid"){charp=2;username=code;}
        elif(feild=="ask")domain=code;
        elif(feild=="xstr")xstr=code;

    }


    if(charp==0)handle_student_case(domain, username, xstr);
    elif(charp==1)handle_teacher_case(domain, username, xstr);
    elif(charp==2)handle_lesson_case(domain,username,xstr);



}
Func(bar_to_colon, Var raw){
  for_in(ch, raw)  {
      if(ch=='-')
          ch=':';
  }return raw;
};

void talkserver::handle_student_case(std::__1::string domain, std::string sid, std::__1::string xstr)
{
    Func(itos, Var i){
        sstream ss;
        ss<<i;
        string s;
        ss>>s;return s;
    };

    Var stu=defschool::karacter::cb_binding->get_student_of_sid(sid);
    if(domain=="ALL_INFORMATION"){

        toSky("sid:"+stu->getSID()+","+"name:"+stu->getname()+","+\
              "grade:"+stu->getgrade_s()+","+"klass:"+stu->getklass()+","+\
              "gpa:"+stu->get_GPA_s()+",school:"+stu->getschool());

    }

    elif(domain=="LESSON_TABLE"){
        string ss;
        std::vector<defschool::lesson_sptr> current;
        stu->get_required_lessons(current);
        stu->get_optional_lessons(current);

#define insert_comma ss+=","
        for_in(it, current){
            ss+=it->getLID();insert_comma;
            ss+=it->getname();insert_comma;
            ss+=itos(it->getcredit());insert_comma;
            ss+=itos(it->getgrade());insert_comma;
            ss+=(it->getopt_p()?"yes":"no");insert_comma;
            ss+="\"";
            for_in(tch, it->get_teacher_binding()){
                ss+=tch->getname();insert_comma;
            }ss+="\"";insert_comma;
            ss+=itos(stu->get_score_of_lesson(it->getLID()));
            insert_comma;
        }
#undef insert_comma


        toSky(ss);
    }

    elif(domain=="APPLICABLE_TABLE"){
        string ss;
        std::vector<defschool::lesson_sptr> current;
        //stu->get_applicable_lessons(current);
        defschool::karacter::lb_binding->get_applicable_lessons(stu, current);

#define insert_comma ss+=","
        for_in(it, current){
            ss+=it->getLID();insert_comma;
            ss+=it->getname();insert_comma;
            ss+=itos(it->getcredit());insert_comma;
            ss+=itos(it->getgrade());insert_comma;
            ss+=(it->getopt_p()?"yes":"no");insert_comma;
            ss+="\"";
            for_in(tch, it->get_teacher_binding()){
                ss+=tch->getname();insert_comma;
            }ss+="\"";insert_comma;
        }
#undef insert_comma


        toSky(ss);
    }
    elif(domain=="APPLICABLE_TABLE_X"){

        string ss;
        std::vector<defschool::lesson_sptr> current;

        X::parse_and_assoc_applicable_lesson(bar_to_colon(xstr), stu, current);

#define insert_comma ss+=","
        for_in(it, current){
            ss+=it->getLID();insert_comma;
            ss+=it->getname();insert_comma;
            ss+=itos(it->getcredit());insert_comma;
            ss+=itos(it->getgrade());insert_comma;
            ss+=(it->getopt_p()?"yes":"no");insert_comma;
            ss+="\"";
            for_in(tch, it->get_teacher_binding()){
                ss+=tch->getname();insert_comma;
            }ss+="\"";insert_comma;
        }
#undef insert_comma


        toSky(ss);

    }


}

void talkserver::handle_teacher_case(std::__1::string domain, std::__1::string tid, std::string xstr)
{

    Func(itos, Var i){
        sstream ss;
        ss<<i;
        string s;
        ss>>s;return s;
    };

    Var tch=defschool::karacter::cb_binding->get_teacher_of_tid(tid);
    //X(tid);X(name);X(school);
    if(domain=="ALL_INFORMATION"){

        toSky("tid:"+tch->getTID()+","+"name:"+tch->getname()+","+"school:"+tch->getschool());

    }

    elif(domain=="LESSON_TABLE"){
        string ss;
        std::vector<defschool::lesson_sptr> current;
        tch->get_my_lessons(current);

#define insert_comma ss+=","
        for_in(it, current){
            ss+=it->getLID();insert_comma;
            ss+=it->getname();insert_comma;
            ss+=itos(it->getcredit());insert_comma;
            ss+=itos(it->getgrade());insert_comma;
            ss+=(it->getopt_p()?"yes":"no");insert_comma;
            ss+="\"";
            for_in(tch, it->get_student_binding()){
                ss+=tch->getname();insert_comma;
            }ss+="\"";insert_comma;
        }
#undef insert_comma

        X::cout<<ss<<X::endl;
        toSky(ss);
    }

    elif (domain=="AVAILABLE_TABLE"){


        string ss;
        auto lb_binding=defschool::karacter::lb_binding->get_lessons();
        std::vector<defschool::lesson_sptr> l_container;
        for(auto the_pair : lb_binding){
            l_container.push_back(the_pair.second);
        }



#define insert_comma ss+=","
        for_in(it, l_container){
            ss+=it->getLID();insert_comma;
            ss+=it->getname();insert_comma;
            ss+=itos(it->getcredit());insert_comma;
            ss+=itos(it->getgrade());insert_comma;
            ss+=(it->getopt_p()?"yes":"no");insert_comma;
            ss+="\"";
            for_in(tch, it->get_student_binding()){
                ss+=tch->getname();insert_comma;
            }ss+="\"";insert_comma;
        }
#undef insert_comma

        toSky(ss);
    }

    elif(domain=="AVAILABLE_TABLE_X"){

        string ss;
        std::vector<defschool::lesson_sptr> current;

        X::parse_and_assoc_base(bar_to_colon(xstr), current);

#define insert_comma ss+=","
        for_in(it, current){
            ss+=it->getLID();insert_comma;
            ss+=it->getname();insert_comma;
            ss+=itos(it->getcredit());insert_comma;
            ss+=itos(it->getgrade());insert_comma;
            ss+=(it->getopt_p()?"yes":"no");insert_comma;
            ss+="\"";
            for_in(tch, it->get_teacher_binding()){
                ss+=tch->getname();insert_comma;
            }ss+="\"";insert_comma;
        }
#undef insert_comma


        toSky(ss);

    }


}

void talkserver::handle_lesson_case(std::__1::string domain, std::__1::string lid, std::__1::string xstr)
{

    Func(itos, Var i){
        sstream ss;
        ss<<i;
        string s;
        ss>>s;return s;
    };

    Var lsn=defschool::karacter::lb_binding->lesson_assoc(lid);

    if(domain=="ALL_INFORMATION"){

        toSky("lid:"+lsn->getLID()+","+"name:"+lsn->getname()+","+"credit:"+itos(lsn->getcredit())+","+\
                "grade:"+itos(lsn->getgrade())+","+"optp:"+(lsn->getopt_p()?"yes":"no"));

    }

    elif(domain=="STU_TABLE"){
        string str="";
        Var binding=lsn->get_student_binding();
        for_in(stu, binding){
            str+=stu->getSID();str+=",";
            str+=stu->getname();str+=",";
            str+=itos(stu->get_score_of_lesson(lsn->getLID()));str+=",";
        }
        toSky(str);
    }
}

void talkserver::proc_blk_code(std::__1::string blk)
{

    X::X.compile_do_block(blk, true);

}

void talkserver::status_cycle()
{

    Var just_frozen=false;

    while(true){

        if(X::X.frozenp()){
            string cmd="freeze";
            if(not just_frozen)
                yell("freeze");
            just_frozen=true;
        }
        else if(just_frozen)
        {
            string cmd="melt";
            yell("melt");
            just_frozen=false;
        }

        if(ear->hasPendingDatagrams()){
            QByteArray datagram;
            datagram.resize(ear->pendingDatagramSize());
            ear->readDatagram(datagram.data(), datagram.size());
            Var blk=string(datagram.data());


            svec sv;
            X::split_by_ret(blk, sv);
            if(sv[0]!="0"){
                X::cout<<"Received packet: "<<X::endl<<blk<<X::endl;
                set_current_working_seq(sv[0]);
                interpreter(blk);

            }
            else csleep(1);
        }
    }

}

void talkthread::uptate_client_time()
{
    Var_ref mapp=this->clientmap;
    for(Var it=mapp.begin();it!=mapp.end();){
        X::cout<<"Found record: "<<it->first<<" ttl: "<<it->second.getttl()<< X::endl;
        Var_ref the_record=it->second;
        the_record.setttl(the_record.getttl()-1);
        if(the_record.getttl()==0){
            mapp.erase(it);
        }else{it++;}
    }

}
