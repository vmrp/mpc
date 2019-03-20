#include "interpreter.h"
#include "mrc_base.h"
#include "mrc_exb.h"
#include "mrc_base_i.h"
#include "mrc_bmp.h"
#include "myfunc.h"
#include "mrc_sound.h"
#include "picoc.h"
#include "interpreter.h"
#include "momo.h"

char getHostCB[32];
char initNetworkCB[32];
char Source[256];

int32 GET_HOST_CB(int32 ip)
{
    FuncRet=ip;//FuncRet是内部符号_ret
    if(CheckBaseCall(getHostCB,&IntType,1))//检查函数是否符合要求
    {
        ProgramFail(NULL, "无法使用%s",getHostCB);;
    }
    mrc_sprintf(Source,"_ret=%s(_ret);",getHostCB);
    PicocParse(" ", Source, strlen(Source), TRUE, TRUE, FALSE);
    return FuncRet;
}

void Lib_GetHostByName(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    mrc_strcpy(getHostCB,Param[1]->Val->Pointer);
    ReturnValue->Val->LongInteger = mrc_getHostByName(Param[0]->Val->Pointer, GET_HOST_CB);
}

int32 INIT_NETWORK_CB(int32 result)
{
    FuncRet=result;//FuncRet是内部符号_ret
    if(CheckBaseCall(initNetworkCB,&IntType,1))//检查函数是否符合要求
    {
        ProgramFail(NULL, "无法使用%s",initNetworkCB);;
    }
    mrc_sprintf(Source,"_ret=%s(_ret);",initNetworkCB);
    PicocParse(" ", Source, strlen(Source), TRUE, TRUE, FALSE);
    return FuncRet;
}
void Lib_InitNetwork(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    mrc_strcpy(initNetworkCB,Param[0]->Val->Pointer);
    ReturnValue->Val->LongInteger = mrc_initNetwork(INIT_NETWORK_CB, Param[1]->Val->Pointer);
}

void Lib_GetNetworkID(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger = mrc_getNetworkID();
}

void Lib_CloseNetwork(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger =mrc_closeNetwork();
}

void Lib_Socket(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger = mrc_socket(Param[0]->Val->LongInteger, Param[1]->Val->LongInteger);
}
void Lib_Connect(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger = mrc_connect(Param[0]->Val->LongInteger, Param[1]->Val->LongInteger,
        Param[2]->Val->UnsignedShortInteger, Param[3]->Val->LongInteger);
}
void Lib_GetSocketState(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger = mrc_getSocketState(Param[0]->Val->LongInteger);
}
void Lib_CloseSocket(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger = mrc_closeSocket(Param[0]->Val->LongInteger);
}

void Lib_Recv(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger = mrc_recv(Param[0]->Val->LongInteger, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}
void Lib_Send(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger = mrc_send(Param[0]->Val->LongInteger, Param[1]->Val->Pointer, Param[2]->Val->Integer);
}

void Lib_Recvfrom(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger = mrc_recvfrom(Param[0]->Val->LongInteger, Param[1]->Val->Pointer, Param[2]->Val->Integer
        ,Param[3]->Val->Pointer, Param[4]->Val->Pointer);
}
void Lib_Sendto(struct ParseState *Parser, struct Value *ReturnValue, struct Value **Param, int NumArgs)
{
    ReturnValue->Val->LongInteger = mrc_sendto(Param[0]->Val->LongInteger, Param[1]->Val->Pointer, Param[2]->Val->Integer
        ,Param[3]->Val->LongInteger, Param[4]->Val->UnsignedShortInteger);
}


struct LibraryFunction mrc_net_Functions[13];
void mrc_net_SetupFunc(void)
{
    mrc_net_Functions[0].Func = Lib_Recv;
    mrc_net_Functions[0].Prototype = "int recv(int,char*,int);";

    mrc_net_Functions[1].Func = Lib_Send;
    mrc_net_Functions[1].Prototype = "int send(int,char*,int);";

    mrc_net_Functions[2].Func = Lib_Sendto;
    mrc_net_Functions[2].Prototype = "int sendto(int,char*,int,int,uint16);";

    mrc_net_Functions[3].Func = Lib_Connect;
    mrc_net_Functions[3].Prototype = "int connect(int,int,int,int);";

    mrc_net_Functions[4].Func = Lib_GetSocketState;
    mrc_net_Functions[4].Prototype = "int getsocketstate(int);";

    mrc_net_Functions[5].Func = Lib_CloseSocket;
    mrc_net_Functions[5].Prototype = "int closesocket(int);";

    mrc_net_Functions[6].Func = Lib_GetNetworkID;
    mrc_net_Functions[6].Prototype = "int getnetworkid();";

    mrc_net_Functions[7].Func = Lib_Recvfrom;
    mrc_net_Functions[7].Prototype = "int recvfrom(int,char*,int,int*,uint16*);";

    mrc_net_Functions[8].Func = Lib_Socket;
    mrc_net_Functions[8].Prototype = "int socket(int,int);";

    mrc_net_Functions[9].Func = Lib_CloseNetwork;
    mrc_net_Functions[9].Prototype = "int closenetwork();";

    mrc_net_Functions[10].Func = Lib_GetHostByName;
    mrc_net_Functions[10].Prototype = "int gethostbyname(char*,char*);";

    mrc_net_Functions[11].Func = Lib_InitNetwork;
    mrc_net_Functions[11].Prototype = "int initnetwork(char*,char*);";

    mrc_net_Functions[12].Func = NULL;
    mrc_net_Functions[12].Prototype = NULL;
}
