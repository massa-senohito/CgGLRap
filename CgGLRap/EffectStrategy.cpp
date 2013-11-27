#include "EffectStrategy.h"

#include <Cg\cgGL.h>

void //CgLoader::
    ForEachPath(Types::action f,CGtechnique tech){
        CGpass pass;//選んだテクニックを適用するにはHandleが必要です
    /*
    選ばれたテクニックの最初のパスを cgGetFirstPassで取得しています。
    そして、 cgSetPassStateはパスのために、すべての状態を割り当てます。
    This includes evaluating as necessary any state assignment expressions to determine their updated values.
    これは更新された 評価　必要なすべての状態割り当て式
    パスのバーテックス、フラグメントプログラムをバインドするために適切なGLAPIコマンドが発行されます。
    （cgGLRegisterStatesをmainから呼び出したときのように規定されています？）
    */
        //Types::func<int,int> add=[](int i){return i+5;};
        pass=cgGetFirstPass(tech);
        while(pass){//パスがある限り
         cgSetPassState(pass);
         f();//パスの適用対象を描画
         cgResetPassState(pass);//cgSetPassStateで変更されたGLの状態をクリーンアップする
         pass=cgGetNextPass(pass);}

}
//EffectStrategy::EffectStrategy(Types::str effectfile)
//{
//}
//void EffectStrategy::
//    SendTextureToFlagment(Types::str effectname,int texid){
//        auto myCgModelViewProjParam=cgGetEffectParameterBySemantic(_Effect,"ModelViewProj");
//
//        /*
//        エフェクトパラメータをモデルビュー～透視行列から、現在のGLモデルビュー～透視行列に設定しました。
//        やろうと思えばトーラスを移動されたり回転させたりできます
//        */
//        cgGLSetStateMatrixParameter(myCgModelViewProjParam,
//            CG_GL_MODELVIEW_PROJECTION_MATRIX,
//            CG_GL_MATRIX_IDENTITY);
//        //cgSetParameter3fv(myCgEyePositionParam, eyePosition);//float3をcgに割り当て
//
//        CGparameter CgFragmentParam_decal = cgGetNamedEffectParameter(_Effect, effectname);
//        //テクスチャIDと関連付ける、テクスチャのためにサンプラーをセット
//        cgGLSetTextureParameter(CgFragmentParam_decal, texid);
//        //サンプラーのパラメータのための状態設定
//        cgSetSamplerState(CgFragmentParam_decal);
//        cgGLEnableTextureParameter(CgFragmentParam_decal);
//}
//void EffectStrategy::BindEnable(Types::action){
//}
//EffectStrategy::~EffectStrategy()
//{
//
//    //cgDestroyEffect(_Effect);
//}
VertexStrategy::VertexStrategy(Types::str effectfile,CGcontext _Context){
    _Profile=cgGetProfile(effectfile);  //コンパイルしたときのプロフィールをプログラムから読み取る
    _VertexProgram =
      cgCreateProgramFromFile(
        _Context,  /* Cg runtime context */
        CG_OBJECT, /* コンパイル後のオブジェクトコードとしてファイルを読み出す */
        effectfile,  /* 読み出すファイル名 */
        _Profile,  /* Profile: OpenGL ARB vertex program */
        "main",     /* Entry function name */
        nullptr);//エントリーに渡す引数
    cgGLSetOptimalOptions(_Profile);
}
void VertexStrategy::BindEnable(Types::action f){

    cgGLBindProgram(_VertexProgram);
      f();
    cgGLUnbindProgram(_Profile);
}
void VertexStrategy::SendTextureToFlagment(Types::str,int){
}
VertexStrategy::~VertexStrategy(){

    cgDestroyProgram(_VertexProgram);
    printf_s("destructed vertex");
}
FragmentStrategy::FragmentStrategy(Types::str effectfile,CGcontext _Context){
    _Profile=cgGetProfile(effectfile);  //コンパイルしたときのプロフィールをプログラムから読み取る
    _FragmentProgram=
      cgCreateProgramFromFile(
        _Context,  /* Cg runtime context */
        CG_OBJECT, /* コンパイル後のオブジェクトコードとしてファイルを読み出す */
        effectfile,  /* 読み出すファイル名 */
        _Profile,  /* Profile: OpenGL ARB vertex program */
        "main",     /* Entry function name */
        nullptr);//エントリーに渡す引数
    cgGLSetOptimalOptions(_Profile);

}

void FragmentStrategy::BindEnable(Types::action f){

    cgGLBindProgram(_FragmentProgram);
	f();
    cgGLUnbindProgram(_Profile);
}
void FragmentStrategy::SendTextureToFlagment(Types::str,int){
}
FragmentStrategy::~FragmentStrategy(){
    cgDestroyProgram(_FragmentProgram);
    printf_s("destructed fragment");
}
