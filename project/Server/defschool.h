#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <regex>
#include <algorithm>
#include <exception>
#ifndef _DEFSCHOOL_H_
#define _DEFSCHOOL_H_

/*property builder.
 * Auto-generate properties with getters and setters.
 * happy hacking.*/

#define __PBUILDERF(type, name, access_permission)\
access_permission:\
type m_##name;\
public:\
inline void set##name(type v) {\
m_##name = v;\
}\
inline type get##name() {\
return m_##name;\
}\


#define SID_LENGTH 10   /*ex.20142116718*/


/*The namespace for the core of atQlas.*/
/* the functions starts with __ is never used explicitly.*/

namespace defschool {
    using std::tuple;
    using std::string;
    using std::vector;
    using std::map;
    using std::pair;
    using std::cout;
    using std::endl;
    //shared_ptr


    using std::shared_ptr;
    class lesson;
    using lesson_sptr=shared_ptr<lesson>;    /*...*/
    using std::make_shared;


    auto int_to_string=[](int i){
        std::stringstream ss;
        ss<<i;
        std::string str;
        ss>>str;
        return str;
    };

/********************************class lesson starts here.**************************************************/
    class student;/*declaration of class student*/
    class teacher;/*declaration of class teacher*/
    class lesson : public std::enable_shared_from_this<lesson>{
    using stu_sptr=shared_ptr<student>;
    using tec_sptr=shared_ptr<teacher>;

    __PBUILDERF(string, LID, private)    /*The ID of this lesson*/
    __PBUILDERF(string, name, private)   /*The name of this lesson*/
    __PBUILDERF(int, credit, private)    /*The credit of this lesson*/
    __PBUILDERF(int, grade, private)     /*Note : not the score of a student, indeed the grade the lesson is in.*/
    __PBUILDERF(bool, opt_p, private)    /*Is this lesson optional?*/


    private:
        vector<stu_sptr> students_binding;   /*contain the pointer of students who chose this lesson.*/
        vector<tec_sptr> teachers_binding;   /*contain the pointer of teachers who teach this lesson.*/
    public:
        /*constructor.*/
        /* it gonna consume this five properties*/
        lesson(string ID, string name, int credit, int grade, bool opt_p);
        lesson(string ID, string name, string credit, string grade, string opt_p);
        /*To init an empty class.
         * Only used for generating error messages.*/

        inline lesson(){}

        /*Copy constructor.
         * Well this should be used somewhere, say, the initialization of the lesson_base
         * This is should below. The lesson_base consumes a lesson and create a copy of that one
         * And bind that shared_pointer to its binding.
         * The original one passed to the add_lesson function is then ignored. Let it go.*/
        inline lesson(const lesson& l){
            m_name=l.m_name;
            m_LID=l.m_LID;
            m_credit=l.m_credit;
            m_opt_p=l.m_opt_p;
            m_grade=l.m_grade;

        }

        /*Add a binding to a specific student.
         * This should be never explicitly called by the user, say, you.
         * Automatically called in when the function *choose_lesson* of student class is called.
         * Which will bind the student to this lesson.*/
        bool bind_to_student(stu_sptr sptr);
        /*See bind_to_student above.*/
        bool bind_to_teacher(tec_sptr tptr);

        void unbind_student(stu_sptr sptr);
        void unbind_teacher(tec_sptr tptr);
        /*Well just bring that private out of class. Nothing to say.*/
        inline auto& get_student_binding(){
            return this->students_binding;}
        inline auto& get_teacher_binding(){
            return this->teachers_binding;}

        void t_show_all_students();

        bool v_match_p(string v, int cate);


        friend inline void operator<<(std::ostream& ofs, lesson the_lesson){
            ofs<<"[lesson]\n";
            ofs<<"lid:"<<the_lesson.getLID()<<","<<endl;
            ofs<<"name:"<<the_lesson.getname()<<","<<endl;
            ofs<<"credit:"<<the_lesson.getcredit()<<","<<endl;
            ofs<<"grade:"<<the_lesson.getgrade()<<","<<endl;
            if(the_lesson.getopt_p())
                ofs<<"optp:yes"<<endl;
            else
                ofs<<"optp:no"<<endl;
            ofs<<"[\\lesson]\n";
        }


    };
/********************************class lesson ends here.****************************************************/

/********************************class lesson_base starts here.*********************************************/
    class student;

/*serves as the asylum for the lessons.*/
    class lesson_base {
       using stu_sptr=shared_ptr<student>;
    public:

        /*fetch that property out. see *lessons**/
        map<string, lesson_sptr>& get_lessons(){return this->lessons;}

        /*Constructor.
         * This will check each lessons of that vector, create a new copy of that vector and bind that to
         * the base. That is to say, the original allocated lesson is forgotten.
         * So never should you new a lesson in the heap. We are not in charge of the safety of your memory
         * anyway. Good luck :)*/
        lesson_base(vector<lesson> lv);

        /*The constructor consuming a unique lesson. The lesson passed is not used, too.
         * see the other constructor above.*/
        lesson_base(lesson l);

        /*generate an empty lesson base, nothing should be added.*/
        lesson_base(){}

        /*add a new lesson to this lesson base.
         * This would work on a new copy of that lesson l, not on the lesson itself.
         * Please take care of the memory of that lesson l. Anyway you should give it up.*/
        bool add_lesson(lesson l);

        /*This one, instead, will just bind that pointer to the lesson to our base.
         * Would not copy anything, just add the pointer.
         * So learn to watch the pointer after calling this one.
         * If you freed that pointer and therefore caused the broken of that binding, the
         * nightmere will take you.*/
        bool add_lesson(lesson_sptr lptr);

        /* assoc our lesson_base using that ID of the lesson.
         * Will return a shared_ptr to that lesson.
         * We will take care of the memory for you. Relax.*/
        lesson_sptr lesson_assoc(string LID);


        void v_lesson_assoc(std::string v, int cate, vector<lesson_sptr> &container);
        void v_applicable_lesson_assoc(std::string v, int cate, stu_sptr st, vector<lesson_sptr> & container);
        /*
         *
         *  the lesson using SID
         * Currently would cause a problem that though the lesson was taken out of lesson_base,
         * it is not removed from the class list of a student or a teacher.
         * cowsay: to be fixed.
         * cowsay: was not tested.*/
        bool delete_lesson(string SID);

        /*get the IDs of applicable lessons of a student.*
         * The result is put in the vector passed to it.*/
        void get_applicable_lessons(stu_sptr st, vector<lesson_sptr> &vs);

        bool modify_lesson_property(string lid, string slot, string val);

        void clear_all();

        void t_show_all_lessons();


        friend inline void operator<<(std::ostream & ofs, lesson_base lb){
            for(auto & the_pair : lb.get_lessons()){
               ofs<<*(the_pair.second);
            }
        }

    private:
        map<string, lesson_sptr> lessons;   /*ID->lesson_ptr binding pairs of lessons.*/
    };
/********************************class lesson_base ends here.***********************************************/






/********************************class karacter starts here.************************************************/



    class lesson;
    class lesson_base;
    class character_base;
    /*the character. derived: student, teacher.*/
    class karacter {

    __PBUILDERF(string, name, private)   /*the name of that character.*/
    __PBUILDERF(string, school, private) /*the school of that character.*/

    protected:
       //static shared_ptr<lesson_base> lb_binding;   /*binding to that lesson_base.*/

    public:

        static shared_ptr<lesson_base> lb_binding;   /*binding to that lesson_base.*/
        static inline void bind_lesson_base(shared_ptr<lesson_base> lb){lb_binding=lb;}

        static shared_ptr<character_base> cb_binding;
        static inline void bind_character_base(shared_ptr<character_base> cb){cb_binding=cb;}
        virtual bool v_match_p(std::__1::string v, int cate);

    };

    class binding_not_bound_exception:public std::exception{
    public:
        const char * what() const throw(){
            const char * the_flaw_binding;
            if(karacter::lb_binding==nullptr){
                the_flaw_binding="lb_binding";
            }else{
                the_flaw_binding="cb_binding";
            }
            return the_flaw_binding;
        }

    };


/********************************class karacter ends here.**************************************************/

/********************************class student starts here.*************************************************/
    class negative_score_error:public std::exception{
    public:
        virtual const char * what() const throw(){
            return "Error: negative score of lesson!";
        }

    };
    class student : public karacter ,public std::enable_shared_from_this<student>{  //2016/4/30

    __PBUILDERF(string, SID, private)   /*the student id.*/
    __PBUILDERF(string, klass, private) /*the class the student is in .*/


    private:

        int grade;   /*the grade the student is in.*/
        /*well, the string->int pair marks the score of the
         * lessons this student possess. this sounds a little trivial but it just works.*/
        //这是学生的课程的唯一容器.
        vector<pair<string, int>> stu_lesson;

