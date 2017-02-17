#include "TIME_MACHINE.h"
#include <QFileDialog>
#include <functional>
#include "account.h"
#include "algorithm"
#define __XOMPILER__


using namespace X;
using symb=std::string;
using symbv=vector<symb>;
//add v2's elements to v1.


using closurev=vector<closure>;
using tclosurev=vector<time_piece>;
using timefrag=pair<time_t, tclosurev>;
using symbv=vector<symb>;
vector<timefrag> TIME_MACHINE::time_capsule=vector<timefrag>();

auto merge_v=[](auto & v1, auto v2){
    std::for_each(v2.begin(), v2.end(), [&](auto elem){v1.push_back(elem);});
};

auto ignore=[](){};




//by default, the arg blk is well lifted from the block.

auto stu_compile_in_braced = [](auto & __add_lesson_fns, symb line)
{
    symbv linev;
    split_by_colon(line, linev);
    auto the_lesson_id=linev[0];
    auto  the_lesson_score=std::stoi(linev[1]);
    __add_lesson_fns.push_back(
            [the_lesson_id, the_lesson_score](stu_sptr stu) {
                stu->admin_add_lesson(the_lesson_id);
                stu->__set_lesson_score(the_lesson_id, the_lesson_score);
            });
};
auto compile_in_student_label=[](auto __this, auto v){
    using closure=std::function<stu_sptr ()>;
    using closurev=vector<std::function<void (stu_sptr)>>;
    closure __creates_fn;
    closurev __add_lesson_fns;
    string sid = "", name = "", school = "", klass = "";  //receive the val.
    bool go_into_braced=false; //to see wether we are in a inner lexical .
    for (auto word : v){
        if(go_into_braced){ //go into inner lexical block.
            symbv bsv;
            filter_return(word, false);
            filter_space (word); //we can freely filter the space and return out.
            split_by_comma(word, bsv);
            //now the inner lexical block is seperated into lines.
            for (auto line : bsv){
               // filter_return(line, false);
                //filter_space(line);
                if(__this->dh.well_sperated_by_colonp(line))
                stu_compile_in_braced(__add_lesson_fns, line);
            }
            go_into_braced=false;
        }
        else  //not in a brace.h
        if (__this->dh.well_sperated_by_colonp(word)) {
            symbv cv;
            filter_return(word, false);
            //do not filter space.
            split_by_colon(word, cv);

            symb the_slot = cv[0];
            string the_val = cv[1];//we should not filter the space inner the val, cause the name may contain a space.
            filter_space(the_slot);
            carefully_filter_space(the_val);
            auto the_slot_num = __this->assoc_property(the_slot, "student");
            switch (the_slot_num) {
                case 0: {//sid
                    sid = the_val;
                }break;
                case 1: {
                    name = the_val;
                }break;
                case 2: {
                    school = the_val;
                }break;
                case 3: {
                    klass = the_val;
                }break;
                case 4: {
                    go_into_braced=true;
                }break;
                default:
                    ignore();
            }

        }
        else{

                __this->output<<"the property is not perfectly seperated by colon."<<endl;
            }
        }
    bool slot_all_set= sid != "" and name != "" and school != "" and klass != "";
    if(slot_all_set) {
        __creates_fn = [name, sid, school, klass] {
            return factory::creates(name, sid, school, klass);
        };
        std::function<void()> res=[=]()mutable{
            auto stu=__creates_fn();
            for(auto cls : __add_lesson_fns){
                cls(stu);
            }
        };
        return res;
    }
    else {
        //alert that incomplete definition found.
        std::function<void()> error_res=[=]()mutable{};
        return error_res;
    }
};
//student domain ends here.



