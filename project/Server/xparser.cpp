#include"xparser.h"

using namespace xparser;
void filter_space(string &block) {

    for (auto it=block.begin();it!=block.end();){
        if(*it==' ') {
            block.erase(it); //no it++;
        }
        else {
            if (it != block.end())
                it++;
        }
    }
}
void filter_tab(string &block) {

    for (auto it=block.begin();it!=block.end();){
        if(*it=='\t') {
            block.erase(it); //no it++;
        }
        else {
            if (it != block.end())
                it++;
        }
    }
}
void xparser::parse_and_assoc_base(string s, vector<lesson_sptr> &container){

    enum __category{
        lid,
        name,
        grade,
        credit,
        optp
    };
    using std::shared_ptr;
    auto lb_bind=karacter::lb_binding;
    vector<string> sv;
    filter_space(s);
    filter_tab(s);
    splitf(s,",", sv);//store all of the commands in the sv box.
    /*split the message into pieces.
     * ex: g:4 nam: niu ->
     * g:4
     * nam: niu
     * */

    bool is_first_time=true;  /*if that val turned into false, turn the logic to "and", aka, using filter.*/

    for(auto str : sv){   //for each piece of messages.
        auto clean_str=to_lower(str);//erase_space(str));

        cout<<"XParser: working on str "<<clean_str<<endl;
        vector<string> cate_and_info;
        splitf(clean_str, ":", cate_and_info);  //split the command and messange. ex: lid:student ->lid  student
        auto category=cate_and_info[0]; //get the category. ready to parse.
        if(cate_and_info.size()==1){

            return; //the input is not complete.

        }
        auto info=cate_and_info[1];   //get that info.
        auto align_cate=category.substr(0,1);
        if(align_cate=="l"){
            is_first_time?lb_bind->v_lesson_assoc(info, lid, container):filter(info, lid, container);
        }
        else if(align_cate=="n"){
            is_first_time?lb_bind->v_lesson_assoc(info, name, container):filter(info, name, container);
        }
        else if(align_cate=="g")
        {
            is_first_time?lb_bind->v_lesson_assoc(info, grade, container):filter(info, grade, container);
        }
        else if(align_cate=="c")
        {
            is_first_time?lb_bind->v_lesson_assoc(info, credit, container): filter(info, credit, container);
        }
        else if(align_cate=="o")
        {
            is_first_time?lb_bind->v_lesson_assoc(info, optp, container): filter(info, optp, container);
        }
        else
        {
            ;

        }
        is_first_time=false;
    }

}



void xparser::filter(string info, int cate, vector<lesson_sptr> &container){
    enum __category{
        lid,
        name,
        grade,
        credit,
        optp
    };
    cout<<"before filter: "<<container.size()<<endl;
    bool flag;
    for (auto the_lesson_it=container.begin();the_lesson_it!=container.end(); ){

        flag=false;

        switch (cate) {
        case lid:{
            if ((*the_lesson_it)->v_match_p(info, lid))
                flag=true;
            break;}
        case name:{
            if((*the_lesson_it)->v_match_p(info, name))
                flag=true;
            break;
        }
        case grade:{
            if((*the_lesson_it)->v_match_p(info, grade))
                flag=true;
            break;
        }
        case credit:{
            if((*the_lesson_it)->v_match_p(info, credit))
                flag=true;
            break;
        }
        case optp:{
            if((*the_lesson_it)->v_match_p(info, optp)) //cowsay: potential bug. "yes" doesn't match "y"
                flag=true;
            break;
        }
        default:
            break;
        }

        if(not flag){
            container.erase(the_lesson_it);
        }
        else
            the_lesson_it++;
    }
    cout<<"after filter: "<<container.size()<<endl;

}

