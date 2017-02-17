
#include "defschool.h"
#include <exception>
using namespace defschool;


/*copy a lesson and store its ptr in the memory.*/
bool lesson_base::add_lesson(lesson l) {
    using std::make_shared;
    lesson_sptr lsp=make_shared<lesson>(lesson(l));
    this->lessons.insert({l.getLID(), lsp});   //Using LID as main keyword.
    return true;
}
/*use the original address of the lesson.*/
bool lesson_base::add_lesson(lesson_sptr lptr) {
    this->lessons.insert({lptr->getLID(), lptr});   //Using LID as main keyword.
    return true;
}

lesson_base::lesson_base(vector<lesson> lv) {
    for_each(lv.begin(), lv.end(), [=](lesson l) {
        this->add_lesson(l);
    });
}

lesson_base::lesson_base(lesson l) {
    this->add_lesson(l);
}

bool lesson_base::delete_lesson(string l) {

    auto it=this->lessons.find(l);

    string st;
    if (this->lessons.end() != it) {
        st=it->second->getname();
        cout<<"found lesson: "<<it->second->getname()<<endl;
        auto students=it->second->get_student_binding();
        auto teachers=it->second->get_teacher_binding();
        //this may be alerting memory errors.
        //if this happen, invent a hard-copy version of get_student_binding and modify the one there.

        for (auto& student : students){
            student->__force_delete_lesson(it->first);
        }

        for (auto& teacher : teachers){
            teacher->delete_my_lesson(it->first);
        }

        this->lessons.erase(it);
        cout<<"deleted lesson: "<<st<<endl;
        return true;
    }



    return false;
}

void lesson_base::get_applicable_lessons(stu_sptr st, vector<lesson_sptr> &vs) {
    for (auto it : this->lessons){
        if(st->lesson_applicable_p(it.second))
            vs.push_back(it.second);
    }
}
void lesson_base::v_applicable_lesson_assoc(std::__1::string v, int cate, stu_sptr st, vector<lesson_sptr> &container){
    vector<lesson_sptr> apl;
    get_applicable_lessons(st, apl);
    for (auto the_lesson : apl){
        auto match_p=the_lesson->v_match_p(v, cate);
        if (match_p){
            container.push_back(the_lesson);
        }

    }
}

lesson_sptr lesson_base::lesson_assoc(string LID){
    auto search= this->lessons.find(LID);
    if (search!=this->lessons.end()) {

        return search->second;
    }
    return nullptr;       /*cowsy: this reference is  invalid.*/
}


/*vague search of the lessons.*/
/*//old version
void lesson_base::v_lesson_assoc(std::string v_lid, vector<lesson_sptr> & container)
{
    std::transform(v_lid.begin(), v_lid.end(), v_lid.begin(), (int (*)(int))std::tolower);
    string pat="\\w*"+v_lid+"\\w*";
    const std::regex pattern(pat);

    for (auto the_pair : this->lessons){

        auto the_lid=the_pair.first;
        auto the_name=the_pair.second->getname();
        std::transform(the_name.begin(), the_name.end(), the_name.begin(), (int (*)(int))std::tolower);

        std::smatch result1;
        std::smatch result2;
        bool match = std::regex_search(the_lid, result1, pattern) or\
                std::regex_search(the_name, result2, pattern);
        if (match){
            container.push_back(the_pair.second);
        }

    }


}
*/



void lesson_base::v_lesson_assoc(std::string v, int cate,  vector<lesson_sptr> & container){

    for (auto the_pair : this->lessons){

        auto match_p=the_pair.second->v_match_p(v, cate);

        if (match_p){
            container.push_back(the_pair.second);
        }

    }

}

void lesson_base::t_show_all_lessons() {
    for (auto pai : this->lessons){
        cout<<"LID: "<<pai.first<<" Name: "<<pai.second->getname()<<" LID: "<<pai.second->getLID()<<" Optional? "<<\
              pai.second->getopt_p()<<endl;
        cout<<"Students: ";
        for(auto str : pai.second->get_student_binding()){
            cout<<str->getname();
        }
        cout<<endl;
    }
}

bool lesson_base::modify_lesson_property(string lid, string slot, string val) {

    bool flag=false;
    auto the_lesson=this->lesson_assoc(lid);
    if (the_lesson!=nullptr){


        if(slot=="name"){
            the_lesson->setname(val);
            flag=true;

        }
        else if (slot=="grade"){
            the_lesson->setgrade(std::stoi(val));
            flag=true;
        }
        else if (slot=="credit"){
            the_lesson->setcredit(std::stoi(val));
            flag=true;
        }

        else if (slot=="optp"){
            if(val=="yes" or val=="y")
                the_lesson->setopt_p(true);
            else
                the_lesson->setopt_p(false);
            flag=true;
        }

    }
    return flag;
}