auto tch_compile_in_braced = [](auto & __add_lesson_fns, symb line)
{
    auto the_lesson_id=line;
    __add_lesson_fns.push_back(
            [the_lesson_id](tec_sptr tec) {
                tec->add_to_my_lesson(the_lesson_id);
            });
};
auto compile_in_teacher_label=[](auto __this, auto v){
    using closure=std::function<tec_sptr ()>;
    using closurev=vector<std::function<void (tec_sptr)>>;
    closure __createt_fn;
    closurev __add_lesson_fns;
    string tid = "", name = "", school = "";  //receive the val.
    bool go_into_braced=false; //to se wether we are in a inner lexical .
    for (auto word : v){
        if(go_into_braced){ //go into inner lexical block.
            symbv bsv;
            filter_return(word, false);
            filter_space (word); //we can freely filter the space and return out.
            split_by_comma(word, bsv);
            //now the inner lexical block is seperated into lines.
            for (auto line : bsv){//no need to check wether is seperated by colon.
                    tch_compile_in_braced(__add_lesson_fns, line);
            }
            go_into_braced=false;
        }
        else  //not in a brace.
        if (__this->dh.well_sperated_by_colonp(word)) {
            symbv cv;
            filter_return(word, false);
            //do not filter space.
            split_by_colon(word, cv);

            symb the_slot = cv[0];
            string the_val = cv[1];//we should not filter the space inner the val, cause the name may contain a space.
            filter_space(the_slot);
            carefully_filter_space(the_val);
            auto the_slot_num = __this->assoc_property(the_slot, "teacher");
            switch (the_slot_num) {
                case 0: {//sid
                    tid = the_val;
                }break;
                case 1: {
                    name = the_val;
                }break;
                case 2: {
                    school = the_val;
                }break;
                case 3: {
                    go_into_braced=true;
                }break;
                default:
                    ignore();
            }

        }
        else{
           __this->output<<"the property is not perfectly seperated by colon."<<endl;
        }
    }
    bool slot_all_set= tid != "" and name != "" and school != "";
    if(slot_all_set) {
        __createt_fn = [name, tid, school] {
            return factory::createt(name, tid, school);
        };
        std::function<void()> res=[=]()mutable{
            auto tch=__createt_fn();
            for(auto cls : __add_lesson_fns){
                cls(tch);
            }
        };
        return res;
    }
    else {
        //alert that incomplete definition found.
        std::function<void()> error_res=[=]()mutable{};
        return error_res;
    }
};
//teacher domain ends here.




auto compile_in_lesson_label=[](auto __this, auto v){

    using closure=std::function<void ()>;
    closure __createt_fn;
    string lid = "", name = "", optp = "",grade="", credit="";  //receive the val.
    for (auto word : v){
        //for lesson there is no "brace".
        if (__this->dh.well_sperated_by_colonp(word)) {
            symbv cv;
            filter_return(word, false);
            //do not filter space.
            split_by_colon(word, cv);
            symb the_slot = cv[0];
            string the_val = cv[1];//we should not filter the space inner the val, cause the name may contain a space.
            filter_space(the_slot);
            carefully_filter_space(the_val);

            auto the_slot_num = __this->assoc_property(the_slot, "lesson");

            switch (the_slot_num) {
                case 0: {//sid
                    lid = the_val;
                }break;
                case 1: {
                    name = the_val;
                }break;
                case 2: {
                    grade = the_val;
                }break;
                case 3: {
                    credit=the_val;
                }break;
                case 4:{
                    if (the_val[0]=='y' or the_val[0]=='Y')
                    optp="yes";
                    else optp="no";
                }
                default:
                    ignore();
            }

        }
        else{
            __this->output<<"the property is not perfectly seperated by colon."<<endl;
        }
    }

    bool slot_all_set= lid != "" and name != "" and grade != "" and credit!="" and optp!="";
    if(slot_all_set) {
        __createt_fn = [name, lid, grade, credit, optp] {
            factory::createl(name, lid, grade, credit, optp);
        };
        closure res=[=]()mutable{__createt_fn();};

        return res;
    }
    else {
        //alert that incomplete definition found.
        std::function<void()> error_res=[=]()mutable{};
        return error_res;
    }
};
//student domain ends here.

auto compile_in_user_label=[](auto __this, auto v){
    using closure=std::function<void ()>;
    closure __create_fn;
    string username = "", passwd="";  //receive the val.
    auto pty=UNKNOWN_PTY;
    for (auto word : v){
        //for lesson there is no "brace".
        if (__this->dh.well_sperated_by_colonp(word)) {
            symbv cv;
            filter_return(word, false);
            //do not filter space.
            split_by_colon(word, cv);
            symb the_slot = cv[0];
            string the_val = cv[1];//we should not filter the space inner the val, cause the name may contain a space.
            filter_space(the_slot);
            carefully_filter_space(the_val);

            if(the_slot=="username"){
                username=the_val;
            }
            else if(the_slot=="passwd"){
                passwd=the_val;
            }
            else if(the_slot=="pty"){
                pty=translate_pty(the_val);
            }
            else{
                ; //alert.
            }

        }
        else{
            __this->output<<"the property is not perfectly seperated by colon."<<endl;
        }
    }

    bool slot_all_set=  username!= "" and passwd != "" and pty !=UNKNOWN_PTY ;
    if(slot_all_set) {
        __create_fn = [username, passwd, pty] {
            //factory::createl(name, lid, grade, credit, optp);
            adb_binding->reg_new_user(username, passwd, pty);
        };
        closure res=[=]()mutable{__create_fn();};

        return res;
    }
    else {
        //alert that incomplete definition found.
        std::function<void()> error_res=[=]()mutable{};
        return error_res;
    }
};