        /* have to write here, in order to use the type deduction.*/
        /*get the lessons, I say, the real pointer of the lessons, not the IDs, that the student posses.*/
        /*Takes a pred function and push the results that correspond to the pred to a vector. */
        template <typename Func>
        auto __get_lessons(Func fn) {
            vector <lesson_sptr> vl;
            for (pair<string, int> lsp : this->stu_lesson){
                auto ls=lsp.first;
                lesson_sptr asptr= assoc_lesson_base(ls);
                if (fn(asptr)){
                    vl.push_back(asptr);
                }
            }
            /*You will never understand this.*/
            return  [=](vector <lesson_sptr>& vectl)mutable{
                for(lesson_sptr itptr: vl)
                    vectl.push_back(itptr);
            };

        }


        //the main keyword as the LID.
    public:

        /*constructor.*/


        //friend std::istream & >>(student& stu);

        student(string name, string SID, string school, string klass);

        student(string && name, string && SID, string && school, string && klass);
        /*show the scores of that student.*/
        void show_scores(){
            for (pair<string, int> p : this->stu_lesson){
                cout<<p.second<<endl;
            }
        }

        /*get the score of a lesson that student posses. (consumes the id.)
         * Currently not able to check wether the lesson belongs to the student.
         * cowsay: fix this.*/
        auto get_score_of_lesson(string LID){
            for (auto p : this->stu_lesson){
                if (p.first==LID)
                    return p.second;
            }
            return 0;
        }


        /*get the grade.*/
        inline int getgrade(){return this->grade;}
        inline string getgrade_s(){
            std::stringstream ss;
            ss<<this->grade;
            string str;
            ss>>str;
            return str;

        }

        inline vector<pair<string, int>> & get_stu_lesson(){return this->stu_lesson;}


        /*set the grade.*/
        int set_grade(string SID);

        /*The student is able to choose the optional lessons she want.
         *This one will check wether this lesson is applicable.*/
        bool choose_lesson(string lesson);
        /*forwards to the functions above.*/
        void choose_lesson(vector<string> lessons);

        bool admin_add_lesson(string lesson_id);
        /*takes the id, returns the real pointer to the body of a lesson.*/
        inline lesson_sptr assoc_lesson_base(string ls)
        {return (this->lb_binding)->lesson_assoc(ls);}

        /*check wether a lesson is applicable to this student.*/
        inline bool lesson_applicable_p(lesson_sptr lptr)
        {return (lptr->getopt_p() && this->getgrade()==lptr->getgrade());}

        /*cowsay: not tested.*/
        bool delete_opt_lesson(string lesson);

        /*show the lessons of this student.*/
        void show_my_lessons(void);

        /*used by the teacher, never used by a student itself.*/
        bool __set_lesson_score(string ls, int score) throw (negative_score_error);

        /*calculate the GPA.*/
        int get_GPA();
        string get_GPA_s();
        /*instance of that high_level function above.*/
        inline void get_optional_lessons(vector<lesson_sptr>& vl)
        {this->__get_lessons([](lesson_sptr l){return l->getopt_p();})(vl);}

        inline void get_required_lessons(vector<lesson_sptr>& vl)
        {this->__get_lessons([](lesson_sptr l){return not l->getopt_p();})(vl);}

        //never used directly by users.
        bool __force_delete_lesson(string LID);

        //int __fix_my_lessons(); //old implementation.

        bool has_lesson(std::string lid);

        virtual bool v_match_p(std::__1::string v, int cate) override;

        //using stu_sptr=shared_ptr<student>;



        friend inline void operator<<(std::ostream & ofs, student stu){
            ofs<<"[student]\n";
            ofs<<"sid:"<<stu.getSID()<<","<<endl;
            ofs<<"name:"<<stu.getname()<<","<<endl;
            ofs<<"school:"<<stu.getschool()<<","<<endl;
            ofs<<"class:"<<stu.getklass();
            if(not stu.get_stu_lesson().empty()){
                ofs<<","<<endl;
                ofs<<"lessons:{"<<endl;
                for(auto the_lesson_pair : stu.get_stu_lesson()){
                    ofs<<the_lesson_pair.first<<":"<<the_lesson_pair.second<<","<<endl;
                }
                ofs<<"}"<<endl;
            }
            else{
                ofs<<endl;
            }
            ofs<<"[\\student]\n";
        }

    };




/********************************class student ends here.***************************************************/



/********************************class teacher starts here.*************************************************/
    class teacher : public karacter , public std::enable_shared_from_this<teacher>{

