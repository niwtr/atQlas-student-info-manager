#include "defschool.h"

#include <iostream>
#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_
namespace defschool{


constexpr auto ADMIN_PTY=777;
constexpr auto STU_PTY=000;
constexpr auto TEC_PTY=022;
constexpr auto UNKNOWN_PTY=-1;

auto translate_pty=[](auto ptys){
    if(ptys=="STU_PTY"){
        return STU_PTY;
    }
    else if(ptys=="TCH_PTY"){
        return TEC_PTY;
    }
    else if(ptys=="ADMIN_PTY"){
        return ADMIN_PTY;
    }
    else return UNKNOWN_PTY;
};



struct atquser_property{
    string id;
    string passwd;
    int priority=UNKNOWN_PTY;
};

using aqp=atquser_property;

class atquser;

class account_base{

private:

    map<string, aqp> adb_binding;
public:
    inline auto & get_adb_binding(){return this->adb_binding;}
    auto account_verify(string id, string pass);
    shared_ptr<atquser> manage_login(string id, string pass);
    bool reg_new_user(string id, string passwd, int priority);
    stu_sptr bind_to_student(string sid) noexcept;
    tec_sptr bind_to_teacher(string tid) noexcept;
    bool adb_lookup(string id); //check if the user exsists.

    friend inline void operator<<(std::ostream & ofs, account_base base){
        auto binding=base.get_adb_binding();
        for (auto  the_pair : binding){
            auto user=the_pair.second;
            ofs<<"[user]\n";
            ofs<<"username:"<<user.id<<","<<endl;
            ofs<<"passwd:"<<user.passwd<<","<<endl;
            switch(user.priority){
            case ADMIN_PTY:
                ofs<<"pty:"<<"ADMIN_PTY"<<endl;break;
            case STU_PTY:
                ofs<<"pty:"<<"STU_PTY"<<endl;break;
            case TEC_PTY:
                ofs<<"pty:"<<"TCH_PTY"<<endl;break;
            }
             ofs<<"[\\user]\n";

        }


    }



};



class atquser {

    __PBUILDERF(int, priority, private)

    __PBUILDERF(string, name, private)

    __PBUILDERF(string, _passwd, private)

    public:
        inline atquser(string sid, string _passwd, int pri) {
        this->setname(sid);
        this->set_passwd(_passwd);
        this->setpriority(pri);
    }
};
extern shared_ptr<account_base> adb_binding;
}

#endif
