#include "EffectStrategy.h"

#include <Cg\cgGL.h>

void //CgLoader::
    ForEachPath(Types::action f,CGtechnique tech){
        CGpass pass;//�I�񂾃e�N�j�b�N��K�p����ɂ�Handle���K�v�ł�
    /*
    �I�΂ꂽ�e�N�j�b�N�̍ŏ��̃p�X�� cgGetFirstPass�Ŏ擾���Ă��܂��B
    �����āA cgSetPassState�̓p�X�̂��߂ɁA���ׂĂ̏�Ԃ����蓖�Ă܂��B
    This includes evaluating as necessary any state assignment expressions to determine their updated values.
    ����͍X�V���ꂽ �]���@�K�v�Ȃ��ׂĂ̏�Ԋ��蓖�Ď�
    �p�X�̃o�[�e�b�N�X�A�t���O�����g�v���O�������o�C���h���邽�߂ɓK�؂�GLAPI�R�}���h�����s����܂��B
    �icgGLRegisterStates��main����Ăяo�����Ƃ��̂悤�ɋK�肳��Ă��܂��H�j
    */
        //Types::func<int,int> add=[](int i){return i+5;};
        pass=cgGetFirstPass(tech);
        while(pass){//�p�X���������
         cgSetPassState(pass);
         f();//�p�X�̓K�p�Ώۂ�`��
         cgResetPassState(pass);//cgSetPassState�ŕύX���ꂽGL�̏�Ԃ��N���[���A�b�v����
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
//        �G�t�F�N�g�p�����[�^�����f���r���[�`�����s�񂩂�A���݂�GL���f���r���[�`�����s��ɐݒ肵�܂����B
//        ��낤�Ǝv���΃g�[���X���ړ����ꂽ���]��������ł��܂�
//        */
//        cgGLSetStateMatrixParameter(myCgModelViewProjParam,
//            CG_GL_MODELVIEW_PROJECTION_MATRIX,
//            CG_GL_MATRIX_IDENTITY);
//        //cgSetParameter3fv(myCgEyePositionParam, eyePosition);//float3��cg�Ɋ��蓖��
//
//        CGparameter CgFragmentParam_decal = cgGetNamedEffectParameter(_Effect, effectname);
//        //�e�N�X�`��ID�Ɗ֘A�t����A�e�N�X�`���̂��߂ɃT���v���[���Z�b�g
//        cgGLSetTextureParameter(CgFragmentParam_decal, texid);
//        //�T���v���[�̃p�����[�^�̂��߂̏�Ԑݒ�
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
    _Profile=cgGetProfile(effectfile);  //�R���p�C�������Ƃ��̃v���t�B�[�����v���O��������ǂݎ��
    _VertexProgram =
      cgCreateProgramFromFile(
        _Context,  /* Cg runtime context */
        CG_OBJECT, /* �R���p�C����̃I�u�W�F�N�g�R�[�h�Ƃ��ăt�@�C����ǂݏo�� */
        effectfile,  /* �ǂݏo���t�@�C���� */
        _Profile,  /* Profile: OpenGL ARB vertex program */
        "main",     /* Entry function name */
        nullptr);//�G���g���[�ɓn������
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
    _Profile=cgGetProfile(effectfile);  //�R���p�C�������Ƃ��̃v���t�B�[�����v���O��������ǂݎ��
    _FragmentProgram=
      cgCreateProgramFromFile(
        _Context,  /* Cg runtime context */
        CG_OBJECT, /* �R���p�C����̃I�u�W�F�N�g�R�[�h�Ƃ��ăt�@�C����ǂݏo�� */
        effectfile,  /* �ǂݏo���t�@�C���� */
        _Profile,  /* Profile: OpenGL ARB vertex program */
        "main",     /* Entry function name */
        nullptr);//�G���g���[�ɓn������
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
