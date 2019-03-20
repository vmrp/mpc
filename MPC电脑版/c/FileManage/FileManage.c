/*
* FileManage(文件管理器)
* By:天天
* QQ:643552878
* Date:2012..
* PS:全部由天天编写、天天精力之作、之前FileView也弄过3、4次、花了很久的、手机也不知道黑了多少次屏、求大家支持！* *键截图(全屏)
*/

#include <base.h>

//字体
#define FONT 1

//文字宽高
int tw,th;
textwh("天",FALSE,FONT,&tw,&th);

#include <c/FileManage/FileView.h>

#include <c/FileManage/PhotoView.h>

#include <c/FileManage/BookView.h>

#include <c/FileManage/MusicPlay.h>

#include <c/FileManage/tt/ttFileAdd.h>

#include <c/FileManage/tt/tt.h>


uint8 Jm;//界面控制

//Jm取值
enum
{
 Jm_FV,//文件浏览(FileView)
 Jm_PV,//图片浏览(PhotoView)
 Jm_BV,//文本浏览(BookView)
 Jm_MP//音乐播放(MusicPlay)
};

res *lang;
uint16 *Bmp;

//入口函数
int init(void)
{
 //lang=langload("c/FileManage/tt/res_lang0.rc",28);
 ShowMsg("初始化...",0);
 Bmp=FileOpen("c/FileManage/tt/fileicon.bmp");
 Jm=Jm_FV;
 ListInit();
 return 0;
}

//按键入口函数
int event(int type, int p1, int p2)
{
 if(type==KY_UP&&p1==_STAR)
 {
 char *text;
 text=PrintScr("c/FileManage截图.bmp")?"截图成功！":"截图失败！";
 ShowMsg(text,0);
 }

 switch(Jm)
 {
 case Jm_FV:
 FVEvent(type,p1,p2);
 break;
/* case Jm_PV:
 PVEvent(type,p1,p2);
 break;
 case Jm_BV:
 BVEvent(type,p1,p2);
 break;
 case Jm_MP:
 MPEvent(type,p1,p2);
 break;
*/ }

 return 0;
}

//平台暂停入口函数
int pause(void)
{
 return 0;
}

//平台恢复入口函数
int resume(void)
{
 return 0;
}

//程序退出
void ExitApp()
{
 ShowMsg("天天制作",0);
 //free(lang);
 free(Bmp);
 ExitList();
 sleep(500);
 exit();
}

//By:天天