void lesson_base::clear_all() {
    auto &x = get_lessons();
    for (auto it = x.begin(); it != x.end();) {
        auto old_it = it;
        it++;
        auto the_lid = old_it->first;
        delete_lesson(the_lid);

    }
}

bool student::choose_lesson(string lesson_id) {

    lesson_sptr lsptr= this->assoc_lesson_base(lesson_id);

    if (this->lesson_applicable_p(lsptr))
    {
        auto flag=false;
        for(auto p : this->stu_lesson){
            if(p.first==lesson_id)
                flag=true;
        }//find the lesson. if exsists, return.

        if(not flag){
            this->stu_lesson.push_back(pair<string, int>(lesson_id, 0));

            //shared_ptr<student> sptr(this);
            lsptr->bind_to_student(shared_from_this());

            return true;
        }
        else return false;
    }
    return false;
}



bool student::admin_add_lesson(string lesson_id){
    cout<<"admin: Added lesson "<<lesson_id<<" to "<<this->getname()<<endl;
    lesson_sptr lsptr=this->assoc_lesson_base(lesson_id);
    if (lsptr!=nullptr){
        this->stu_lesson.push_back(pair<string, int>(lesson_id, 0));
        lsptr->bind_to_student(shared_from_this());
        return true;
    }
    return false;
}

void student::choose_lesson(vector<string> lessons) {
    for_each(lessons.begin(), lessons.end(), [=](string l) {
        if (this->choose_lesson(l))
            std::cout<<"Added lesson"<<l<<endl;
        else{
            std::cout<<"Fail to add lesson"<<l<<endl;
        }
    });
}

int student::set_grade(string SID){
    this->grade=std::stoi(SID.substr(0, 4));
    return this->grade;
}


student::student(string name, string SID, string school, string klass) {

    this->setname(name);
    this->setschool(school);
    this->setSID(SID);
    this->setklass(klass);
    this->set_grade(SID);


}

shared_ptr<student> defschool::factory::creates(string name, string TID, string school, string klass)
throw (binding_not_bound_exception)
{
    cout<<"created student "<<name<<TID<<school<<klass<<endl;
    auto s = shared_ptr<student>(new student(name, TID, school, klass));
    if(s->cb_binding==nullptr)
        throw binding_not_bound_exception();

    s->cb_binding->bind_student(s);

    return s;
}

void student::show_my_lessons() {

    for(pair<string, int> lp : this->stu_lesson){
        std::cout<<lp.first<<std::endl;
    }
}

bool student::delete_opt_lesson(string lesson) {

    if (this->assoc_lesson_base(lesson)->getopt_p()){
        for (auto it=stu_lesson.begin();it!=stu_lesson.end();){
            if(it->first==lesson){
                std::cout<<"found lesson: "<<it->first<<std::endl;
                auto the_lesson=karacter::lb_binding->lesson_assoc(it->first);
                the_lesson->unbind_student(shared_from_this());//free that student from this lesson.
                this->stu_lesson.erase(it);

                return true;
            }
            else it++;

        }
    }
    return false;
}
bool student::__set_lesson_score(string ls, int score) throw (negative_score_error)

{

    if(score<0)
        throw negative_score_error();
    for(auto &lsp : this->stu_lesson)
    {if (lsp.first==ls)
        {lsp.second=score;
            return true;}}return false;}

int student::get_GPA() {
    auto GPA=0;
    auto rcount=0;
    auto ocount=0;
    auto rgpa=0;
    auto ogpa=0;

    vector<lesson_sptr> rls;
    this->get_required_lessons(rls);
    vector<lesson_sptr> ols;
    this->get_optional_lessons(ols);
    for (auto lespair: rls){
        rgpa+=lespair->getcredit()*this->get_score_of_lesson(lespair->getLID());
        rcount++;
    }
    if (rcount!=0)
        GPA+=rgpa/rcount;
    for (auto lespair: ols){
        ogpa+=lespair->getcredit()*this->get_score_of_lesson(lespair->getLID())/2;
        ocount++;
    }
    if(ocount!=0)
        GPA+=ogpa/ocount;

    return GPA;
}
string student::get_GPA_s(){
    int gpa=get_GPA();
    std::stringstream ss;
    ss<<gpa;
    string str;
    ss>>str;
    return str;
}

