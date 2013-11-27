#pragma once
#include "TypeUtil.h"

#include "EffectStrategy.h"
#include "Cg\cg.h"
enum Shader{
  EffectObj,
  VertexObj,
  FragmentObj,
};
class CgUtil{
public:
    static CGcontext InitCg();
    
};
//http://tip.sakura.ne.jp/htm/gl_chp20frame.htm#c_1_0_0_0_0
//ポストエフェクトするには、画面全体にテクスチャをはればいい
void ForEachPath(Types::action,CGtechnique);
class CgLoader
{
    IShaderStrategy* _shader;//pimplかなにかを使わないと消せない
    bool _err;
public:
    CgLoader(Types::str,Shader);
    bool IsError();
    void BindEnable(Types::action);
    void SendTextureToFlagment(Types::str,int);
    ~CgLoader(void);
};