void ::X::lift_out_braced_exps(symb &blk, symbv &v){
    string current_str="";
    bool in_brace=false;
    for(auto it=blk.begin();it!=blk.end();){

        if(*it=='{'){
            blk.erase(it);
            in_brace=true;
        }
        else if(*it=='}'){
            blk.erase(it);
            in_brace=false;
            v.push_back(current_str);
            current_str.clear();
        }
        if(in_brace){
            current_str=current_str+*it;
            blk.erase(it);
        }
        else {
            if (it != blk.end())
                it++;
        }

    }

}

//compile definition block
//the definition block must be run once compiled. you know, if we do not do that way, we are doomed
//to encounter error.
//now, we should mark the definition closure to not be runned.
void TIME_MACHINE::compile_definition_block(symb blk, string domain, bool instantp) {
    output<<"Xompiler: Compiling definition block of a "<<domain<<"."<<endl;
    auto original_block=blk;

    symbv v;

    symbv braced_buffer;
    lift_out_braced_exps(blk, braced_buffer);

    split_by_comma(blk, v);
    merge_v(v, braced_buffer);

    //the block is now seperated by comma.
    closure def;
    if(domain=="student") {
         def=compile_in_student_label(this, v);
        //def(); //now we need not to run at compilation time.
        //this->do_closure.push_back(def);

    }
    else if(domain=="teacher"){
         def=compile_in_teacher_label(this, v);
        //def();
        //this->do_closure.push_back(def);

    }
    else if(domain=="lesson"){
        def=compile_in_lesson_label(this, v);
        //def();
        //this->do_closure.push_back(def);
    }

    else if (domain=="user"){
        def=compile_in_user_label(this, v);
    }

    if(instantp)
        def();
    save_current_time(time_piece{original_block, def, "define-"+domain});
    output<<"Xompiler: Compile finished."<<endl;
}



enum do_syntax{
    at_first_word,
    at_domain_pos,
    at_target_pos,
    at_targets_pos,
    at_candy_pos,
    at_do_pos,
    error_syntax,
    complete
};
enum context{
    as_for,
    _for,
    nil_context
};
enum domain{
    stud,
    teac,
    less,
    nil_domain
};

auto wordp=[](auto wd, auto supposed){
    return wd==supposed;
};
//firstword.
auto forp=[](auto wd){
    return wordp(wd, "for");
};
auto asforp=[](auto wd){
    return wordp(wd, "as-for");
};
//domain
auto studentp=[](auto wd){
    return (wordp(wd, "student") or wordp(wd, "students"));
};
auto teacherp=[](auto wd){
    return (wordp(wd,"teacher") or wordp(wd, "teachers"));
};
auto lessonp=[](auto wd){
    return (wordp(wd, "lesson") or wordp(wd, "lessons"));
};

//candy


auto thatp=[](auto wd){
    return (wordp(wd, "that"));
};

//dos
auto add_lessonp=[](auto wd){
    return (wordp(wd, "add-lesson"));
};
auto add_lesson_bangp=[](auto wd){
    return (wordp(wd, "add-lesson!"));
};

auto delete_lessonp=[](auto wd){
    return (wordp(wd, "delete-lesson"));
};
auto delete_lesson_bangp=[](auto wd){
    return (wordp(wd, "delete-lesson!"));
};

auto deletep=[](auto wd){
    return (wordp(wd, "delete"));
};

auto modify_propertyp=[](auto wd){
    return (wordp(wd, "modify-property"));
};
auto set_score_bangp=[](auto wd){
    return (wordp(wd, "set-score!"));
};
auto clear_lesson_basep=[](auto wd){
    return (wordp(wd, "clear-lesson-base"));
};

