#ifndef SORTAPI_H
#define SORTAPI_H
#include "def.h"
#include <utility>
#include <algorithm>
using std::swap;

Func (sort_by_slot, Var_ref container, Var fn, Var ascendp){
    for_in(i, container){
        for_in(j, container){
            Var res=fn(i)<fn(j);
            if(not ascendp)res=not res;
            if(res)swap(i, j);
        }
    }
};




#endif // SORTAPI_H
