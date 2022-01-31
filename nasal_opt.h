#ifndef __NASAL_OPT_H__
#define __NASAL_OPT_H__

void calc_const_num(nasal_ast& root)
{
    auto& vec=root.child();
    for(auto& i:vec)
        calc_const_num(i);
    if(root.type()!=ast_add &&
        root.type()!=ast_sub &&
        root.type()!=ast_mult &&
        root.type()!=ast_div &&
        root.type()!=ast_less &&
        root.type()!=ast_leq &&
        root.type()!=ast_grt &&
        root.type()!=ast_geq)
        return;
    if(vec.size()!=2 || vec[0].type()!=ast_num || vec[1].type()!=ast_num)
        return;
    double res;
    switch(root.type())
    {
        case ast_add: res=vec[0].num()+vec[1].num(); break;
        case ast_sub: res=vec[0].num()-vec[1].num(); break;
        case ast_mult:res=vec[0].num()*vec[1].num(); break;
        case ast_div: res=vec[0].num()/vec[1].num(); break;
        case ast_less:res=vec[0].num()<vec[1].num(); break;
        case ast_leq: res=vec[0].num()<=vec[1].num();break;
        case ast_grt: res=vec[0].num()>vec[1].num(); break;
        case ast_geq: res=vec[0].num()>=vec[1].num();break;
    }
    if(std::isinf(res) || std::isnan(res)) // inf and nan will cause number hashmap error in codegen
        return;
    root.set_num(res);
    root.child().clear();
    root.set_type(ast_num);
}
void optimize(nasal_ast& root)
{
    for(auto& i:root.child())
        calc_const_num(i);
}
#endif