bool student::__force_delete_lesson(string LID) {

    auto it=stu_lesson.begin();
    for(; it!=stu_lesson.end(); it++){
        if(it->first==LID)
            break;
    }
    if (it!=this->stu_lesson.end()){
        auto the_lesson=karacter::lb_binding->lesson_assoc(it->first);
        the_lesson->unbind_student(shared_from_this());//free that student from this lesson.
        this->stu_lesson.erase(it);
        return true;
    }
    else{

        return false;
    }

}


bool student::has_lesson(std::__1::string lid)
{

    for(auto the_pair : this->stu_lesson){
        auto & the_lesson_lid=the_pair.first;
        if(lid==the_lesson_lid){
            return true;
        }
    }
    return false;
}

bool lesson::v_match_p(std::__1::string v, int cate)
{
    std::transform(v.begin(), v.end(), v.begin(), (int (*)(int))std::tolower);
    string pat="\\w*"+v+"\\w*";
    const std::regex pattern(pat);

    enum category{
        lid,
        name,
        grade,
        credit,
        optp
    };

    auto lambda=[&](){
        string x="???";
        switch(cate){
        case lid:
            return this->getLID();
        case name:
            return this->getname();
        case grade:
            return int_to_string(this->getgrade());
        case credit:
            return int_to_string(this->getcredit());
        case  optp:{
            string is;
            if(this->getopt_p())is="yes";
            else is="no";
            return is;
        }

        }
        return x;
    };

    auto abstract_target=lambda();

    std::transform(abstract_target.begin(), abstract_target.end(), abstract_target.begin(), (int (*)(int))std::tolower);

    std::smatch result;

    bool match = std::regex_search(abstract_target , result, pattern);

    return match;
}



/*high_level function for getting lessons.*/


bool teacher::add_to_my_lesson(string lesson_id) {
    if(this->lb_binding== nullptr)
        return false;     /*uninitialized lb_binding.*/
    lesson_sptr lsptr= this->assoc_lesson_base(lesson_id);
    if (lsptr!= nullptr) {
        if(not this->has_lesson(lesson_id)){
            this->m_tlesson.push_back(lesson_id); /* succ*/
            lsptr->bind_to_teacher(shared_from_this());
            return true;
        }
        else {
            return false;
        }
    }
    return false; /*fail*/

}

void teacher::add_to_my_lesson(vector<std::__1::string> &lessons)
{
    for_each(lessons.begin(), lessons.end(), [=](string l) {
        if (this->add_to_my_lesson(l))
            std::cout<<"Added lesson"<<l<<endl;
        else{
            std::cout<<"Fail to add lesson"<<l<<endl;
        }
    });
}

bool teacher::delete_my_lesson(std::__1::string lid)
{
    for (auto it=m_tlesson.begin();it!=m_tlesson.end();){
        if(*it==lid){
            auto the_lesson=karacter::lb_binding->lesson_assoc(lid);
            the_lesson->unbind_teacher(shared_from_this());//free that student from this lesson.
            this->m_tlesson.erase(it);

            return true;
        }
        else it++;

    }
    return false;
}

teacher::teacher(string name, string TID, string school) {
    this->setname(name);
    this->setTID(TID);
    this->setschool(school);

}

bool teacher::has_lesson(std::__1::string lid)
{
    for(auto id : this->m_tlesson){
        if(id==lid){
            return true;
        }
    }
    return false;
}

bool teacher::v_match_p(std::__1::string v, int cate)
{
    std::transform(v.begin(), v.end(), v.begin(), (int (*)(int))std::tolower);
    string pat="\\w*"+v+"\\w*";
    const std::regex pattern(pat);

    enum category{
        tid,
        name,
        school,
        lessons,
    };


    auto lambda=[&](){
        string x="???";
        switch(cate){
        case tid:
            return this->getTID();
        case name:
            return this->getname();
        case school:
            return this->getschool();

        }
        return x;
    };

    auto abstract_target=lambda();

    std::transform(abstract_target.begin(), abstract_target.end(), abstract_target.begin(), (int (*)(int))std::tolower);

    std::smatch result;

    bool match = std::regex_search(abstract_target , result, pattern);

    return match;

}

shared_ptr<teacher> defschool::factory::createt(string name, string TID, string school)
throw (binding_not_bound_exception)
{
    cout<<"created teacher "<<name<<TID<<school<<endl;
    auto t=make_shared<teacher>(name, TID, school);
    if(t->cb_binding==nullptr)throw binding_not_bound_exception();
    t->cb_binding->bind_teacher(t);
    return t;
}

