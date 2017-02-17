#ifndef XPARSER_H
#define XPARSER_H

#include"defschool.h"
#include"regex"
#include<algorithm>


namespace xparser{


using namespace defschool;
auto splitf=[](string s, string delim, vector <string> & sv){
    std::string delimiter=delim;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        sv.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    sv.push_back(s);
};//should garantee that at list one str is put into the vector.
auto erase_space=[](string s){

            auto x=s;

    const char space=' ';
    for(auto it=x.begin();it!=x.end(); it++){

        if((*it)==space){
            cout<<"encounter empty."<<endl;
            x.erase(it);

        }
    }

    return x;
};

auto bool_to_string=[](bool is){
    if(is)
        return "yes";
    else
        return "no";
};

//transform to lower case.
auto to_lower=[](string  s){
    std::transform(s.begin(), s.end(), s.begin(), (int (*)(int))std::tolower);
    return s;
};

void filter(string info, int cate, vector<stu_sptr> &container);
void filter(string info, int cate, vector<lesson_sptr> &container);
void filter(string info, int cate, vector<tec_sptr> &container);
void parse_and_assoc_base(string s, vector<lesson_sptr> & container);
void parse_and_assoc_base(string s, vector<stu_sptr> & container);
void parse_and_assoc_base(string s, vector<tec_sptr> & container);
void parse_and_assoc_applicable_lesson(string s, stu_sptr stu, vector<lesson_sptr> &container);

}





#endif // XPARSER_HPP
