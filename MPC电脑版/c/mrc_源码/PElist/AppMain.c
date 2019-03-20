/*
 * 浏览器核心代码 v1.0
 * Revise frequency: 0003
 * 版权所有 (C) 2012-2012 DeepBlue
 * [6/8/2012]
 */

#include <mrc_base.h>
#include <mrc_base_i.h>
#include <mrc_exb.h>
#include <mrc_bmp.h>

#include "AppTimer.h"
#include "KeyTimer.h"
#include "nv_base.h"
#include "nv_ui.h"
#include "FileType.h"
#include "String.h"
#include "Functions.h"
#include "AppSmallwin.h"
#include "MDskin.h"
//#include "PictureSee.h"

#include "AppMain.h"

#define namelen   100 // 文件名最大长度
#define lunamelen 300 // 完整路径最大长度

// 文件数据结构体
typedef struct
{
	char *name; // 文件名
	int num;    // 文件类型 参见 FileType.h
	int shear;  // 0:正常 1:选择
}ini;

// 节点数据结构体
typedef struct LIST
{
	ini news;          // 数据信息
	struct LIST *last; // 上一节点
	struct LIST *next; // 下一节点
}lists,*plist;

// 文件浏览器数据结构体
typedef struct
{
	char luname[lunamelen]; // 完整路径
	int winnu;              // 已进文件夹数目
	int pagey;              // 列表焦点位置
	int32 pagety;           // 列表开头位置
	int32 number;           // 搜索的总数目
	int32 jlu[30][2];       // 位置数据记录
	plist dirhead;          // 文件夹头链表
	plist filehead;         // 文件头链表
	plist pagehead;         // 列表显示头节点
	plist pagenum;          // 焦点显示节点
}ini_view;

// 触屏管理器数据结构体
typedef struct
{
	int xzAll;     // 选择全部
	int duplicate; // 1.复制 2.剪切
}xz_view;

// 触屏管理器数据结构体
typedef struct
{
	int32 downy; // y触点
	int32 upy;   // y放点
	int32 downx; // x触点
	int32 upx;   // x放点
}mouse_view;

// 浏览器配色结构
typedef struct
{
	// 路径栏配色
	colorct waybackground;        // 背景色
	colorct wayfont;              // 字体色
	// 列表配色
	colorct listbackground;       // 背景色
	colorct listfont;             // 字体色
	colorct listfocusbackground;  // 焦点背景
	colorct listfocusfont;        // 焦点字体色
	colorct listfocusfilenumfont; // 焦点文件名色
	colorct listline;             // 列表滚动条色
}colort_view_M;

static ini_view vies;        // 文件浏览器数据
static colort_view_M viesco; // 文件浏览器颜色数据
static mouse_view viet;      // 触屏器管理:暂时未完善
static xz_view vier;         // 选择复制剪切器
static int32 windownu=0;     // 窗口id

/*---------------------------------------------------------------------------*/

// 选择全部
static void filexzAll()
{
	int32 nuk=0,w=0,h=0;
	char nau[50]={0};
	plist p=vies.dirhead->next;

	while(p != NULL)
	{
		if(vier.xzAll == 0)
			p->news.shear=1;
		else
			p->news.shear=0;
		p=p->next;
		nuk++;
	}
	if(vier.xzAll == 0)
		vier.xzAll=1,mrc_sprintf(nau,"已选择%d个文件/文件夹!",nuk);
	else
		vier.xzAll=0,mrc_sprintf(nau,"解除选择%d个文件/文件夹!",nuk);

	txtwh(nau,&w,&h);
	svrect(mspw/2-(int16)w/2-14,msph-50,(int16)w+28,(int16)h+6+8,255,255,255,240,240,240,4);
	rectJG(mspw/2-(int16)w/2-10,msph-46,(int16)w+20,(int16)h+6,viesco.waybackground);
	txtJG(nau,mspw/2-(int16)w/2,msph-50+7,viesco.listfocusfont);
	fresh();

	mrc_sleep(450);
	FileViewBrowser_UI();
}

/*---------------------------------------------------------------------------*/

// 加载Loading
static void FileViewBrowser_TrendsWin(void)
{
	int32 w=0,h=0;
	char nau[30]={0};

	mrc_sprintf(nau,">%d",vies.number);

	txtwh(nau,&w,&h);

	rectJG(0,14,mspw,18,viesco.waybackground);
	txtJG("正在搜索加载中",3,16,viesco.wayfont);
	txtJG(nau,(int16)(mspw-w-5),16,viesco.wayfont);

	mrc_refreshScreen(0,14,mspw,18);
#ifndef SDK_MOD
#else
	mrc_sleep(100); // Windows because so fast , inconvenient test
#endif
}