void factory::createl(string name, string LID, string grade, string credit, string optp) {
    cout<<"created lesson "<<name<<LID<<grade<<credit<<optp<<endl;
    karacter::lb_binding->add_lesson(lesson(LID, name, credit, grade, optp));
}


void teacher::get_my_lessons(vector<lesson_sptr> & c) {
    for (const string lsid : this->m_tlesson){
        lesson_sptr lsptr=assoc_lesson_base(lsid);
        if(lsptr!=nullptr){
            c.push_back(lsptr);
        }
    }
}

void teacher::show_my_students() {
    for (const string lsid : this->m_tlesson){
        lesson_sptr lsptr=assoc_lesson_base(lsid);
        cout<<"Lesson name:   "<<lsptr->getname()<<endl;
        for (auto stu : lsptr->get_student_binding()){
            cout<<"Name:   "<<stu->getname()<<"ID:   "<<stu->getSID()<<endl;
        }
    }
}

void teacher::get_students_of_lesson(string LID, vector<stu_sptr> & vs ) {
    auto les=this->assoc_lesson_base(LID);
    for (auto stu : les->get_student_binding()){
        vs.push_back(stu);
    }
}


lesson::lesson(string ID, string name, int credit, int grade, bool opt_p) {
    this->m_credit = credit;
    this->m_LID = ID;
    this->m_name = name;
    this->m_opt_p=opt_p;
    this->m_grade=grade;
}

bool lesson::bind_to_student(stu_sptr sptr) {
    if (find(this->students_binding.begin(), this->students_binding.end(), sptr)==this->students_binding.end()){
        this->students_binding.push_back(sptr);
        return true;
    }
    return false;
}


bool lesson::bind_to_teacher(tec_sptr tptr) {
    if(find(this->teachers_binding.begin(), this->teachers_binding.end(), tptr)==this->teachers_binding.end()){
        this->teachers_binding.push_back(tptr);
        return true;
    }
    return false;
}

void lesson::unbind_student(lesson::stu_sptr sptr)
{

    //unbind that student.

    auto & bind=this->students_binding;

    for(auto it=bind.begin();it!=bind.end();it++){
        if(*it==sptr){
            this->students_binding.erase(it);
            break;//why this needs a break?
        }
    }

}

void lesson::unbind_teacher(lesson::tec_sptr tptr)
{
    auto & bind=this->teachers_binding;

    for(auto it=bind.begin();it!=bind.end();it++){
        if(*it==tptr){
            this->teachers_binding.erase(it);
            break;//why this needs a break?
        }
    }
}

void lesson::t_show_all_students()
{
    auto & binding=this->get_student_binding();
    for(auto stu : binding){
        cout<<stu->getname()<<endl;
    }
}

lesson::lesson(string ID, string name, string credit, string grade, string opt_p) {

    this->m_credit = std::stoi(credit);
    this->m_LID = ID;
    this->m_name = name;
    this->m_opt_p=opt_p=="yes";
    this->m_grade=std::stoi(grade);
}


bool student::v_match_p(std::__1::string v, int cate)
{
    std::transform(v.begin(), v.end(), v.begin(), (int (*)(int))std::tolower);
    string pat="\\w*"+v+"\\w*";
    const std::regex pattern(pat);

    enum category{
        sid=0,
        name=1,
        school=2,
        grade=3,
        klass=4
    };


    auto lambda=[&](){
        string x="???";
        switch(cate){
        case sid:
            return this->getSID();
        case name:
            return this->getname();
        case school:
            return this->getschool();
        case grade:
            return this->getgrade_s();
        case klass:{
            return this->getklass();
        }

        }
        return x;
    };

    auto abstract_target=lambda();

    std::transform(abstract_target.begin(), abstract_target.end(), abstract_target.begin(), (int (*)(int))std::tolower);

    std::smatch result;

    bool match = std::regex_search(abstract_target , result, pattern);

    return match;
}



student::student(string &&name, string &&SID, string &&school, string &&klass) {
    this->setname(name);
    this->setschool(school);
    this->setSID(SID);
    this->setklass(klass);
    this->set_grade(SID);

}


bool character_base::students_binding_lookup(string SID) {
    return (this->students_binding.find(SID)!=students_binding.end());
}
bool character_base::teachers_binding_lookup(string TID){
    return (this->teachers_binding.find(TID)!=teachers_binding.end());
}

bool character_base::bind_student(stu_sptr stusptr) {
    if (not students_binding_lookup(stusptr->getSID())){
        this->students_binding.insert({stusptr->getSID(), stusptr});
        return true;
    }
    return false;
}

