#ifndef BLOCK_ENCAPSULES_H
#define BLOCK_ENCAPSULES_H
#include <string>
using std::string;

//generate block encapsulers.


static string encapsule_in_block(string blk, string cont){
  return "["+blk+"]\n"+cont+"\n[\\"+blk+"]"  ;
}


#define X(Y) Func(encapsule_in_##Y##_block, Var cont){\
return encapsule_in_block(#Y, cont);};

X(query)

X(info)

X(code)

X(login)

#undef X


#endif // BLOCK_ENCAPSULES_H