// 遍历目录代码并排序
static void FindView_Main(void)
{
	int32 fd=0, listlen=sizeof(lists);

	char buf[namelen]={0}, *ptet=NULL;
	char path[lunamelen]={0};

	plist p=NULL, q=NULL, t=NULL;

	// 如果空链表头
	if(vies.dirhead == NULL)
	{
		// 初始化文件夹链表头
		vies.dirhead=(plist)mrc_malloc(listlen);
		mrc_memset(vies.dirhead,0,listlen);

		vies.dirhead->last=NULL;
		vies.dirhead->next=NULL;

		// 初始化文件链表头
		vies.filehead=(plist)mrc_malloc(listlen);
		mrc_memset(vies.filehead,0,listlen);

		vies.filehead->last=NULL;
		vies.filehead->next=NULL;
	}

	q=vies.dirhead,t=vies.filehead;

    //查找第一个
	fd=mrc_findStart(vies.luname,buf,namelen);
	if(fd>0)
	{
		do
		{
			// 如果 buf 是 "." 或 ".." ，则无效
			if(mrc_strcmp(buf,".") != 0 && mrc_strcmp(buf,"..") != 0)
			{
				// 合成完整路径,获取类型
				mrc_sprintf(path,"%s/%s",vies.luname,buf);

				// 申请节点内存
				p=(plist)mrc_malloc(listlen);
				mrc_memset(p,0,listlen);
				// 获取文件名
				p->news.name=strdup(buf);

				// 如果是文件夹
				if(mrc_fileState(path) == MR_IS_DIR)
				{	
					q->next=p;    // 文件夹链表最后一个指向 p
					p->last=q;    // p 指向文件夹链表最后一个
					p->next=NULL; // p 的下一个指向 NULL
					q=q->next;    // 文件链表尾指向下一个
				}
				else
				{ 
					// 获取后缀
					ptet=mrc_strrchr(path,'.');	 
					// 分析后缀且获取类型序号
					p->news.num=typeanalyse(ptet); 

					t->next=p;    // 文件链表最后一个指向 p
					p->last=t;    // p 指向文件链表最后一个
					p->next=NULL; // p 的下一个指向 NULL
					t=t->next;    // 文件链表尾指向下一个
				}
				// 记录搜索数目
				vies.number++; 

				// 15个刷新一次
                if (0 == vies.number % 15 && vies.number > 0) 
					FileViewBrowser_TrendsWin();
			}
            // 查找下一个
			buf[0] = '\0';
		}
		while(mrc_findGetNext(fd,buf,namelen) == MR_SUCCESS);
	}
    // 结束查找
	mrc_findStop(fd);

	// 如果搜索数目大于0,则合并链表
	if(vies.number > 0) 
		q->next=vies.filehead->next,vies.filehead->next->last=q;

	// 赋值焦点和列表头
	vies.pagehead = vies.pagenum = vies.dirhead->next;
}

// 释放链表内存,置空链表
static void FileViewBrowser_Free(void)
{
	plist p=vies.dirhead,q=NULL;

	while(p!=NULL)
	{
		mrc_free(p->news.name);
		mrc_free(p);
		q=p->next;
		p=q;
	}
	mrc_free(vies.filehead);
	vies.dirhead=NULL;
	vies.filehead=NULL;
}

/*---------------------------------------------------------------------------*/

// 路径栏
static void FileViewBrowser_QUI(void)
{
	int32 w=0,h=0;
	char nau[70]={0};

	rectJG(0,14,mspw,18,viesco.waybackground);

	// 如果空路径
	if(mrc_strcmp(vies.luname,"")==0)
	{
		mrc_memcpy(nau,"根目录",6);	
		txtJG(nau,4,16,viesco.wayfont);
	}
	else
	{
		// 显示当前路径
		txtJG(vies.luname,4,16,viesco.wayfont);
	}

	mrc_sprintf(nau,"%d/%d",vies.pagey+vies.pagety+1,vies.number);

	txtwh(nau,&w,&h);
	rectJG((int16)(mspw-2*w+4),14,(int16)(2*w)-4,18,viesco.waybackground);
	txtJG(nau,(int16)(mspw-w-4),16,viesco.wayfont);

	mrc_refreshScreen(0,14,mspw,18);
}

