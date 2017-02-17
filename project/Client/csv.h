#ifndef CSV_H
#define CSV_H

#include <string>
using std::string;
#include "def.h"

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

Func(splitf, Var s, Var delim, Var_ref sv){
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

Func(split_by_colon, Var_ref block, Var_ref vs) {
    splitf(block, ":", vs);
};
Func(split_by_ret, Var_ref block, Var_ref vs){
    splitf(block, "\n", vs);
};


#endif // CSV_