auto clear_character_basep=[](auto wd){
    return (wordp(wd, "clear-character-base"));
};
auto compile_in_do=[](auto __this, auto & lex, auto & sublex){
    using closure=std::function<void()>;
//    using closurev=vector<closure>;
    auto filtdeco=[](auto& wd){
        filter_space(wd);
        filter_return(wd, false);
    };
    //pop one word.
    auto bring_one=[&](auto & lexi){
        symb x="null";
        if(lexi.empty())return x;
        auto res=lexi.at(0);
        lexi.erase(lexi.begin());
        return res;
    };
    auto bring_one_word=[&](){
       return bring_one(lex);
    };
    auto bring_one_sublex=[&](){
        return bring_one(sublex);
    };


    std::function <vector<stu_sptr>()> __stus_op;
    std::function <vector<tec_sptr>()> __tchs_op;
    std::function <vector<lesson_sptr>()> __less_op;

    closure __X__;

    int status=at_first_word;
    int context=nil_context;
    int domain=nil_domain;
    while(status!=complete){
        auto wd= bring_one_word();
        filtdeco(wd);//remove the decorates.
        switch (status){

            case at_first_word:{
              /*  if(not __this->chkvalid_in_g(wd)){
                    status=error_syntax;
                    throw lead_word_syntax_error(wd);
                    break;
                }*/
                if(forp(wd)){
                    context=_for;
                    status=at_domain_pos;
                    }
                else if (asforp(wd)){
                    context=as_for;
                    status=at_domain_pos;
                }
                else if (clear_lesson_basep(wd)) {
                    __X__ = []() {
                        karacter::lb_binding->clear_all();
                    };
                    status = complete;
                }
                else if (clear_character_basep(wd)){
                    __X__=[]() {
                        karacter::cb_binding->clear_students();
                        karacter::cb_binding->clear_teachers();
                    };
                    status=complete;
                }
                else {
                    status=error_syntax;
                    throw lead_word_syntax_error(wd);
                }

                }break;

            case at_domain_pos:{
                if(not __this->chkvalid_in_g(wd)){
                    status=error_syntax;
                    throw domain_syntax_error(wd);

                }
                if(studentp(wd)){
                    domain=stud;
                }
                else if (teacherp(wd)){
                    domain=teac;
                }
                else if (lessonp(wd)){
                    domain=less;
                }


                if(context==as_for)
                status=at_target_pos;  //to candy pos.
                else if(context==_for)
                    status=at_candy_pos;

            }break;

            case at_candy_pos:{
                if(context==_for){
                    wd=bring_one_word();
                    if(thatp(wd)){;}
                    else {
                        ;
                    }
                    //the "satisfy command is niled.
                    status=at_targets_pos;
                }


            }break;

            case at_target_pos:{
                //handle the case for unique target.
             if(domain==stud){
                 auto stuid=wd; //assert that current word is the stuid.
                 __stus_op=[=](){return vector<stu_sptr> {karacter::cb_binding->get_student_of_sid(stuid)};};
             }
             else if(domain==teac){
                 auto tchid=wd;
                 __tchs_op=[=](){return vector<tec_sptr >{karacter::cb_binding->get_teacher_of_tid(tchid)};};
             }
             else if(domain==less){
                 auto lid=wd;
                 __less_op=[=](){return vector<lesson_sptr>{karacter::lb_binding->lesson_assoc(lid)};};
             }

             bring_one_word(); //we just assert the word"do" exsists here.
             status=at_do_pos;

            }break;

            case at_do_pos:{
                auto xstring=bring_one_sublex();//this should let xparser to handle that.
                carefully_filter_space(xstring);

                auto lesson_operation_on_character=[&](auto target, auto operate_method){
                    bring_one_word(); //eat that.
                    std::function<void()> lambda;
                    lambda=[=](){
                        auto x=xstring;
                        vector<lesson_sptr> vl;
                        xparser::parse_and_assoc_base(x, vl);
                        auto v=target();
                        for(auto stu : v) {
                            if(stu!=nullptr)
                            for (auto les : vl) {
                                operate_method(stu, les->getLID());
                            }

                        }
                    };
                    __X__=lambda;
                };



                switch (domain){

                    case stud:{

                        if(add_lessonp(wd)){
                            lesson_operation_on_character(__stus_op,\
                             [](auto stu, auto id) {stu->choose_lesson(id);});
                        }
                        else if(add_lesson_bangp(wd)){

                            lesson_operation_on_character(__stus_op,\
                            [](auto stu, auto id){stu->admin_add_lesson(id);});
                        }
                        else if(delete_lessonp(wd)){
                            lesson_operation_on_character(__stus_op,\
                            [](auto stu, auto id){stu->delete_opt_lesson(id);});
                        }
                        else if(delete_lesson_bangp(wd)){
                            lesson_operation_on_character(__stus_op,\
                            [](auto stu, auto id){stu->__force_delete_lesson(id);});
                        }
                            //half abstraction.
                        else if(deletep(wd)){
                            bring_one_word(); //eat that.
                            std::function<void()> lambda;
                            lambda=[=](){
                                auto v=__stus_op();
                                for(auto stu : v) {
                                    if(stu!=nullptr)
                                    karacter::cb_binding->remove_student(stu->getSID());
                                }
                            };
                            __X__=lambda;
                        }
                            //cowsay: half abstraction.
                        else if (modify_propertyp(wd)){

                            auto slot=bring_one_word();
                            auto val=bring_one_word();
                            while(!lex.empty()){
                                auto next=bring_one_word();
                                filtdeco(next);
                                val=val+" "+next;

                            }  //potentially that the property is a name and conatains space.
                            std::function<void()> lambda;
                            lambda=[=]() {
                                auto v=__stus_op();
                                for (auto stu : v) {
                                    if(stu!= nullptr)
                                    karacter::cb_binding->modify_student_property(stu->getSID(), slot, val);

                                }
                            };
                            __X__=lambda;
                        }

                        else if (set_score_bangp(wd)){

                            auto the_lesson_id=bring_one_word();
                            auto the_score=bring_one_word();

                            functor lambda;
                            lambda=[=](){
                                auto v=__stus_op();
                                for(auto stu : v){
                                    if (stu!=nullptr)
                                    stu->__set_lesson_score(the_lesson_id, std::stoi(the_score));
                                }
                            };
                            __X__=lambda;
                        }

                        else {
                            throw operator_syntax_error(wd);
                        }

                    }break;
                    case teac:{

                        if(add_lessonp(wd)){
                            lesson_operation_on_character(__tchs_op,\
                             [](auto tch, auto id) {tch->add_to_my_lesson(id);});
                        }

                        else if(delete_lessonp(wd)){
                            lesson_operation_on_character(__tchs_op,\
                            [](auto tch, auto id){tch->delete_my_lesson(id);});
                        }
                        else if(deletep(wd)){
                            //no that to eat.
                            std::function<void()> lambda;
                            lambda=[=](){
                                auto v=__tchs_op();
                                for(auto tch: v) {
                                    if(tch!=nullptr)
                                    karacter::cb_binding->remove_teacher(tch->getTID());
                                }
                            };
                            __X__=lambda;
                        }

                        else if (modify_propertyp(wd)){

                            auto slot=bring_one_word();
                            auto val=bring_one_word();
                            while(!lex.empty()){
                                auto next=bring_one_word();
                                filtdeco(next);
                                val=val+" "+next;

                            }  //potentially that the property is a name and conatains space.
                            std::function<void()> lambda;
                            lambda=[=](){
                                auto v=__tchs_op();
                                for(auto tch : v) {
                                    if(tch!=nullptr)
                                    karacter::cb_binding->modify_teacher_property(tch->getTID(), slot, val);
                                }
                            };
                            __X__=lambda;
                        }
                        else{
                            throw operator_syntax_error(wd);
                        }

                    }break;
                    case less:{
                        if (modify_propertyp(wd)){

                            auto slot=bring_one_word();
                            auto val=bring_one_word();
                            while(!lex.empty()){
                                auto next=bring_one_word();
                                filtdeco(next);
                                val=val+" "+next;

                            }  //potentially that the property is a name and conatains space.
                            std::function<void()> lambda;
                            lambda=[=](){
                                auto v=__less_op();
                                for(auto les : v) {
                                    if(les!=nullptr)
                                    karacter::lb_binding->modify_lesson_property(les->getLID(), slot, val);
                                }
                            };
                            __X__=lambda;
                        }

                        else if(deletep(wd)){
                            std::function<void()> lambda;
                            lambda=[=](){
                                auto v=__less_op();
                                for(auto les: v) {
                                    if(les!=nullptr)
                                    karacter::lb_binding->delete_lesson(les->getLID());
                                }
                            };
                            __X__=lambda;
                        }

                        else {
                            throw operator_syntax_error(wd);
                        }

                    }break;
                }

                status=complete;
            }break;

            case at_targets_pos:{
                switch (domain){
                    case stud:{
                        auto xstring=bring_one_sublex();//this should let xparser to handle that.
                        carefully_filter_space(xstring);
                        auto squeezer=[=] (){
                            vector<stu_sptr> vs;
                            xparser::parse_and_assoc_base(xstring, vs);
                            return vs;
                        };
                        __stus_op=squeezer;

                        //currently the current word must be do.
                        status=at_do_pos;
                    }break;
                    case teac:{
                        auto xstring=bring_one_sublex();//this should let xparser to handle that.
                        carefully_filter_space(xstring);
                        __tchs_op=[=](){
                            vector <tec_sptr > vs;
                            xparser::parse_and_assoc_base(xstring, vs);
                            return vs;
                        };

                        //currently the current word must be do.
                        status=at_do_pos;
                    }break;
                    case less:{
                        auto xstring=bring_one_sublex();//this should let xparser to handle that.
                        carefully_filter_space(xstring);
                        __less_op=[=](){
                            vector <lesson_sptr > vs;
                            xparser::parse_and_assoc_base(xstring, vs);
                            return vs;
                        };
                        //currently the current word must be do.
                        status=at_do_pos;
                    }break;
                }
            }


        }




    }
return __X__;
};