// 整个列表绘制
void FileViewBrowser_UI(void)
{
	int16 yi=0;
	int32 w=0,h=0,tw=0,th=0,pw=0,ph=0;
	char nau[30]={0},nau2[30]={0},path[lunamelen]={0};

	plist p=vies.pagehead;

	rectJG(0,32,mspw,msph-32,viesco.listbackground);

	mrc_sprintf(nau,"%d/%d",vies.pagey+vies.pagety+1,vies.number);

	txtwh(nau,&w,&h);
	rectJG((int16)(mspw-2*w+4),14,(int16)(2*w)-4,18,viesco.waybackground);
	txtJG(nau,(int16)(mspw-w-4),16,viesco.wayfont);

	// 条件: p不是空节点 与 不得大于显示数目
	while(p!=NULL && yi<=(msph-72)/40)
	{
		mrc_drawRect(0,72+yi*40,mspw,1,viesco.listbackground.r-10,viesco.listbackground.r-10,viesco.listbackground.r-10);
		mrc_drawRect(0,73+yi*40,mspw,1,viesco.listbackground.r+10,viesco.listbackground.r+10,viesco.listbackground.r+10);

		// 如果是焦点
		if(p==vies.pagenum)
		{
			//名称
			mrc_EffSetCon(0,32+(int16)vies.pagey*40,mspw,40,viesco.listfocusbackground.r,viesco.listfocusbackground.g,viesco.listfocusbackground.b);
			//mrc_drawText(p->news.name,35,35+(yi++)*40,220,220,220,0,1);
			txtJG(p->news.name,35,40+(yi++)*40,viesco.listfocusfont);

			//类型
			mrc_sprintf(nau2,"%s",typeanalysetxt(p->news.num));	
			txtwh(nau2,&tw,&th);
			txtJG(nau2,mspw-(int16)tw-10,44+(int16)th+(yi-1)*40,viesco.listfocusfilenumfont);

			// 合成完整路径,获取大小
			mrc_sprintf(path,"%s/%s",vies.luname,p->news.name);
			if(mrc_fileState(path) == MR_IS_FILE)
			{
				mrc_sprintf(nau2,"大小:%d.%dKb",mrc_getLen(path)/1024,mrc_getLen(path)%1024);	
				txtwh(nau2,&pw,&ph);
				txtJG(nau2,mspw-(int16)(tw+pw)-15,44+(int16)(th+(th-ph))+(yi-1)*40,viesco.listfocusfilenumfont);
			}
		}
		else
		{
			//mrc_drawText(p->news.name,35,35+(yi++)*40,0,0,0,0,1);
			txtJG(p->news.name,35,40+(yi++)*40,viesco.listfont);
		}

		if(p->news.shear == 1)
			rectJG(0,32+(yi-1)*40,2,40,viesco.listline);

		// 显示图标
		MDtub(p->news.num,5,35+(yi-1)*40);

		// 指向下一个
		p=p->next;
	}

	if(vies.number == 0)
	{
		mrc_sprintf(nau,"空文件夹,请返回上一级!");	
		txtwh(nau,&w,&h);
		svrect(mspw/2-(int16)w/2-14,msph-50,(int16)w+28,(int16)h+6+8,255,255,255,240,240,240,4);
		rectJG(mspw/2-(int16)w/2-10,msph-46,(int16)w+20,(int16)h+6,viesco.waybackground);
		txtJG(nau,mspw/2-(int16)w/2,msph-50+7,viesco.listfocusfont);
	}

	// 阴影
	drect(0,32,mspw,4,150,150,150,255,255,255);

	// Move strip
	if(vies.number < (msph-72)/40)
		rectJG(mspw-3,37,1,msph-42,viesco.listline);
	else
		rectJG(mspw-3,37+(int16)vies.pagety/3,1,msph-42-(int16)vies.number/3,viesco.listline);

	mrc_refreshScreen(0,14,mspw,msph-14);
}