void xparser::parse_and_assoc_base(string s, vector<stu_sptr> &container)
{
    enum __category{
        sid=0,
        name=1,
        school=2,
        grade=3,
        klass=4
    };
    using std::shared_ptr;
    auto cb_bind=karacter::cb_binding;
    vector<string> sv;

    filter_space(s);
    filter_tab(s);
     splitf(s,",", sv);//store all of the commands in the sv box.
    /*split the message into pieces.
     * ex: g:4 nam: niu ->
     * g:4
     * nam: niu
     * */

    bool is_first_time=true;  /*if that val turned into false, turn the logic to "and", aka, using filter.*/

    for(auto str : sv){   //for each piece of messages.
        auto clean_str=to_lower(str);//erase_space(str));
        cout<<"XParser: working on str "<<clean_str<<endl;
        vector<string> cate_and_info;
        splitf(clean_str, ":", cate_and_info);  //split the command and messange. ex: lid:student ->lid  student
        auto category=cate_and_info[0]; //get the category. ready to parse.`
        if(cate_and_info.size()==1){

            return; //the input is not complete.

        }
        auto info=cate_and_info[1];   //get that info.
        auto align_cate=category.substr(0,1);
        if(align_cate=="i"){
            is_first_time?cb_bind->v_student_assoc(info, sid, container):filter(info, sid,  container);
        }
        else if(align_cate=="n"){
            is_first_time?cb_bind->v_student_assoc(info, name, container):filter(info, name, container);
        }
        else if(align_cate=="s")
        {
            is_first_time?cb_bind->v_student_assoc(info, school, container):filter(info, school, container);
        }
        else if(align_cate=="c")
        {
            is_first_time?cb_bind->v_student_assoc(info, klass, container): filter(info, klass, container);
        }
        else if(align_cate=="g")
        {
            is_first_time?cb_bind->v_student_assoc(info, grade, container): filter(info, grade, container);
        }
        else //no specific cmd is found, find name and lid.
        {
            ;

        }
        is_first_time=false;
    }
}

void xparser::filter(string info, int cate, vector<stu_sptr> &container){
    enum __category{
        sid=0,
        name=1,
        school=2,
        grade=3,
        klass=4
    };
    //cout<<"before filter: "<<container.size()<<endl;
    bool flag;
    for (auto the_stu_it=container.begin();the_stu_it!=container.end(); ){
        flag=false;

        switch (cate) {
        case sid:{
            if ((*the_stu_it)->v_match_p(info, sid))
                flag=true;
            break;}
        case name:{
            if((*the_stu_it)->v_match_p(info, name))
                flag=true;
            break;
        }
        case klass:{
            if((*the_stu_it)->v_match_p(info, klass))
                flag=true;
            break;
        }
        case grade:{
            if((*the_stu_it)->v_match_p(info, grade))
                flag=true;
            break;
        }
        case school:{
            if((*the_stu_it)->v_match_p(info, school)) //cowsay: potential bug. "yes" doesn't match "y"
                flag=true;
            break;
        }
        default:
            break;
        }

        if(not flag){
            container.erase(the_stu_it);
        }
        else
            the_stu_it++;
    }
    //cout<<"after filter: "<<container.size()<<endl;

}

