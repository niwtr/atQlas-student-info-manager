#ifndef _XENGINE_H_
#define _XENGINE_H_

#include "defschool.h"
#include "xparser.h"
#include <fstream>
#include <regex>
#include <set>
#include <map>
#include <utility>
#include <functional>
#include <exception>
#include <ctime>
//dsl parser for atQlass.

namespace X {
    using namespace defschool;
    using namespace xparser;
    using std::set;
    using std::map;
    using functor=std::function<void()>;
    using closure=std::function<void()>;
    //using closurev=vector<std::function<void()>>;
    using symb=std::string;
    using symbv=vector<symb>;


    void filter_return(symb &block, bool replace_by_spacep);

    void filter_space(symb &block);
    void filter_dspace(symb &block);
    void filter_comma(symb &block, bool replace_by_spacep);

    void carefully_filter_space(symb &block);

    //only filter the outter space.
    inline void split_by_comma(symb &block, vector<symb> &vs) {
        splitf(block, ",", vs);
    }; //split the
    inline void split_by_space(symb &block, vector<symb> &vs) {
        splitf(block, " ", vs);
    }

    inline void split_by_colon(symb &block, vector<symb> &vs) {
        splitf(block, ":", vs);
    }

    void lift_out_braced_exps(symb &blk, symbv &v);


    class symb_decorate_handler {
    public:
        inline bool __bracketp(symb word, char cs, char ce) {
            return (*(word.begin()) == cs and *(word.end()) == ce);;
        };

        inline bool squarep(symb word) {
            return __bracketp(word, '[', ']');
        }

        inline bool bracep(symb word) {
            return __bracketp(word, '{', '}');
        }

        inline bool commap(symb word) {
            return (*(word.end()) == ',');
        }

        inline int colonedp(symb word) {
            if (*(word.begin()) == ':')
                return -1;
            else if (*(word.end()) == ':') {
                return 1;
            }
            else return 0;

        }

        bool well_sperated_by_colonp(symb word);

        inline void remove_comma(symb &commaed_word) {
            if (commap(commaed_word)) {
                commaed_word.erase(commaed_word.end());
                return;
            }

        }

        inline void remove_colon(symb &coloned_word) {
            if (colonedp(coloned_word) == -1) {
                coloned_word.erase(coloned_word.begin());
            }
            else if (colonedp(coloned_word) == 1) {
                coloned_word.erase(coloned_word.end());
            }

        }

        inline void remove_brackets(symb &bracked_word) {
            if (squarep(bracked_word) or bracep(bracked_word)) {
                bracked_word.erase(bracked_word.begin());
                bracked_word.erase(bracked_word.end());
            }
        }

        inline void remove_decorators(symb &decorated_word) {
            //there is a must, that the comma is first removed, next colon and next brackets.
            remove_comma(decorated_word);
            remove_colon((decorated_word));
            remove_brackets(decorated_word);


        }


    };


    class time_piece {
        //property builders.


        __PBUILDERF(long, _seq, private)
        __PBUILDERF(symb,_block, private)
        __PBUILDERF(string, _category, private)
        __PBUILDERF(closure, _piece, private)


    public:
        static long global_seq ;



        time_piece(symb block, closure piece, string category) :\
         m__block(block),m__piece(piece),m__category(category)
        {
            set_seq(global_seq);
            global_seq++;
        }

        void operator()(){
            this->get_piece()();
        }


    };


    class TIME_MACHINE {


    __PBUILDERF(int, _current_context, private)


        using closurev=vector<closure>;
        //using tagged_closure=pair<string, closure>; //old implementation
        using tclosurev=vector<time_piece>;
        using timefrag=pair<time_t, tclosurev>;
        using symbv=vector<symb>;
    public:

        void compile_definition_block(symb blk, string domain, bool instantp=false);

        void compile_do_block(symb blk, bool instantp=false);

        void insert_time_piece_behind(long fragment_time, time_piece tp, long seq);

        void delete_time_fragment(long fragment_time);

        void load_and_compile();

        void dump_to_xfile();


        inline void freeze(){frozen=true;}
        inline void melt(){frozen=false;}

        inline void update_time(){
           if(not frozen)
            global_time=time(NULL);
           else ;   //the time is frozen!
        }

        bool time_frag_existsp(long curtime);

        void save_current_time(time_piece cap);

        void travel_to_time(time_t curtime);

        symb_decorate_handler dh;