void TIME_MACHINE::compile_do_block(symb blk, bool instantp) {
    output<<"Xompiler: Compiling do block."<<blk<<endl;
    auto original_block=blk;
    symbv lex;
    symbv sublex;
    filter_return(blk, true);
    filter_dspace(blk);
    lift_out_braced_exps(blk, sublex);  //extract the braced exps into sublex vector.
    split_by_space(blk, lex);
    //no merge_v.
    try {
        auto __do_closure=compile_in_do(this, lex, sublex);
        if(instantp)
            __do_closure();
        //tagged_closure tc={original_block, compile_in_do(this, lex, sublex)};
        this->save_current_time(time_piece{original_block, __do_closure, "do-block"});
    }
    catch(syntax_error & s){
        output<<"Xompiler: "<<s.describe()<<endl;
        output<<s.word_hint()<<endl;
        this->error_num+=1;
    }
    output<<"Xcompiler: Compile finished."<<endl;
}

void TIME_MACHINE::insert_time_piece_behind(long fragment_time, time_piece tp, long seq)
{
    for(auto & time_frag : time_capsule){
        if(time_frag.first==fragment_time){
            auto & container=time_frag.second;
            for(auto it=container.begin();it!=container.end();it++){
                if(it->get_seq()==seq){
                    container.insert(it, tp);break;
                }
            }
        }
    }

}

