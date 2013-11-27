#pragma once
#include "TypeUtil.h"

#include "Cg\cg.h"
class IShaderStrategy{
public:
    virtual void BindEnable(Types::action) =0;
    virtual void SendTextureToFlagment(Types::str,int)=0;
    virtual ~IShaderStrategy(){};
};

//class EffectStrategy:public IShaderStrategy
//{
//    CGprofile _Profile;
//    CGeffect _Effect;
//
//public:
//    EffectStrategy(Types::str effectfile);
//    void BindEnable(Types::action);
//    void SendTextureToFlagment(Types::str,int);
//    ~EffectStrategy(void);
//};

class VertexStrategy:public IShaderStrategy{

    CGprogram _VertexProgram;
    CGprofile _Profile;
public:
    VertexStrategy(Types::str effectfile,CGcontext);
    void BindEnable(Types::action);
    void SendTextureToFlagment(Types::str,int);
    ~VertexStrategy(void);
};

class FragmentStrategy:public IShaderStrategy{
    CGprogram _FragmentProgram;
    CGprofile _Profile;
public:
    FragmentStrategy(Types::str effectfile,CGcontext);
    void BindEnable(Types::action);
    void SendTextureToFlagment(Types::str,int);
    ~FragmentStrategy(void);
};