#include "CgLoader.h"
#include <Cg\cgGL.h>
#include <iostream>
//#define _DEBUG
//static CgUtil::_Context=nullptr;
CGcontext CgUtil::InitCg(){
    static CGcontext _Context = cgCreateContext();
    return _Context;
        //else std::cout << "already Cg context made" <<std::endl;
   }
#define CHECKER(state) _err=checker(state);
bool checkForCgError(Types::str situation,Types::str myProgramName,CGcontext myCgContext)
{
  CGerror error;
  const char *string = cgGetLastErrorString(&error);
  
  if (error != CG_NO_ERROR) {
    if (error == CG_COMPILER_ERROR) {
      fprintf(stderr,
             "Program: %s\n"
             "Situation: %s\n"
             "Error: %s\n\n"
             "Cg compiler output...\n%s",
             myProgramName, situation, string,
             cgGetLastListing(myCgContext));
    } else {
      fprintf(stderr,
              "Program: %s\n"
              "Situation: %s\n"
              "Error: %s",
              myProgramName, situation, string);
    }
    return true;
  }
  return false;
}
//http://wiki.livedoor.jp/mikk_ni3_92/d/CgFX%CA%D401

CgLoader::CgLoader(Types::str effectfile,Shader s)
{
    //Cgプログラムのリストみたいのを生成している、すべてのCgプログラムはこの中に属しなければならない。
    auto _Context = CgUtil::InitCg();
    cgGLRegisterStates(_Context);
#ifdef _DEBUG
    for(int i=0;i<cgGetNumSupportedProfiles();++i){
        auto p=cgGetSupportedProfile(i);
        printf_s("%s",cgGetProfileString(p));
        printf_s(" = %d\n",cgGLIsProfileSupported(p));

    }
    printf_s("%d  %d %d\n",GL_TRUE,CG_TRUE,GL_FALSE);
#endif
    auto checker=[effectfile,_Context](Types::str sit)
      {
          return checkForCgError(sit, effectfile,_Context) ;
      };
    CHECKER("state reg");
    //次のプログラムがバインドされるか、updateparamが実行されるまで変更は遅延される
    cgSetParameterSettingMode(_Context, CG_DEFERRED_PARAMETER_SETTING);
    CHECKER("parametor setting");

    //シェーダーの種類など、テッセレーションコントロールや、ジオメトリシェーダも
    /*
    エフェクトはフラグメントもバーテックスも入る。
    いくつかのテクニックを持つことができ、それぞれパスを持っている
    テクニックは材質ごとのシェーダのように考えられる
    パスには頂点シェーダやフラグメントシェーダが記述されており、パスはレンダリングパイプラインと見ることができる
    コンパイル時にプロフィールを指定すればいい
    */
    auto fName="main";
    switch (s)
    {//state持っておいて、オブジェクトを張り替えるパターンが望まれる
    case EffectObj:
	//_Effect=cgCreateEffectFromFile(_Context,effectfile,nullptr);
	CHECKER("effect loaded");
    break;
    case VertexObj:
        _shader=new VertexStrategy(effectfile,_Context);
	CHECKER("vertex loaded");
        break;
    case FragmentObj:
        _shader=new FragmentStrategy(effectfile,_Context);
	CHECKER("fragment loaded");
	break;
    default:
        break;
    }
    
    //エラーが起こったときデータを保護するオプション
    cgGLSetManageTextureParameters(_Context,CG_TRUE);

}
/*
cgGLUnbindProgram unbinds the program which is bound in the profile specified by profile.
プロフィールに記述された範囲のプログラムをアンバインドします。
It also resets the texture state back to the state
it was in at the point cgGLBindProgram was first called with a program in the given profile.
テクスチャ状態を引数に与えられたプロフィールでcgGLBindProgramが最初に呼ばれた状態にリセットします
*/

/*
auto alphaShader=new CgLoader("alpha.vertex","alpha.pixel");

-loop
auto boxFunc=[](){ glBegin(); glVertex }//こうすればここまでこのシェーダーを使うことを明示できる
alphaShader->BindEnable(boxFunc);//enableで描画後disable
-loop end
*/
void CgLoader::
    BindEnable(Types::action f){
        //テクニックが有効かどうかvalidaTechで調べられ、無効ならnextTechできる
	//材質の設定がおかしいのならこの時点でエラーを出すべき
        //レンダリングクオリティを扱うのなら
        //cgGetNamedTechniqueで探す。
	//
        _shader->BindEnable(f);
        //if(!tech){std::cerr << " no tech avail" <<std::endl;}
        //checkForCgError("effect applied",cgGetEffectName(_Effect));
        //cgGLEnableProfile(_Profile);
        //cgGLUnbindProgram(_Profile);
}
bool CgLoader::IsError(){return _err;}

CgLoader::~CgLoader(void)
{
    _shader->~IShaderStrategy();
    cgDestroyContext(CgUtil::InitCg());
}
/*
const
変数を定数としてマークします
Unless this is combined with uniform or varying,
the declarator must include an initializer to give the variable a value.

extern
Marks this declaration as solely a declaration and not a definition.
There must be a non-extern declaration elsewhere in the program.

in
Only usable on parameter and varying declarations.
Marks the parameter or varying as an input to the function or program.
Function parameters with no in, out, or inout specifier are implicitly in

inline
関数宣言にのみ使用可能。
コンパイラに可能なら関数を常にインライン化するよう指示します

inout
パラメータとvarying宣言にのみ使用可能です。
Marks the parameter or varying as both an input to and an output from the function or program

static
グローバルでのみ使用可能
変数をプログラム中でプライベートとしてマークします。そして外から見えなくします。
uniformや varyingとともには使用できません。

out
Only usable on parameter and varying declarations.
Marks the parameter or varying as an output from the function or program

uniform
Usable on global variables and parameters to the top-level main function of a program
and to define constant buffers (see the Constant Buffers section).
グローバル変数、トップレベルメイン関数のパラメータと、定数バッファの定義に使用可能（詳しくは定数バッファの項を参照）
もし非トップレベルパラメータに記述されると無視されます。
The intent of this rule is to allow a function to serve as either a top-level function or as one that is not.
このルールの意図は　トップレベル関数に送信可能かそうでないか
uniform変数は非uniform変数のように読み出し、書き込みが可能であることに注意してください。
The uniform qualifier simply provides information about how the initial value of the variable is 
to be specified and stored, through a mechanism external to the language.
uniformは単純に　値の初期化が記述され、記憶されているかの情報を外の言語に提供します。

varying
グローバル変数とトップレベルメイン関数のパラメータにのみ使用できます。
If specified on a non-top-level function parameter it is ignored.

profile name
The name of any profile (or profile wildcard -- see Profiles)
may be used as a specifier on any function declaration.
It defines a function that is only visible in the corresponding profiles.


*/