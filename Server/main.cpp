//#define __enigma

#ifndef __enigma
#include <QApplication>
#include "stu_manage.h"
#include "tch_manage.h"
#include "atqlass_main.h"
#include "timemachine_main.h"
#endif
#include <iostream>
#include "defschool.h"
#include "account.h"
#include"common_animation.h"
#include "kompiler.h"
#include <ctime>
#include "talkserver.h"
#include <QThread>
//#define __no_compiler
#ifndef __no_compiler   //compiler exsist.
#include "TIME_MACHINE.h"

#endif


using namespace defschool;

int main(int argc, char *argv[])
{


    X::X.init_capsule();
    adb_binding->reg_new_user("2014", "111", ADMIN_PTY);
    //adb_binding->reg_new_user("1001", "12345", TEC_PTY);
    //adb_binding->reg_new_user("2014333555", "12345",STU_PTY);
    //adb_binding->reg_new_user("2014211971", "111", STU_PTY);


    std::set_unexpected([](){cout<<"Unexpected Caught!"<<endl;exit(1);});
#ifdef __no_compiler
    adb_binding->reg_new_user("2014", "111", ADMIN_PTY);
        adb_binding->reg_new_user("2014333555", "12345", STU_PTY);
        adb_binding->reg_new_user("2014222222", "11111", STU_PTY);
        adb_binding->reg_new_user("1000", "12345", TEC_PTY);
        adb_binding->reg_new_user("1001", "12345", TEC_PTY);
        //adb_binding->reg_new_user("1002", "12345", TEC_PTY);
        adb_binding->reg_new_user("2013155155", "12345", STU_PTY);
        adb_binding->reg_new_user("2014211111", "12345", STU_PTY);

        lesson ai0("10100", "Introduction to Computer Science", 2, 2014, true);
        lesson ai1("10101", "Artificial Inteligence", 4, 2013, true);
        lesson ai2("10102", "Introduction to Art", 2, 2013, true);
        lesson ai4("10105", "Advanced Mathematics", 3, 2014, false);
        lesson ai3("10103", "Linear Mathematics", 4, 2015, true);
        lesson ai5("10106", "Advanced Physics", 4, 2014, true);
        lesson ai6("10107", "Machine Learning", 4, 2014, true);
        lesson ai7("10108", "Animal Inteligence", 4, 2014, true);
        lesson ai8("10109", "Statistics", 4, 2014, true);
        lesson ai9("10110", "Introduction to Computer System", 4, 2014, true);
        lesson ai10("10111", "Logic Route", 6, 2015, false);
        lesson ai11("10112", "Java the Language", 2, 2014, false);
        lesson ai12("10113", "Introduction to Lambda Calculus", 10, 2014, false);
        lesson ai13("10114", "Functional Programming", 4, 2014, false);
        lesson ai14("10115", "Lisp Programming Language", 10, 2014, false);
        lesson ai15("10116", "Design Pattern", 10, 2014, true);
        vector<lesson> ls={ai0,ai1,ai2,ai3,ai4,ai5,ai6,ai7,ai8,ai9, ai10,
                          ai11, ai12, ai13, ai14, ai15};
      auto lb_binding=shared_ptr<lesson_base>(new lesson_base(ls));
#endif

#ifndef __no_compiler
    auto lb_binding=shared_ptr<lesson_base>(new lesson_base());
    karacter::bind_lesson_base(lb_binding);
    auto cb_binding=shared_ptr<character_base>(new character_base());
    karacter::bind_character_base(cb_binding);
#endif

    try{
#ifdef __no_compiler
        auto niu=factory::creates("Jack Robins", "2014333555", "CS", "319");
        factory::creates("Tom Lancer", "2013155155", "CS", "214");
        factory::creates("Dijkstra Hacker", "2014211111", "CS", "101");
        factory::creates("Martin Looser",  "2015222222", "CS", "211");
        factory::creates("Xach Finiz", "2014111231", "CS", "222");
        factory::creates("Choke Smith" , "2013133554", "CS", "101");
        factory::creates("Boost Standard", "2014221411", "CS", "211");

        niu->choose_lesson("10100");
        niu->choose_lesson("10106");
        niu->choose_lesson("10107");
        niu->choose_lesson("10108");
        niu->choose_lesson("10102");
        niu->choose_lesson("10103");

        //then load teacher.
        auto tea=factory::createt("Jack Smith", "1000", "CS");
        auto tea2=factory::createt("Tommy Muk", "1001","CS");
        auto tea3=factory::createt("Kophor Sushi", "1002", "CS");
        factory::createt("Helanz", "2300", "CS");
        tea->add_to_my_lesson("10101");
        tea->add_to_my_lesson("10100");
        tea->add_to_my_lesson("10110");
#endif


#ifndef __no_compiler
        //X::X=shared_ptr<X::TIME_MACHINE>(new X::TIME_MACHINE());

        X::X.load_and_compile();
        auto xtime=time(NULL);
        X::X.travel_to_time(xtime);
        cout<<"current time: "<<xtime<<endl;
        X::X.melt();

#endif

    }
    catch(binding_not_bound_exception &be){
        std::cout<<be.what()<<std::endl;
    }




#ifndef __enigma
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QApplication a(argc, argv);
        atqlass_main w;
        w.show();


        talkthread tt;
        tt.start();


    return a.exec();
#endif
}