        inline bool __chkvalid(symb word, const set<symb> &domain) {
            return domain.find(word) != domain.end();
        }

        inline bool chkvalid_in_g(symb word) {
            return __chkvalid(word, g_keyword_map);
        }

        inline bool chkvalid_in_label(symb word) {
            return __chkvalid(word, label_word);
        }

        inline bool chkvalid_in_p(symb word) {
            return \
                    __chkvalid(word, property_lesson_word) or \
            __chkvalid(word, property_stu_word) or \
             __chkvalid(word, property_tch_word);
        }

    private:
        //closurev do_closure;
        //closurev def_closure;

        //the time capsule is a vector, in which each element is a pair, the first of
        //that pair, of course, is the time, and the second is a vector,
        //which the element is a pair, the first one being the description (aka, the block
        //code itself, and the second being the closure.
        //this is of course fairly complicated.
        using capsule=vector<timefrag>;
        capsule time_capsule;

        bool frozen=false;

        static long global_time;

        unsigned int error_num=0;

        //keywords.
        const set<symb> g_keyword_map =
                {"for", "that", "satisfies", "do", "as-for", "clear-lesson-base","add-lesson", "add-lesson!",
                        "delete-lesson", "delete-lesson!", "delete", "student", "teacher", "lesson",
                };
        const set<symb> domain_word = {"student", "teacher", "lesson"};

        const set<symb> property_stu_word = {
                "sid", "name", "school", "class", "lessons"
        };
        const set<symb> property_tch_word = {
                "tid", "name", "school", "lessons"
        };
        const set<symb> property_lesson_word = {
                "lid", "name", "grade", "credit", "optp"
        };
        const set<symb> label_word = {
                "do", "student", "teacher", "lesson"
        };


        //maps
        const map<symb, int> stu_property = {
                {"sid", 0},
                {"name", 1},
                {"school", 2},
                {"class", 3},
                {"lessons", 4}
        };
        const map<symb, int> tch_property = {
                {"tid", 0},
                {"name", 1},
                {"school", 2},
                {"lessons", 3}
        };

        const map<symb, int> lesson_property = {
                {"lid", 0},
                {"name", 1},
                {"grade", 2},
                {"credit", 3},
                {"optp", 4}
        };

    public:
        int assoc_property(symb word, string domain) {
            if (domain == "student") {
                return stu_property.find(word)->second;
            }
            else if (domain == "teacher") {
                return tch_property.find(word)->second;
            }
            else
                return lesson_property.find(word)->second;
        }

    };

    class syntax_error : public std::exception {

    public:
        using hint=std::string;
        using word=hint;

        virtual hint describe() = 0;

        virtual hint word_hint() = 0;

    protected:
        hint error_description;
        word error_word;
    };

    class lead_word_syntax_error : public syntax_error {
    public :
        inline lead_word_syntax_error(word wd) {
            this->error_word = wd;
        };

        hint describe() override {
            return hint("Encounter leading word error on do block. ");
        }

        hint word_hint() override {
            return hint("Error on syntax \"" + error_word + "\".");
        }
    };

    class domain_syntax_error : public syntax_error {
    public:
        inline domain_syntax_error(word wd) {
            this->error_word = wd;
        };

        hint describe() override {
            return hint("Encounter domain word error on do block. ");
        }

        hint word_hint() override {
            return hint("Error on syntax \"" + error_word + "\".");
        }
    };

    class target_syntax_error : public syntax_error {
    public:
        inline target_syntax_error(word wd) {
            this->error_word = wd;
        };

        hint describe() override {
            return hint("Encounter target word error on do block. ");
        }

        hint word_hint() override {
            return hint("Error on syntax \"" + error_word + "\".");
        }
    };

    class middle_word_syntax_error : public syntax_error {
    public:
        inline middle_word_syntax_error(word wd) {
            this->error_word = wd;
        };

        hint describe() override {
            return hint("Encounter middle word error on do block. ");
        }

        hint word_hint() override {
            return hint("Error on syntax \"" + error_word + "\".");
        }
    };

    class operator_syntax_error:public syntax_error{
    public:
        inline operator_syntax_error(word wd){this->error_word=wd;}
        hint describe() override {
            return hint("Encounter operator word error on do block. ");
        }
        hint word_hint() override {
            return hint("Error on syntax \""+error_word+"\".");
        }
    };

    static TIME_MACHINE X;  //def of time-machine.




};

#endif
