#ifndef __NV_UI_H__
#define __NV_UI_H__

#define mspw MSpw()
#define msph MSph()

typedef struct COLORCT
{
	uint8 r;
	uint8 g;
	uint8 b;
}colorct;

// 初始化
void nv_ui_init(void);
// 卸载
void font_exit(void);

// 清屏
void cls(int32 r,int32 g,int32 b);
// 清屏最快
void clsr(int32 w,int32 h);

// 显示缩写
void fms(int16 x,int16 y,int16 w,int16 h);
// 小显示
void fresh(void);
// 全屏显示
void fresht(void);
// 屏幕宽
int16 MSpw(void);
// 屏幕高
int16 MSph(void);

// 字体宽高
int32 txtwh(char *text,int32 *tw,int32 *th);

// 根据颜色结构画图
void txtJG(char* szText,int16 x,int16 y,colorct bs);
void rectJG(int16 x,int16 y,int16 w,int16 h,colorct bs);
void yrectJG(int16 x,int16 y,int16 w,int16 h,colorct bs);
void ysrectJG(int16 x,int16 y,int16 w,int16 h,colorct bs,colorct bs2);

// 根据颜色RGB画图

// 写中文12号
void txt(char* szText,int16 x,int16 y,uint8 r,uint8 g,uint8 b);
// 写unicode12号
void txt2(char* szText,int16 x,int16 y,uint8 r,uint8 g,uint8 b);

// 透明渐变从上而下
void drect(int16 x,int16 y,int16 w,int16 h,uint8 r1,uint8 g1,uint8 b1,uint8 r2,uint8 g2,uint8 b2);
// 透明渐变从左到右
void drect2(int16 x,int16 y,int16 w,int16 h,uint8 r1,uint8 g1,uint8 b1,uint8 r2,uint8 g2,uint8 b2);
// 渐变按钮
void ysrect(int16 x,int16 y,int16 w,int16 h,uint8 r1,uint8 g1,uint8 b1,uint8 r2,uint8 g2,uint8 b2);
// 渐变矩形从左到右
void wsrect(int16 x,int16 y,int16 w,int16 h,uint8 r1,uint8 g1,uint8 b1,uint8 r2,uint8 g2,uint8 b2);
// 渐变矩形从上而下
void srect(int16 x,int16 y,int16 w,int16 h,uint8 r1,uint8 g1,uint8 b1,uint8 r2,uint8 g2,uint8 b2);
// x矩形
void urect(int16 x,int16 y,int16 tx,int16 ty,uint8 r,uint8 g,uint8 b,int16 hu);
// 1格圆角矩形
void yrect(int16 x,int16 y,int16 w,int16 h,uint8 r,uint8 g,uint8 b);
// 1格圆角矩形框架
void nrect(int16 x,int16 y,int16 w,int16 h,uint8 r,uint8 g,uint8 b,int16 hu);
// 1格透明圆角矩形框架
void ntrect(int16 x,int16 y,int16 w,int16 h,uint8 r,uint8 g,uint8 b,int16 hu);
// 直角矩形框
void mrect(int16 x,int16 y,int16 w,int16 h,uint8 r,uint8 g,uint8 b,int16 hu);
// 4格圆角矩形
void vrect(int16 x,int16 y,int16 w,int16 h,uint8 r,uint8 g,uint8 b,int t);
// 渐变4格透明圆角矩形阴影
void svrect(int16 x,int16 y,int16 w,int16 h,uint8 r1,uint8 g1,uint8 b1,uint8 r2,uint8 g2,uint8 b2,int t);
// 三角形
void san(int16 x,int16 y,int16 w,uint8 r,uint8 g,uint8 b,int16 hu);
// 1格透明圆角矩形
void trect(int16 x,int16 y,int16 w,int16 h,uint8 r,uint8 g,uint8 b);

#endif  // __NV_UI_H__