// 快速移动光标
static void FileViewBrowser_UIEx(int32 data)
{
	int32 tw=0,th=0,pw=0,ph=0;
	char nau[30]={0},path[lunamelen]={0};
	plist p=vies.pagenum;

	mrc_sprintf(nau,"%d/%d",vies.pagey+vies.pagety+1,vies.number);

	txtwh(nau,&pw,&ph);
	rectJG((int16)(mspw-2*pw+4),14,(int16)(2*pw)-4,18,viesco.waybackground);
	txtJG(nau,(int16)(mspw-pw-4),16,viesco.wayfont);

	//名称
	mrc_EffSetCon(0,32+(int16)vies.pagey*40,mspw,40,viesco.listfocusbackground.r,viesco.listfocusbackground.g,viesco.listfocusbackground.b);
	//mrc_drawText(p->news.name,35,35+(yi++)*40,220,220,220,0,1);
	txtJG(p->news.name,35,40+(int16)vies.pagey*40,viesco.listfocusfont);

	//类型
	mrc_sprintf(nau,"%s",typeanalysetxt(p->news.num));	
	txtwh(nau,&tw,&th);
	txtJG(nau,mspw-(int16)tw-10,44+(int16)th+(int16)vies.pagey*40,viesco.listfocusfilenumfont);

	// 合成完整路径,获取大小
	mrc_sprintf(path,"%s/%s",vies.luname,p->news.name);
	if(mrc_fileState(path) == MR_IS_FILE)
	{
		mrc_sprintf(nau,"大小:%d.%dKb",mrc_getLen(path)/1024,mrc_getLen(path)%1024);	
		txtwh(nau,&pw,&ph);
		txtJG(nau,mspw-(int16)(tw+pw)-15,44+(int16)(th+(th-ph))+(int16)vies.pagey*40,viesco.listfocusfilenumfont);
	}

	if(p->news.shear == 1)
		rectJG(0,32+(int16)vies.pagey*40,2,40,viesco.listline);

	MDtub(p->news.num,5,35+(int16)vies.pagey*40);

	if(data == MR_KEY_DOWN)
	{
		//名称
		rectJG((int16)(p->last->news.shear*2),32+(int16)(vies.pagey-1)*40,mspw,40,viesco.listbackground);

		mrc_drawRect((int16)(p->last->news.shear*2),32+(int16)(vies.pagey-1)*40,mspw,1,
			viesco.listbackground.r-10,viesco.listbackground.r-10,viesco.listbackground.r-10);
		mrc_drawRect((int16)(p->last->news.shear*2),33+(int16)(vies.pagey-1)*40,mspw,1,
			viesco.listbackground.r+10,viesco.listbackground.r+10,viesco.listbackground.r+10);

		//mrc_drawText(p->news.name,35,35+(yi++)*40,220,220,220,0,1);
		txtJG(p->last->news.name,35,40+(int16)(vies.pagey-1)*40,viesco.listfont);

		if(p->last->news.shear == 1)
			rectJG(0,32+(int16)(vies.pagey-1)*40,2,40,viesco.listline);

		MDtub(p->last->news.num,5,35+(int16)(vies.pagey-1)*40);

		// 阴影
		if(vies.pagey == 1)
			drect(0,32,mspw,4,150,150,150,255,255,255);
	}
	else if(data == MR_KEY_UP)
	{
		//名称
		rectJG((int16)(p->next->news.shear*2),32+(int16)(vies.pagey+1)*40,mspw,40,viesco.listbackground);
	
		mrc_drawRect((int16)(p->next->news.shear*2),32+(int16)(vies.pagey+2)*40,mspw,1,
			viesco.listbackground.r-10,viesco.listbackground.r-10,viesco.listbackground.r-10);
		mrc_drawRect((int16)(p->next->news.shear*2),33+(int16)(vies.pagey+2)*40,mspw,1,
			viesco.listbackground.r+10,viesco.listbackground.r+10,viesco.listbackground.r+10);

		//mrc_drawText(p->news.name,35,35+(yi++)*40,220,220,220,0,1);
		txtJG(p->next->news.name,35,40+(int16)(vies.pagey+1)*40,viesco.listfont);

		if(p->next->news.shear == 1)
			rectJG(0,32+(int16)(vies.pagey+1)*40,2,40,viesco.listline);

		MDtub(p->next->news.num,5,35+(int16)(vies.pagey+1)*40);
	}
	// Move strip
	if(vies.number < (msph-72)/40)
		rectJG(mspw-3,37,1,msph-42,viesco.listline);
	else
		rectJG(mspw-3,37+(int16)vies.pagety/3,1,msph-42-(int16)vies.number/3,viesco.listline);

	mrc_refreshScreen(0,14,mspw,msph-14);
}