bool character_base::bind_teacher(tec_sptr tecsptr) {

    if (not teachers_binding_lookup(tecsptr->getTID())){
        this->teachers_binding.insert({tecsptr->getTID(), tecsptr});
        return true;
    }
    return false;
}

bool character_base::remove_student(std::__1::string sid)
{

    auto & the_binding=this->get_students_binding();
    auto it=the_binding.find(sid);
    if (it!=the_binding.end()){

        auto the_stu=it->second;
        for(auto the_pair : the_stu->get_stu_lesson()){

            auto the_lesson=the_stu->assoc_lesson_base(the_pair.first);
            if(the_lesson!=nullptr)
            the_lesson->unbind_student(the_stu);

        }

//above: remove the binding between stu and lesson.
        the_binding.erase(it);
        cout<<"Removed: "<<sid<<endl;
        return true;

    }
    return false;
}

bool character_base::remove_teacher(std::string tid)
{
    auto & the_binding=this->get_teachers_binding();
    auto it=the_binding.find(tid);
    if (it!=the_binding.end()){
        auto the_tch=it->second;
        for(auto the_lesson_id : the_tch->gettlesson()){
            auto the_lesson=the_tch->assoc_lesson_base(the_lesson_id);
            if(the_lesson!=nullptr)
            the_lesson->unbind_teacher(the_tch);
        }
//above: remove the binding between tch and lesson.
        the_binding.erase(it);
        cout<<"Removed: "<<tid<<endl;
        return true;
    }
    return false;
}

bool character_base::modify_student_property(string SID, string slot, string val)
{

    bool flag=false;
    auto the_it=this->get_students_binding().find(SID);
    if (the_it!=this->get_students_binding().end()){
        auto & the_student=the_it->second;

        if(slot=="name"){
            the_student->setname(val);
            flag=true;

        }
        else if (slot=="school"){
            the_student->setschool(val);
            flag=true;
        }
        else if (slot=="grade"){
            the_student->set_grade(val);
            flag=true;
        }
        else if (slot=="class"){
            the_student->setklass(val);
            flag=true;
        }

    }
    return flag;

}

bool character_base::modify_teacher_property(std::__1::string TID, std::__1::string slot, std::__1::string val)
{
    bool flag=false;
    auto the_it=this->get_teachers_binding().find(TID);
    if (the_it!=this->get_teachers_binding().end()){
        auto & the_teacher=the_it->second;

        if(slot=="name"){
            the_teacher->setname(val);
            flag=true;

        }
        else if (slot=="school"){
            the_teacher->setschool(val);
            flag=true;
        }


    }
    return flag;
}

character_base::stu_sptr character_base::get_student_of_sid(std::string sid)
{
    auto it=this->get_students_binding().find(sid);
    if (it!=this->get_students_binding().end())
        return it->second;
    return nullptr;
}

character_base::tec_sptr character_base::get_teacher_of_tid(std::__1::string tid)
{
    auto it=this->get_teachers_binding().find(tid);
    if(it!=this->get_teachers_binding().end())
        return it->second;
    return nullptr;
}

/*for defining static slot.*/
shared_ptr<lesson_base> karacter::lb_binding=nullptr;

shared_ptr<character_base> karacter::cb_binding=nullptr;





template<typename c, typename f>
void character_base::__v_character_assoc(std::__1::string v, int cate, c & container, f & slot)
{
    for(auto the_pair : slot){
        auto match_p=the_pair.second->v_match_p(v, cate);
        if(match_p){
            container.push_back(the_pair.second);
        }
    }
}

void character_base::v_student_assoc(std::__1::string v, int cate, vector<stu_sptr> &container)
{
    this->__v_character_assoc(v, cate, container, this->get_students_binding());
}

void character_base::v_teacher_assoc(std::__1::string v, int cate, vector<character_base::tec_sptr> &container)
{
    this->__v_character_assoc(v, cate, container, this->get_teachers_binding());
}

void character_base::clear_teachers() {
    auto &x = this->teachers_binding;
    for (auto it = x.begin(); it != x.end();) {
        auto old_it = it;
        it++;
        auto the_tid = old_it->first;
        remove_teacher(the_tid);

    }
}

void character_base::clear_students() {
    auto &x = this->students_binding;
    for (auto it = x.begin(); it != x.end();) {
        auto old_it = it;
        it++;
        auto the_sid = old_it->first;
        remove_student(the_sid);

    }
}
bool karacter::v_match_p(std::__1::string v, int cate)
{
    v=v+v;
    cate=2*cate;
    return false;
}
