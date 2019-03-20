/*
 * 浏览器分类和图形绘制代码
 * 版权所有 (C) 2012-2012 DeepBlue
 * [6/8/2012]
 */

#include <mrc_base.h>

#include "nv_ui.h"

#include "FileType.h"

// 文件图标
void MDtub(int32 dtd,int16 x,int16 y)
{	
	int i=0;
	int32 w=0,h=0;
	svrect(x-2,y+2,25+4,33,255,255,255,245,245,245,3);
	switch(dtd)
	{
	case FILE_DIR:
		mrc_drawRect(x,y,25,33,50,50,50);
		drect(x+1,y+33/2-6+i*40,23,7,255,255,255,40,40,40);
		mrc_drawRect(x+1,y+1+33/2+i*40,23,1,70,70,70);
		mrc_drawRect(x+5,y+33/2+i*40-9,17,10,150,150,150);
		mrc_drawRect(x+2,y+1+33/2-5+i*40,12,5,100,0,0);
		break;
	case FILE_TXT:
	case FILE_WML:
	case FILE_LRC:
		mrc_drawRect(x,y,25,33,140,180,140);
		for(i=0;i<10;i++)
		{
			mrc_drawRect(x+2,y+3+i*3,21,1,60,100,60);
		}
		break;
	case FILE_C:
		mrc_drawRect(x,y,25,33,140,180,140);
		for(i=0;i<10;i++)
		{
			mrc_drawRect(x+2,y+3+i*3,21,1,60,100,60);
		}
		mrc_textWidthHeight("c",0,1,&w,&h);
		mrc_drawText("c",x+25/2-(int16)w/2,y+33/2-(int16)h/2-1,0,0,0,0,1);
		break;
	case FILE_H:
		mrc_drawRect(x,y,25,33,140,180,140);
		for(i=0;i<10;i++)
		{
			mrc_drawRect(x+2,y+3+i*3,21,1,60,100,60);
		}
		mrc_textWidthHeight("h",0,1,&w,&h);
		mrc_drawText("h",x+25/2-(int16)w/2,y+33/2-(int16)h/2,0,0,0,0,1);
		break;
	case FILE_MRP:
		mrc_drawRect(x,y,25,33,200,200,200);
		mrc_drawRect(x+2,y+11,21,3,0,0,0);
		mrc_drawRect(x+2,y+11,3,8,0,0,0);
		mrc_drawRect(x+8,y+11,4,10,0,0,0);
		mrc_drawRect(x+18,y+11,5,9,0,0,0);
		break;
	case FILE_JPG:
	case FILE_GIF:
	case FILE_BMP:
	case FILE_PNG:
		mrc_drawRect(x,y,25,33,100,100,100);
		mrc_drawText("...",x+1,y+10,0,0,0,0,0);
		mrc_bitmapShow(1, x, y, BM_COPY, 0, 0, 25,33);
		break;
	case FILE_ZIP:
	case FILE_RAR:
	case FILE_GZ:
	case FILE_7Z:
		mrc_drawRect(x,y,25,33,50,50,50);
		for(i=0;i<3;i++)
		{
			mrc_drawRect(x+15,y+5+i*5,10,3,80,80,80);
		}
		break;
	case FILE_MP3:
	case FILE_AMR:
	case FILE_WAV:
	case FILE_AWB:
	case FILE_MID:
		mrc_drawRect(x,y,25,33,200,200,200);
		for(i=0;i<7;i++)
		{
			mrc_drawPointEx(x+6+i*3,y+32,100,100,100);
		}
		mrc_drawRect(x+5,y+7,15,3,0,0,0);
		mrc_drawRect(x+5,y+7,1,18,0,0,0);
		mrc_drawRect(x+20,y+7,1,18,0,0,0);
		mrc_drawRect(x+3,y+24,3,3,0,0,0);
		mrc_drawRect(x+18,y+24,3,3,0,0,0);
		break;
	case FILE_MP4:
	case FILE_F4V:
	case FILE_RMVB:
	case FILE_3GP:
		mrc_drawRect(x,y,25,33,200,200,200);
		for(i=0;i<7;i++)
		{
			mrc_drawPointEx(x+6+i*3,y+32,100,100,100);
		}
		break;
	case FILE_OTHER:
		mrc_drawRect(x,y,25,33,200,200,200);
		for(i=0;i<7;i++)
		{
			mrc_drawPointEx(x+6+i*3,y+32,100,100,100);
		}
		break;
	}
}