// 滚动条
static int16 Gox=0,Posgox=0;
static void TrendsTXT(int32 data)
{
	int32 w=0,h=0,tw=0,th=0;
	char nau[30]={0},path[lunamelen]={0};

	txtwh(vies.pagenum->news.name,&w,&h);
	if(w > mspw-35-5)
	{
		if(Posgox == 0)
		{
			Gox+=2;
			if(Gox > w-(mspw-40)+10 )
			{
				Posgox=1;
			}
		}
		else
		{
			Gox-=2;
			if(Gox < -10)
			{
				Posgox=0;
			}
		}
		//名称
		rectJG(0,32+(int16)vies.pagey*40,mspw,40,viesco.listbackground);
		mrc_EffSetCon(0,32+(int16)vies.pagey*40,mspw,40,viesco.listfocusbackground.r,viesco.listfocusbackground.g,viesco.listfocusbackground.b);

		//mrc_drawText(p->news.name,35,35+(yi++)*40,220,220,220,0,1);
		txtJG(vies.pagenum->news.name,35-Gox,40+(int16)vies.pagey*40,viesco.listfocusfont);

		//类型
		mrc_sprintf(nau,"%s",typeanalysetxt(vies.pagenum->news.num));	
		txtwh(nau,&w,&h);
		txtJG(nau,mspw-(int16)w-10,44+(int16)h+(int16)vies.pagey*40,viesco.listfocusfilenumfont);
	
		// 合成完整路径,获取大小
		mrc_sprintf(path,"%s/%s",vies.luname,vies.pagenum->news.name);
		if(mrc_fileState(path) == MR_IS_FILE)
		{
			mrc_sprintf(nau,"大小:%d.%dKb",mrc_getLen(path)/1024,mrc_getLen(path)%1024);	
			txtwh(nau,&tw,&th);
			txtJG(nau,mspw-(int16)(w+tw)-15,44+(int16)(th+(h-th))+(int16)vies.pagey*40,viesco.listfocusfilenumfont);
		}

		// Move strip
		if(vies.number < (msph-72)/40)
			rectJG(mspw-3,37,1,msph-42,viesco.listline);
		else
			rectJG(mspw-3,37+(int16)vies.pagety/3,1,msph-42-(int16)vies.number/3,viesco.listline);

		MDtub(vies.pagenum->news.num,5,35+(int16)vies.pagey*40);
	
		if(vies.pagenum->news.shear == 1)
			rectJG(0,32+(int16)vies.pagey*40,2,40,viesco.listline);

		if(vies.pagey == 0)
			drect(0,32,mspw,4,150,150,150,255,255,255);

		mrc_refreshScreen(0,32+(int16)vies.pagey*40,mspw,40);
	}
	else
	{
		KeyTimerStop();
	}
}

/*---------------------------------------------------------------------------*/

//处理文件列表操作
//参数：操作类型
static void FileViewBrowser_Trun(int32 data)
{	
	if(vies.number > 1) // 如果不是空文件夹
	{
		switch(data) // 选择列表项
		{
		case MR_KEY_DOWN:
			// 如果焦点小于屏幕最大位置,焦点指向下一个
			if(vies.pagenum->next != NULL && vies.pagey < (msph-72)/40)
			{
				vies.pagenum=vies.pagenum->next;
				vies.pagey++;
				FileViewBrowser_UIEx(MR_KEY_DOWN);
			}
			// 如果焦点大于于屏幕最大位置,焦点和列表头指向下一个
			else if(vies.pagenum->next != NULL && vies.pagey < vies.number)
			{
				vies.pagehead=vies.pagehead->next,vies.pagenum=vies.pagenum->next;
				vies.pagety++;
				FileViewBrowser_UI();
			}
			// 返回链表头
			else
			{
				vies.pagehead = vies.pagenum = vies.dirhead->next;
				vies.pagety = vies.pagey = 0;
				FileViewBrowser_UI();
			}
			break;
		case MR_KEY_UP:
			// 如果焦点大于0,焦点指向上一个
			if(vies.pagenum != NULL && vies.pagey > 0)
			{
				vies.pagenum=vies.pagenum->last;
				vies.pagey--;
				FileViewBrowser_UIEx(MR_KEY_UP);
			}
			// 如果焦点等于0,焦点和列表头指向上一个
			else if(vies.pagety > 0 && vies.pagehead->last != NULL)
			{
				vies.pagehead=vies.pagehead->last,vies.pagenum=vies.pagenum->last;
				vies.pagety--;
				FileViewBrowser_UI();
			}
			// 返回链表尾
			else
			{
				int32 i=0;
				plist p=vies.dirhead;

				while(p->next != NULL)
					p=p->next;

				vies.pagenum=p;

				if(vies.number > (msph-72)/40)
				{
					while(i < (msph-72)/40 && p!=NULL)
						p=p->last,i++;

					vies.pagehead=p;
					vies.pagety=vies.number-(msph-72)/40-1;
					vies.pagey=(msph-72)/40;
				}
				else
				{
					vies.pagey=vies.number-1;
					vies.pagety=0;
				}
				FileViewBrowser_UI();
			}
			break;
		case MR_KEY_LEFT:
			// 如果焦点大于0,焦点指向列表头
			if(vies.pagey > 0)
			{
				while(vies.pagenum->last != NULL && vies.pagey > 0)
				{
					vies.pagenum=vies.pagenum->last;
					vies.pagey--;
				}
			}
			// 如果焦点等于0,焦点指向上一个列表头,如果到最顶端,自动分析到链表头
			else
			{
				int32 i=0;
				vies.pagey=0;

				while(vies.pagenum->last->last !=NULL && i <= (msph-72)/40)
				{	
					vies.pagenum=vies.pagenum->last,vies.pagehead=vies.pagehead->last;
					vies.pagety--,i++;
				}
			}
			FileViewBrowser_UI();
			break;
		case MR_KEY_RIGHT:
			// 如果焦点小于屏幕最大位置,焦点指向列表头尾部
			if(vies.pagey < (msph-72)/40)
			{
				while(vies.pagenum->next != NULL && vies.pagey < (msph-72)/40)
				{
					vies.pagenum=vies.pagenum->next;
					vies.pagey++;
				}
			}
			// 如果焦点等于屏幕最大位置,焦点指向下一个列表头,如果到最底端,自动分析到链表尾
			else
			{
				int32 i=0;
				while(vies.pagenum->next != NULL && i <= (msph-72)/40)
				{
					vies.pagehead=vies.pagehead->next,vies.pagenum=vies.pagenum->next;
					vies.pagety++,i++;
				}
			}
			FileViewBrowser_UI();
			break;
		}
	}
}

