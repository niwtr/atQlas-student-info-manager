#include "account.h"
using namespace defschool;


auto defschool::account_base::account_verify(std::string id, std::string pass) {
    auto & binding=this->get_adb_binding();
    for (auto account_pair : binding){
        auto the_account=account_pair.second;
        auto the_name=the_account.id;
        auto the_passwd=the_account.passwd;

        auto the_priority=the_account.priority;
        if(the_name==id){

            if (the_passwd==pass){
                return std::make_pair("correct", the_priority);

            }
            else
            {
                return std::make_pair("error_passwd", UNKNOWN_PTY);
            }
        }
    }
    return std::make_pair("no_such_name", UNKNOWN_PTY);
}

shared_ptr<atquser> account_base::manage_login(string id, string pass) {
    string code; int priority;
    std::tie(code, priority)=this->account_verify(id, pass);
    if (code=="correct"){
        return shared_ptr<atquser>(new atquser(id, pass, priority));
    }
    else if (code=="error_passwd"){
        cout<<"passwd error."<<endl;
         return shared_ptr<atquser>(new atquser("false", "false", UNKNOWN_PTY));
    }
    else{
        cout<<"passwd error."<<endl;
        return shared_ptr<atquser>(new atquser("false", "false", UNKNOWN_PTY));
    }
}

bool account_base::reg_new_user(string id, string passwd, int priority) {
    cout<<"reg new user "<<id<<" "<<passwd<<endl;
    aqp newu;
    newu.id=id;
    newu.passwd=passwd;
    newu.priority=priority;
    this->adb_binding.insert({id, newu});
    return true;
}

stu_sptr account_base::bind_to_student(std::__1::string sid) noexcept
{
    return defschool::karacter::cb_binding->get_student_of_sid(sid);
}
tec_sptr account_base::bind_to_teacher(std::string tid) noexcept
{
    return defschool::karacter::cb_binding->get_teacher_of_tid(tid);
}

bool account_base::adb_lookup(std::__1::string id)
{
    if(this->adb_binding.find(id)==adb_binding.end())
        return false;
    return true;
}


shared_ptr<account_base> defschool::adb_binding=shared_ptr<account_base> (new account_base());