void ::X::filter_return(symb &block, bool replace_by_spacep) {
    for (auto it=block.begin();it!=block.end();){
        if(*it=='\n'){

            if(replace_by_spacep){
                *it=' ';
                it++;
            }
            else block.erase(it); //no it++;
        }

        else
            if(it!=block.end())
            it++;
    }
}

void ::X::filter_space(symb &block) {
    for (auto it=block.begin();it!=block.end();){
        if(*it==' ')
            block.erase(it); //no it++;

        else
            if(it!=block.end())
            it++;
    }
}
void ::X::filter_dspace(symb &block) {

    for (auto it=block.begin();it!=block.end();){

        if(it!=block.end() and *it==' ') {
            if((it+1)!=block.end() and *(it+1)==' ')
                block.erase(it); //no it++;
            else it++;
        }
        else
        if(it!=block.end())
            it++;
    }


}
void ::X::carefully_filter_space(symb &block) {
    //bool in_val=false;
    string alpha="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (auto it=block.begin(); it!=block.end();){
        bool isletter=alpha.find(*it)!=std::string::npos;
        if(isletter)break;
        //not a letter.
        if(*it==' '){
            block.erase(it);
        }
        else
        if(it!=block.end())
            it++;
    }
    for (auto it=block.end(); it!=block.begin();){

        bool isletter=alpha.find(*it)!=std::string::npos;
        if(isletter)break;
        //not a letter.
        if(*it==' '){
            block.erase(it);
        }
        else
        if(it!=block.begin())
            it--;
    }

}

void ::X::filter_comma(symb &block, bool replace_by_spacep) {

    for (auto it=block.begin();it!=block.end();){
        if(*it==','){
            if(replace_by_spacep){
                *it=' ';
                it++;
            }
            else block.erase(it); //no it++;
        }

        else
        if(it!=block.end())
            it++;
    }
}