/*---------------------------------------------------------------------------*/

/*
void Back_CB(int32 p0, int32 p1, int32 p2, int32 p3, int32 p4, int32 p5)
{
	FileViewBrowser_QUI();
	FileViewBrowser_UI();
}
*/

// 进入下一个文件夹
static void FileViewBrowser_Next(void)
{
	char path[lunamelen]={0};

	if(vies.number != 0) // 如果不是空文件夹
	{
		// 合并路径
		mrc_sprintf(path,"%s/%s",vies.luname,vies.pagenum->news.name);

		// 如果是文件夹, 则进入下一个文件夹
		if(mrc_fileState(path) == MR_IS_DIR)
		{
			FileViewBrowser_Free();

			mrc_sprintf(vies.luname,"%s",path);

			// 记录上个文件夹位置
			vies.jlu[vies.winnu][0]=vies.pagey;
			vies.jlu[vies.winnu][1]=vies.pagety;

			// 已进数目加1
			vies.winnu++;

			// 清空浏览器位置、数目数据
			vies.number=0,vies.pagey=0,vies.pagety=0;

			FindView_Main();

			FileViewBrowser_QUI();
			FileViewBrowser_UI();
		}
		else
		{
			if(vies.pagenum->news.num == FILE_BMP)
			{
				//WLog(path);
				//windownu=APPWINDOWS_EXITREAME;
				//PictureSee_Func_DrawImg(Back_CB);
			}
		}
	}
}

// 返回上一级文件夹
static void FileViewBrowser_Last(void)
{
	int32 len=0;
	char *text="";
	char path[namelen+1]={0};

	// 查找最后一个 "/" 长度
	text=mrc_strrchr(vies.luname,'/');
	len=mrc_strlen(vies.luname)-mrc_strlen(text);

	// 复制路径指定长度到路径
	memcpy(path,vies.luname,len);
	memset(vies.luname,0,lunamelen);

	mrc_sprintf(vies.luname,"%s",path);

	// 释放链表内存,置空链表
	FileViewBrowser_Free();

	// 已进数目减1
	vies.winnu--;

	// 清空浏览器位置、数目数据
	vies.number=0;

	FindView_Main();

	// 获取浏览器位置记录
	vies.pagey=vies.jlu[vies.winnu][0];
	vies.pagety=vies.jlu[vies.winnu][1];

	// 根据位置记录,浏览器分析显示
	len=0;
	while(len < vies.pagey + vies.pagety)
		vies.pagenum=vies.pagenum->next,len++;

	len=0;
	while(len < vies.pagety)
		vies.pagehead=vies.pagehead->next,len++;

	FileViewBrowser_QUI();
	FileViewBrowser_UI();
}

// 刷新列表
void RefreshList(void)
{
	// 释放链表内存,置空链表
	FileViewBrowser_Free();

	// 清空浏览器位置、数目数据
	vies.number=0,vies.pagey=0,vies.pagety=0;

	FindView_Main();

	FileViewBrowser_QUI();
	FileViewBrowser_UI();
}