// 分析文件类型
int typeanalyse(char *name)
{	
	if(mrc_strcmp(name,".txt")==0||mrc_strcmp(name,".TXT")==0)			
	{			
		return FILE_TXT;
	}				
	else if(mrc_strcmp(name,".wml")==0||mrc_strcmp(name,".WML")==0)				
	{		
		return FILE_WML;
	}
	else if(mrc_strcmp(name,".lrc")==0||mrc_strcmp(name,".LRC")==0)				
	{		
		return FILE_LRC;
	}
	else if(mrc_strcmp(name,".c")==0||mrc_strcmp(name,".C")==0)				
	{		
		return FILE_C;
	}
	else if(mrc_strcmp(name,".h")==0||mrc_strcmp(name,".H")==0)				
	{		
		return FILE_H;
	}
	else if(mrc_strcmp(name,".mrp")==0||mrc_strcmp(name,".MRP")==0)				
	{	
		return FILE_MRP;
	}
	else if(mrc_strcmp(name,".jpg")==0||mrc_strcmp(name,".JPG")==0)				
	{		
		return FILE_JPG;
	}
	else if(mrc_strcmp(name,".gif")==0||mrc_strcmp(name,".GIF")==0)				
	{		
		return FILE_GIF;
	}
	else if(mrc_strcmp(name,".bmp")==0||mrc_strcmp(name,".BMP")==0)				
	{		
		return FILE_BMP;
	}
	else if(mrc_strcmp(name,".png")==0||mrc_strcmp(name,".PNG")==0)				
	{		
		return FILE_PNG;
	}
	else if(mrc_strcmp(name,".zip")==0||mrc_strcmp(name,".ZIP")==0)				
	{
		return FILE_ZIP;
	}
	else if(mrc_strcmp(name,".rar")==0||mrc_strcmp(name,".RAR")==0)				
	{		
		return FILE_RAR;
	}
	else if(mrc_strcmp(name,".gz")==0||mrc_strcmp(name,".GZ")==0)				
	{		
		return FILE_GZ;
	}
	else if(mrc_strcmp(name,".7z")==0||mrc_strcmp(name,".7z")==0)				
	{		
		return FILE_7Z;
	}
	else if(mrc_strcmp(name,".mp3")==0||mrc_strcmp(name,".MP3")==0)				
	{
		return FILE_MP3;
	}
	else if(mrc_strcmp(name,".amr")==0||mrc_strcmp(name,".AMR")==0)				
	{		
		return FILE_AMR;
	}
	else if(mrc_strcmp(name,".wav")==0||mrc_strcmp(name,".WAV")==0)				
	{		
		return FILE_WAV;
	}
	else if(mrc_strcmp(name,".awb")==0||mrc_strcmp(name,".AWB")==0)				
	{		
		return FILE_AWB;
	}
	else if(mrc_strcmp(name,".mid")==0||mrc_strcmp(name,".MID")==0)				
	{		
		return FILE_MID;
	}
	else if(mrc_strcmp(name,".mp4")==0||mrc_strcmp(name,".MP4")==0)				
	{		
		return FILE_MP4;
	}
	else if(mrc_strcmp(name,".f4v")==0||mrc_strcmp(name,".F4V")==0)				
	{		
		return FILE_F4V;
	}
	else if(mrc_strcmp(name,".rmvb")==0||mrc_strcmp(name,".RMVB")==0)				
	{		
		return FILE_RMVB;
	}
	else if(mrc_strcmp(name,".3gp")==0||mrc_strcmp(name,".3GP")==0)				
	{		
		return FILE_3GP;
	}
	else
	{
		return FILE_OTHER;
	}
}

// 分析文件类型,返回名称
char *typeanalysetxt(int num)
{
	char *p=NULL;
	switch(num)
	{
	case FILE_DIR:
		p="文件夹";
		break;
	case FILE_TXT:
		p="电子文本";
		break;
	case FILE_WML:
		p="网页文本";
		break;
	case FILE_LRC:
		p="歌词文本";
		break;
	case FILE_C:
	case FILE_H:
		p="源码文本";
		break;
	case FILE_MRP:
		p="应用程序";
		break;
	case FILE_JPG:
	case FILE_GIF:
	case FILE_BMP:
	case FILE_PNG:
		p="图片文件";
		break;
	case FILE_RAR:
	case FILE_ZIP:
	case FILE_7Z:
	case FILE_GZ:
		p="压缩文件";
		break;
	case FILE_MP3:
	case FILE_AMR:
	case FILE_WAV:
	case FILE_AWB:
	case FILE_MID:
		p="音频文件";
		break;
	case FILE_MP4:
	case FILE_F4V:
	case FILE_RMVB:
	case FILE_3GP:
		p="影像文件";
		break;
	case FILE_OTHER:
		p="未知类型";
		break;
	}
	return p;
}