void ::X::TIME_MACHINE::load_and_compile(){
    vector<string> vs;
    std::ifstream ifs;
    ifs.open("./storage.x");
    if(not ifs.is_open()){
        output<<"cannot open file"<<endl;
        output<<"Creating new file: storage.x"<<endl;

        std::ofstream ofs;
        ofs.open("./storage.x");
        if(not ofs.is_open()){
            output<<"Cannot create file: storage.x"<<endl;
            exit(1);
        }
        return ;
    }
    //string str;
    string acc;
    bool in_block=false;
    string current_block="";
    bool packed=false;
    char s[256];
    bool listening=true;  //wether we should receive a block label.
    while(!ifs.eof()){
        ifs.getline(s,256);
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

        if(listening) {
            if (str == "[student]") {
                open_block("student");
                continue;
            }
            else if (str == "[teacher]") {
                open_block("teacher");
                continue;
            }
            else if (str == "[lesson]") {
                open_block("lesson");
                continue;
            }
            else if(str=="[user]"){
                open_block("user");
                continue;
            }

            else if (str == "[do]") {
                open_block("do");
                continue;
            }
            else if (str == "[comment]") {
                open_block("comment");
                listening=false; //refuse to receive a new block.
                continue;
            }
        }
        if(str=="[\\student]"){
            close_block("student");
        }
            else if(str=="[\\teacher]"){
            close_block("teacher");
        }
            else if(str=="[\\lesson]"){
            close_block("lesson");
        }
            else if(str=="[\\user]"){
            close_block("user");
        }
            else if(str=="[\\do]"){
            close_block("do");
        }
            else if(str=="[\\comment]"){
            close_block("comment");
            listening=true;
        }

        if(in_block){
            acc.append(str);
        }
        if(packed){
            if(current_block=="do")
                compile_do_block(acc, false);
            else if(current_block=="comment")
                ;//nothing to do for comment.
            else
                compile_definition_block(acc, current_block, false);

            packed=false;
            acc.clear();
        }
    }

    if(not acc.empty()) {
        if(current_block!="do")
            compile_definition_block(acc, current_block, false);
        else compile_do_block(acc, false);
    }
    ifs.close();

    output<<"Compilation tasks finished with "<<this->error_num<<" errors."<<endl;
    this->error_num=0; //resume it.
}

void TIME_MACHINE::dump_to_xfile() {
    std::ofstream ofs;
    ofs.open("./storage.x");
    if(not ofs.is_open()){
        cout<<"cannot open file"<<endl;
        return;
    }

    ofs<<(*adb_binding);
    ofs<<*(karacter::lb_binding);
    ofs<<*(karacter::cb_binding);

    ofs.close();

}

/*
    for(auto & the_pair : karacter::lb_binding->get_lessons()){
       ofs<<*(the_pair.second);
    }

    for(auto the_pair : karacter::cb_binding->get_students_binding()){
        ofs<<*(the_pair.second);
    }

    for(auto the_pair : karacter::cb_binding->get_teachers_binding()){
        ofs<<*(the_pair.second);
    }
    */
void TIME_MACHINE::sort_capsule()
{
    std::sort(time_capsule.begin(), time_capsule.end(), [](auto x, auto y){
        return x.first<y.first;
    });

}

bool TIME_MACHINE::time_frag_existsp(long curtime) {
    for (auto the_pair : this->time_capsule){
        if(the_pair.first==curtime){
            return true;
        }
    }
    return false;
}

void TIME_MACHINE::save_current_time(time_piece cap) {
    this->update_time();
    long time_ago=this->global_time;
    auto stdtime=std::localtime(&time_ago);
    output<<"TIMEMACHINE: Saving the world of time: "<<"day "<<stdtime->tm_mday<<"hour "<<stdtime->tm_hour<<"min "<<stdtime->tm_min\
       <<"second "<<stdtime->tm_sec<<endl;

         // time_ago<<endl;
    if(time_frag_existsp(time_ago)){
        for(auto & the_pair : this->time_capsule){
            if(the_pair.first==time_ago){
                the_pair.second.push_back(cap);//push the piece to vector.
            }
        }
    }
    else {

        auto it=time_capsule.begin();
        while(it!=time_capsule.end() and it->first<time_ago)it++;
        tclosurev tc={cap};
        timefrag tf={time_ago, tc};
        //this->time_capsule.push_back(tf);
        this->time_capsule.insert(it, tf);
        //insert the current fragment to the suitable position

    }


}