// 小窗口取消返回
void Noback(void)
{
	windownu=APPWINDOWS_VIEW;
}

void warePlug_CB(int32 p0, int32 p1, int32 p2, int32 p3, int32 p4, int32 p5)
{
	FileViewBrowser_UI();
}

/*---------------------------------------------------------------------------*/

// 触屏位置分析
static int32 ReturnMoveDownUp(int32 q)
{
	int32 downy=0;
	if(q>32 && q<32+40)
	{
		downy=0;
	}
	if(q>32+40 && q<32+80)
	{
		downy=1;
	}
	if(q>32+80 && q<32+120)
	{
		downy=2;
	}
	if(q>32+120 && q<32+160)
	{
		downy=3;
	}
	if(q>32+160 && q<32+200)
	{
		downy=4;
	}
	if(q>32+200 && q<32+240)
	{
		downy=5;
	}
	if(q>32+240 && q<32+280)
	{
		downy=6;
	}
	return downy;
}

/*---------------------------------------------------------------------------*/
/*
	// 路径栏配色
	colorct waybackground;        // 背景色
	colorct wayfont;              // 字体色
	// 列表配色
	colorct listbackground;       // 背景色
	colorct listfont;             // 字体色
	colorct listfocusbackground;  // 焦点背景
	colorct listfocusfont;        // 焦点字体色
	colorct listfocusfilenumfont; // 焦点文件名色
	colorct listline;             // 列表滚动条色
*/

static void colorthuo(void)
{
	viesco.waybackground        = MDskin_Getcolor("路径栏背景");
	viesco.wayfont              = MDskin_Getcolor("路径栏字体");
	viesco.listbackground       = MDskin_Getcolor("列表背景");
	viesco.listfont             = MDskin_Getcolor("列表字体");
	viesco.listfocusbackground  = MDskin_Getcolor("列表焦点背景");
	viesco.listfocusfont        = MDskin_Getcolor("列表焦点文件名字体");
	viesco.listfocusfilenumfont = MDskin_Getcolor("列表焦点文件类型字体");
	viesco.listline             = MDskin_Getcolor("列表滚动条");
}

/*---------------------------------------------------------------------------*/

// 文件浏览器初始化
void FileViewBrowser_Main(void)
{
	// 获取颜色
	colorthuo();
	// 遍历
	FindView_Main();

	// 显示
	FileViewBrowser_QUI();
	FileViewBrowser_UI();
}