void xparser::parse_and_assoc_base(string s, vector<tec_sptr> &container)
{
    enum __category{
        tid,
        name,
        school,
        lessons,
    };
    using std::shared_ptr;
    auto cb_bind=karacter::cb_binding;
    vector<string> sv;
    filter_space(s);
    filter_tab(s);
     splitf(s,",", sv);//store all of the commands in the sv box.
    /*split the message into pieces.
     * ex: g:4 nam: niu ->
     * g:4
     * nam: niu
     * */

    bool is_first_time=true;  /*if that val turned into false, turn the logic to "and", aka, using filter.*/

    for(auto str : sv){   //for each piece of messages.
        auto clean_str=to_lower(str);//erase_space(str));
        cout<<"parser: working on str "<<clean_str<<endl;
        vector<string> cate_and_info;
        splitf(clean_str, ":", cate_and_info);  //split the command and messange. ex: lid:student ->lid  student
        auto category=cate_and_info[0]; //get the category. ready to parse.`
        if(cate_and_info.size()==1){

            return; //the input is not complete.

        }
        auto info=cate_and_info[1];   //get that info.
        auto align_cate=category.substr(0,1);
        if(align_cate=="i"){
            is_first_time?cb_bind->v_teacher_assoc(info, tid, container):filter(info, tid, container);
        }
        else if(align_cate=="n"){
            is_first_time?cb_bind->v_teacher_assoc(info, name, container):filter(info, name, container);
        }
        else if(align_cate=="s")
        {
            is_first_time?cb_bind->v_teacher_assoc(info, school, container):filter(info, school, container);
        }

        else //no specific cmd is found, find name and lid.
        {
            ;

        }
        is_first_time=false;
    }
}


void xparser::filter(string info, int cate, vector<tec_sptr> &container){
    enum __category{
        tid,
        name,
        school,
        lessons,
    };
    //cout<<"before filter: "<<container.size()<<endl;
    bool flag;
    for (auto the_tch_it=container.begin();the_tch_it!=container.end(); ){

        flag=false;

        switch (cate) {
        case tid:{
            if ((*the_tch_it)->v_match_p(info, tid))
                flag=true;
            break;}
        case name:{
            if((*the_tch_it)->v_match_p(info, name))
                flag=true;
            break;
        }
        case school:{
            if((*the_tch_it)->v_match_p(info, school)) //cowsay: potential bug. "yes" doesn't match "y"
                flag=true;
            break;
        }
        default:
            break;
        }

        if(not flag){
            container.erase(the_tch_it);
        }
        else
            the_tch_it++;
    }

}



void xparser::parse_and_assoc_applicable_lesson(string s, stu_sptr stu, vector<lesson_sptr> &container)
{
    enum __category{
        lid,
        name,
        grade,
        credit,
        optp
    };
    using std::shared_ptr;
    auto lb_bind=karacter::lb_binding;
    vector<string> sv;
    filter_space(s);
    filter_tab(s);
     splitf(s,",", sv);//store all of the commands in the sv box.

    /*split the message into pieces.
     * ex: g:4 nam: niu ->
     * g:4
     * nam: niu
     * */

    bool is_first_time=true;  /*if that val turned into false, turn the logic to "and", aka, using filter.*/

    for(auto str : sv){   //for each piece of messages.
        auto clean_str=to_lower(str);//erase_space(str));
        cout<<"XParser: working on str "<<clean_str<<endl;
        vector<string> cate_and_info;
        splitf(clean_str, ":", cate_and_info);  //split the command and messange. ex: lid:student ->lid  student
        auto category=cate_and_info[0]; //get the category. ready to parse.
        if(cate_and_info.size()==1){

            return; //the input is not complete.

        }
        auto info=cate_and_info[1];   //get that info.
        auto align_cate=category.substr(0,1);
        if(align_cate=="l"){
            is_first_time?lb_bind->v_applicable_lesson_assoc(info, lid, stu, container):filter(info, lid, container);
        }
        else if(align_cate=="n"){
            is_first_time?lb_bind->v_applicable_lesson_assoc(info, name, stu, container):filter(info, name, container);
        }
        else if(align_cate=="g")
        {
            is_first_time?lb_bind->v_applicable_lesson_assoc(info, grade, stu, container):filter(info, grade, container);
        }
        else if(align_cate=="c")
        {
            is_first_time?lb_bind->v_applicable_lesson_assoc(info, credit, stu, container): filter(info, credit, container);
        }
        else if(align_cate=="o")
        {
            is_first_time?lb_bind->v_applicable_lesson_assoc(info, optp,stu,  container):filter(info, optp, container);
        }
        else
        {
            ;

        }
        is_first_time=false;
    }

}