void TIME_MACHINE::travel_to_time(time_t curtime) {
    defschool::karacter::lb_binding->clear_all();
    defschool::karacter::cb_binding->clear_students();
    defschool::karacter::cb_binding->clear_teachers();

    global_time=curtime;//set the global time to the time you want to travel to.

    sort_capsule();



    output<<"TIME MACHINE: Traveling to time: "<<curtime<<endl;
    for (auto the_time_frag : this->time_capsule){
        if(the_time_frag.first<=curtime){
            output<<"TIME MACHINE: Found time fragment "<<the_time_frag.first<<endl;
            auto the_timev=the_time_frag.second;
            for(auto piece : the_timev){
                output<<"evolving piece "<<piece.get_seq()<<endl;
                piece();
                output<<"TIME MACHINE: Successfully recovered time piece."<<endl;
            }
        }

    }
    output<<"TIME MACHINE: Moriturus te saluto."<<endl;

    freeze();
}

long TIME_MACHINE::get_capsule_size()
{

    return this->time_capsule.size();
}

TIME_MACHINE::timefrag &TIME_MACHINE::get_fragment_by_index(int index){

   if(index>get_capsule_size())
    {
          return time_capsule[0];
    }
    else{
        return time_capsule.at(index);
   }

}

TIME_MACHINE::timefrag &TIME_MACHINE::get_fragment_by_time(long time)
{

    sort_capsule();

    for(auto & frag : this->time_capsule){
        if(frag.first==time)
            return frag;
    }
    timefrag ttf={time, {}};
    time_capsule.push_back(ttf);
    return time_capsule.back();
}


bool inline symb_decorate_handler::well_sperated_by_colonp(symb word) {
    int pos=0;
    for (auto it=word.begin();it!=word.end();it++){
        //at least contain a simgle letter.
        if(pos>0 and it!=word.end()) {
            if (*it==':')return true;
        }
        pos++;
    }

    return false;
}

long X::time_piece::global_seq=0;
long X::TIME_MACHINE::global_time=time(NULL);
bool X::TIME_MACHINE::frozen=false;
std::stringstream X::TIME_MACHINE::output;

void TIME_MACHINE::delete_time_fragment(long fragment_time) {
    for (auto it=time_capsule.begin();it!=time_capsule.end();it++){
        if(it->first==fragment_time){
            time_capsule.erase(it);break;
        }
    }
}


void TIME_MACHINE::delete_time_piece(long seq){
    for(auto it=time_capsule.begin();it!=time_capsule.end();it++){
        auto& container=it->second;
        for(auto it=container.begin();it!=container.end();it++){
            if(it->get_seq()==seq){
                container.erase(it);
                return;
            }
        }
    }
}
void TIME_MACHINE::instant_stream_compile(std::__1::stringstream &ss, bool runp)
{

    string acc;
    bool in_block=false;
    string current_block="";
    bool packed=false;
    char s[256];
    bool listening=true;  //wether we should receive a block label.
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

        if(listening) {
            if (str == "[student]") {
                open_block("student");
                continue;
            }
            else if (str == "[teacher]") {
                open_block("teacher");
                continue;
            }
            else if (str == "[lesson]") {
                open_block("lesson");
                continue;
            }
            else if(str=="[user]"){
                open_block("user");
                continue;
            }

            else if (str == "[do]") {
                open_block("do");
                continue;
            }
            else if (str == "[comment]") {
                open_block("comment");
                listening=false; //refuse to receive a new block.
                continue;
            }
        }
        if(str=="[\\student]"){
            close_block("student");
        }
            else if(str=="[\\teacher]"){
            close_block("teacher");
        }
            else if(str=="[\\lesson]"){
            close_block("lesson");
        }
            else if(str=="[\\user]"){
            close_block("user");
        }
            else if(str=="[\\do]"){
            close_block("do");
        }
            else if(str=="[\\comment]"){
            close_block("comment");
            listening=true;
        }

        if(in_block){
            acc.append(str);
        }
        if(packed){
            if(current_block=="do")
                compile_do_block(acc, runp);
            else if(current_block=="comment")
                ;//nothing to do for comment.
            else
                compile_definition_block(acc, current_block, runp);

            packed=false;
            acc.clear();
        }
    }

    if(not acc.empty()) {
        if(current_block!="do")
            compile_definition_block(acc, current_block, runp);
        else compile_do_block(acc, runp);
    }

    output<<"Compilation tasks finished with "<<this->error_num<<" errors."<<endl;
    this->error_num=0; //resume it.
}
