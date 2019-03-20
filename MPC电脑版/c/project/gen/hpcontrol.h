/***
wildly hp多管控件，适用于游戏
QQ 319965592
定时器不断刷新效果最好。
***/

#ifndef HP_CONTROL_H
#define HP_CONTROL_H

#include <base.h>

//HP管项最大值
#define HpMax 200

//每项HP管的HP值
#define Hpox 200

typedef struct
{
   int hp[HpMax];
   int hr[HpMax];
   int hg[HpMax];
   int hb[HpMax];
   int hpe;
   int hpnum;
   int nis;
   int x;
   int y;
   int w;
   int h;
}Hpc;

typedef Hpc *hpint;

//创建一个HP多管，hpnum 管项上限值，w与Hpox同步
void *hpcreate(int x,int y,int w,int h,int hpnum)
{int time = getuptime();
if(hpnum >= HpMax)return NULL;
hpint pm = malloc(sizeof(Hpc));
memset(pm,0,sizeof(Hpc));
if(pm == NULL)return NULL;
int dp,h1[4]={255,0,255,0},h2[4]={0,0,128,205},h3[4]={0,205,0,0};
for(int a=0;a<hpnum;a++)
{if(dp > 3)dp = 0;
pm->hp[a] = Hpox;
pm->hr[a] = h1[dp];
pm->hg[a] = h2[dp];
pm->hb[a] = h3[dp];
dp++;}
  pm->x = x;
  pm->y = y;
  pm->w = w;
  pm->h = h;
pm->hpnum = hpnum;
pm->hpe = Hpox;
time = getuptime()-time;
//printf("\n本次HP创建时间%d.%ds,%s\n",time/1000,time%1000);
return pm;
}

//显示HP多管，hpj HP特效的速度，is是否刷新背景 0否 1是
int hpshow(hpint pm,int hpj,int is)
{
if(is)
drect(pm->x,pm->y,pm->w,pm->h,0,0,0);
if(pm->hp[pm->hpnum] <= 0){pm->nis = pm->hp[pm->hpnum];pm->hp[pm->hpnum]=0;pm->hpnum--;}
if(pm->nis < 0){pm->hp[pm->hpnum] += (pm->nis);if(pm->nis >= -Hpox)pm->nis = 0;}
drect(pm->x,pm->y,Hpox,pm->h,255,255,255);
if(pm->hp[pm->hpnum] < (pm->hpe))pm->hpe-=hpj;
if(pm->hpe < (pm->hp[pm->hpnum]))pm->hpe = (pm->hp[pm->hpnum]);
drect(pm->x,pm->y,pm->hpe,pm->h,160,0,0);
drect(pm->x,pm->y,pm->hp[pm->hpnum],pm->h,pm->hr[pm->hpnum],pm->hg[pm->hpnum],pm->hb[pm->hpnum]);
ref(pm->x,pm->y,pm->w,pm->h);
return 0;
}

//销毁HP多管
int hpdel(hpint pm)
{free(pm);
return 0;
}

//HP多管的HP值削减，sh削减值
int hpnext(hpint pm,int sh)
{
pm->hp[pm->hpnum] -= sh;
return 0;
}

//获取当前管数
int gethpnum(hpint pm)
{
return (pm->hpnum);
}

//设置管数、重置
int sethpnum(hpint pm,int hpnum)
{
for(int a=0;a<(pm->hpnum);a++)
pm->hp[a] = Hpox;
pm->hpnum = hpnum;
}

#endif