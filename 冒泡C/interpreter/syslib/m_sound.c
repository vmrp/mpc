#include "interpreter.h"
#include "mrc_sound.h"

void Lib_SoundInit(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger = mrc_playSoundExInit(Param[0]->Val->LongInteger);
}

void Lib_SoundLoadFile(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = mrc_playSoundExLoadFile(Param[0]->Val->Integer,Param[1]->Val->Pointer);
}

void Lib_SoundPlay(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = mrc_playSoundEx(Param[0]->Val->Integer,Param[1]->Val->Integer,Param[2]->Val->Integer,NULL);
}
void Lib_SoundPause(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = mrc_pauseSoundEx(Param[0]->Val->Integer);
}

void Lib_SoundResume(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = mrc_resumeSoundEx(Param[0]->Val->Integer);
}

void Lib_SoundStop(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = mrc_stopSoundEx(Param[0]->Val->Integer);
}

void Lib_SoundClose(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = mrc_closeSoundEx(Param[0]->Val->Integer);
}

void Lib_SetVolume(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = mrc_setVolume(Param[0]->Val->Integer);
}

void Lib_GetSoundTotalTime(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = mrc_getSoundTotalTime(Param[0]->Val->Integer,Param[1]->Val->Pointer);
}

void Lib_GetSoundCurTime(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = mrc_getSoundCurTime(Param[0]->Val->Integer,Param[1]->Val->Pointer);
}

void Lib_GetSoundCurTimeMs(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = mrc_getSoundCurTimeMs(Param[0]->Val->Integer,Param[1]->Val->Pointer);
}

void Lib_SetPlayPos(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = mrc_setPlayPos(Param[0]->Val->Integer,*(T_DSM_AUDIO_POS*)Param[1]->Val->Pointer);
}

void Lib_SetPlayTime(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = mrc_setPlayTime(Param[0]->Val->Integer,*(T_DSM_AUDIO_POS*)Param[1]->Val->Pointer);
}

void Lib_GetDeviceState(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->Integer = mrc_getDeviceState(Param[0]->Val->Integer);
}


/* 所有的库函数和他们的原型列表 */
struct LibraryFunction Sound_Functions[15];

void Sound_SetupFunc(void)
{
    Sound_Functions[0].Func = Lib_SoundInit;
    Sound_Functions[0].Prototype = "int soundinit(int);";

    Sound_Functions[1].Func = Lib_SoundLoadFile;
    Sound_Functions[1].Prototype = "int soundloadfile(int,char*);";

    Sound_Functions[2].Func = Lib_SoundPlay;
    Sound_Functions[2].Prototype = "int soundplay(int,int,int);";

    Sound_Functions[3].Func = Lib_SoundPause;
    Sound_Functions[3].Prototype = "int soundpause(int);";

    Sound_Functions[4].Func = Lib_SoundResume;
    Sound_Functions[4].Prototype = "int soundresume(int);";

    Sound_Functions[5].Func = Lib_SoundStop;
    Sound_Functions[5].Prototype = "int soundstop(int);";

    Sound_Functions[6].Func = Lib_SoundClose;
    Sound_Functions[6].Prototype = "int soundclose(int);";

    Sound_Functions[7].Func = Lib_SetVolume;
    Sound_Functions[7].Prototype = "int setvolume(int);";

    Sound_Functions[8].Func = Lib_GetSoundTotalTime;
    Sound_Functions[8].Prototype = "int getsoundtotaltime(int,uint8**);";

    Sound_Functions[9].Func = Lib_GetSoundCurTime;
    Sound_Functions[9].Prototype = "int getsoundcurtime(int,uint8**);";

    Sound_Functions[10].Func = Lib_GetSoundCurTimeMs;
    Sound_Functions[10].Prototype = "int getsoundcurtimems(int,uint8**);";

    Sound_Functions[11].Func = Lib_SetPlayPos;
    Sound_Functions[11].Prototype = "int setplaypos(int,void*);";

    Sound_Functions[12].Func = Lib_SetPlayTime;
    Sound_Functions[12].Prototype = "int setplaytime(int,void*);";

    Sound_Functions[13].Func = Lib_GetDeviceState;
    Sound_Functions[13].Prototype = "int getdevicestate(int);";

    Sound_Functions[14].Func = NULL;
    Sound_Functions[14].Prototype = NULL;
}