// 文件浏览器按键控制
void FileViewBrowser_Event(int32 code, int32 p, int32 q)
{
	if(windownu == APPWINDOWS_VIEW)
	{
		if(code == MR_KEY_PRESS) // 选择列表项
		{
			KeyTimerStop();
			Gox=0;
			switch(p)
			{
			case MR_KEY_POUND:
			case MR_KEY_SOFTRIGHT:
				if(vies.winnu == 0)
				{
					ExitSmallwin();
					windownu=APPWINDOWS_EXITREAME;
				}
				else
					FileViewBrowser_Last();
				break;
			case MR_KEY_SELECT:
				FileViewBrowser_Next();
				break;
			case MR_KEY_DOWN:
				KeyTimerStart(MR_KEY_DOWN,FileViewBrowser_Trun);
				break;
			case MR_KEY_UP:
				KeyTimerStart(MR_KEY_UP,FileViewBrowser_Trun);
				break;
			case MR_KEY_LEFT:
				KeyTimerStart(MR_KEY_LEFT,FileViewBrowser_Trun);
				break;
			case MR_KEY_RIGHT:
				KeyTimerStart(MR_KEY_RIGHT,FileViewBrowser_Trun);
				break;
			case MR_KEY_2: // 剪切
				break;
			case MR_KEY_3: // 复制
				break;
			case MR_KEY_1: // 单项
				if(vies.number > 0)
				{
					if(vies.pagenum->news.shear == 0)
						vies.pagenum->news.shear=1;
					else
						vies.pagenum->news.shear=0;
					FileViewBrowser_UI();
				}
				break;
			case MR_KEY_4: // 全部
				if(vies.number>0)
				{
					filexzAll();
				}
				break;
			case MR_KEY_7: // 黏贴
				break;
			case MR_KEY_STAR: // 刷新列表
				RefreshList();
				break;
			case MR_KEY_SEND: // 截图
				{
					int32 w=0,h=0;	
					char buf[50]={0},nau[50]={0};
					
					mr_datetime  t_dt;
					mrc_getDatetime(&t_dt);

					mrc_sprintf(buf,"MDs/%d-%d-%d %d.%d.%d.bmp",t_dt.year,t_dt.month,t_dt.day,t_dt.hour,t_dt.minute,t_dt.second);

					mrc_PrintScreen(buf);
	
					mrc_sprintf(nau,"截图成功!");
					txtwh(nau,&w,&h);
					svrect(mspw/2-(int16)w/2-14,msph-50,(int16)w+28,(int16)h+6+8,255,255,255,240,240,240,4);
					rectJG(mspw/2-(int16)w/2-10,msph-46,(int16)w+20,(int16)h+6,viesco.waybackground);
					txtJG(nau,mspw/2-(int16)w/2,msph-50+7,viesco.listfocusfont);
					fresh();

					mrc_sleep(450);
					FileViewBrowser_UI();
				}
				break;
			case MR_KEY_8: // 删除	
				break;
			case MR_KEY_6: // 清空剪切
				break;
			}
	
		}
		else if(MR_KEY_RELEASE == code)
		{
			KeyTimerStop();
			if(vies.pagenum != NULL)
				KeyTimerStart(0,TrendsTXT);
	     }

		if(MR_MOUSE_DOWN == code && mspw <= 240 && msph <= 320) // 240*320
		{
			int32 len=0;
			plist tp=vies.pagehead;

			KeyTimerStop();
			Gox=0;
			if(q<14)
			{
			}
		
			if(q>14 && q<32)
			{
			}

			if(q>32 && q<32+280)
				viet.downy=ReturnMoveDownUp(q);

			viet.downx=p;
			viet.upx=q;

			if(vies.number>viet.downy)
			{
				vies.pagey=viet.downy;

				while(len<viet.downy)
					tp=tp->next,len++;

				vies.pagenum=tp;
			}

			FileViewBrowser_UI();
	
		}
		else if(MR_MOUSE_UP == code && mspw <= 240 && msph <= 320) // 240*320
		{
			int32 len=0;
		
			if(q>32 && q<32+280)
				viet.upy=ReturnMoveDownUp(q);

			if(viet.upy < viet.downy && vies.number > (msph-72)/40) // Towards descend slip
			{
				if(vies.pagey+vies.pagety+(viet.downy-viet.upy) < vies.number - (msph-72)/40 +2)
				{
					while(vies.pagehead != NULL && len < viet.downy-viet.upy)
					{
						vies.pagehead=vies.pagehead->next;
						vies.pagety++,len++;
					}
					vies.pagey=viet.upy;
					FileViewBrowser_UI();
				}
				else if(vies.pagety < vies.number - (msph-72)/40 - 1)
				{
					while(vies.pagehead != NULL && vies.pagety < vies.number - (msph-72)/40 - 1)
						vies.pagehead=vies.pagehead->next,vies.pagety++;

					while(vies.pagenum != NULL && len < viet.upy-viet.downy-2)
						vies.pagenum=vies.pagenum->last,len++;

					vies.pagey=viet.upy;
					FileViewBrowser_UI();
				}
			}
			if(viet.upy > viet.downy && vies.number > (msph-72)/40) // Towards mount slip
			{
				if(vies.pagety-(viet.upy-viet.downy) > 0)
				{
					while(vies.pagehead != NULL && len < viet.upy-viet.downy)
					{
						vies.pagehead=vies.pagehead->last;
						vies.pagety--,len++;
					}
					vies.pagey=viet.upy;
					FileViewBrowser_UI();
				}
				else if(vies.pagety > 0)
				{
					while(vies.pagehead != NULL && vies.pagety > 0)
						vies.pagehead=vies.pagehead->last,vies.pagety--;

					while(vies.pagenum != NULL && len < viet.upy-viet.downy-2)
						vies.pagenum=vies.pagenum->next,len++;

					vies.pagey=viet.upy;
					FileViewBrowser_UI();
				}
			}
			if(viet.upy == viet.downy && q > viet.upx-30 && viet.upx < q+30)
			{
				if(viet.downx < p)
				{	
					if(vies.winnu > 0)
					{
						FileViewBrowser_Last();
					}
				}
				else if(p < viet.downx)
				{	
					FileViewBrowser_Next();
				}
			}
			if(vies.pagenum != NULL)
				KeyTimerStart(0,TrendsTXT);
        }
    }
	else if(windownu != APPWINDOWS_VIEW) // 选择列表项
	{
		RmSmallwin_Event(windownu,code, p, q);
	}
}