    __PBUILDERF(string, TID, private)   /*the teacher id.*/
    __PBUILDERF(vector<string>, tlesson, private)  /*the lessons of this teacher.*/
    using stu_sptr=shared_ptr<student>;
    public:

        /*set the score of that student.*/
        inline bool set_stu_lesson_score(shared_ptr<student> stu, string ls, int score)
        {bool f=stu->__set_lesson_score(ls, score);return f;}

        /*assoc the lesson_base, consumes a lesson id, returns lesson's real pointer.*/
        inline lesson_sptr assoc_lesson_base(string ls)
        {return (this->lb_binding)->lesson_assoc(ls);}

        /*Add that lesson to this teacher. ->m_tlesson
         * will also bind this teacher to the lesson.*/
        bool add_to_my_lesson(string lid);
        void add_to_my_lesson(vector<string> & lessons);

        bool delete_my_lesson(string lid);
        /*show the lessons.*/
        void get_my_lessons(vector<lesson_sptr> &c);

        vector<string>& get_lesson_ref(){return this->m_tlesson;}
        //int __fix_my_lessons();   //old implementation.
        /*show all the students.*/
        void show_my_students();

        /*show student of a specific lesson.*/
        void get_students_of_lesson(string LID, vector<stu_sptr> &vs);

        /*constructor, no lesson currently.*/
        teacher(string name, string TID, string school);      //strongly suggested!!
        bool has_lesson(string lid);

        virtual bool v_match_p(std::__1::string v, int cate) override ;


        friend inline void operator<<(std::ostream & ofs, teacher tch){
            ofs<<"[teacher]\n";
            ofs<<"tid:"<<tch.getTID()<<","<<endl;
            ofs<<"name:"<<tch.getname()<<","<<endl;
            ofs<<"school:"<<tch.getschool();

            if(not tch.gettlesson().empty()){
                ofs<<","<<endl;
                ofs<<"lessons:{"<<endl;
                for(auto lesson : tch.gettlesson()){
                    ofs<<lesson<<",";
                }
                ofs<<endl<<"}"<<endl;
            }
            else{
                ofs<<endl;
            }
            ofs<<"[\\teacher]\n";
        }
    };


/********************************class teacher ends here.***************************************************/


    using stu_sptr=shared_ptr<student>;
    using tec_sptr=shared_ptr<teacher>;
class character_base{

    using stu_sptr=shared_ptr<student>;
    using tec_sptr=shared_ptr<teacher>;
private:

    map<string, stu_sptr> students_binding;/*SID, entity_ptr*/
    map<string, tec_sptr> teachers_binding;/*TID, entity_ptr*/


    bool students_binding_lookup(string SID);
    bool teachers_binding_lookup(string TID);
public:


    inline auto & get_students_binding(){return this->students_binding;}
    inline auto & get_teachers_binding(){return this->teachers_binding;}


    //auto new_student(string name, string sid, string school, string klass);

    bool bind_student(stu_sptr stusptr);

    bool bind_teacher(tec_sptr tecsptr);

    bool remove_student (string sid);
    bool remove_teacher (std::__1::string tid);


    void clear_teachers();
    void clear_students();

    bool modify_student_property(string SID, string slot, string val);

    bool modify_teacher_property(string TID, string slot, string val);

    stu_sptr get_student_of_sid(std::string sid);

    tec_sptr get_teacher_of_tid(std::string tid);
    template<typename c, typename f>
    void __v_character_assoc(std::string v, int cate, c & contianer, f & slot);
    void v_student_assoc(std::string v, int cate, vector<stu_sptr> & container);

    void v_teacher_assoc(std::string v, int cate, vector<tec_sptr> & container);

    friend inline void operator<<(std::ostream & ofs, character_base & cb){
        for(auto the_pair : cb.get_students_binding()){
            ofs<<*(the_pair.second);
        }

        for(auto the_pair : cb.get_teachers_binding()){
            ofs<<*(the_pair.second);
        }
    }
};

class factory{
public:
    static shared_ptr <teacher> createt(string name, string TID, string school)
            throw (binding_not_bound_exception);
    static shared_ptr <student> creates(string name, string TID, string school, string klass)
            throw (binding_not_bound_exception);
    static void createl(string name, string LID, string grade, string credit, string optp);

};

}
#endif
