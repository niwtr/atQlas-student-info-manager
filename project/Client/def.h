#ifndef DEF_H
#define DEF_H



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



#define for_in(var, name)\
    for(auto & var : name)


#define for_through(varName, lb, ub, step)\
    for(auto varName=lb;varName<ub;varName=varName+step)

#define Func(fn_name, args...)\
    auto fn_name=[](args...)

#define ref_Func(fn_name, args...)\
    auto fn_name=[&](args...)

#define captured_Func(fn_name, args...)\
    auto fn_name=[=](args...)

#define Var auto
#define Var_ref auto&
#define elif else if
#define unless(X) if(not(X))

#define defvar constexpr Var

#endif // DEF_H
