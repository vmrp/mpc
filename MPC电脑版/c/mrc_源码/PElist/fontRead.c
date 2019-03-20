#include "mrc_base.h"
#include "mrc_base_i.h"
#include "mrc_exb.h"
#include "mrc_bmp.h"
#include "debug.h"

#include "fontRead.h"

//文件名规范
//fpoint_1.bin    fpoint_2.bin     fpoint_3.bin 分别对应BIG、MEDIUM、SAMLL的字体点阵
//fun_1.bin    fun_2.bin     fun_3.bin 分别对应BIG、MEDIUM、SAMLL的字体unicode索引
//fgb_1.bin    fgb_2.bin     fgb_3.bin 分别对应BIG、MEDIUM、SAMLL的字体gb2312索引
//
//
#define FONT_POINT "fpoint_%d.bin"
#define FONT_UINDEX "fun_%d.bin"
#define FONT_GBINDEX "fgb_%d.bin"

#define FONT_PATH "MDs/font.mrp"

typedef enum
{
	FONT_TYPE_BIG = 1,
	FONT_TYPE_MEDIUM = 2,
	FONT_TYPE_SAMLL=3
}E_FONT_TYPE;


#define FONT_DES  128
#define INSERT_WIDHT 0
#define INSERT_HEIGHT 0

typedef struct fontPoint
{
	uint8 *uIndexBuf;
	uint8 *bIndexBuf;
	uint8 *PointBuf;
	
	BOOL isMemBuf;
	BOOL isSupport;
	
	int32 uIndexLen;
	int32 bIndexLen;
	int32 PointLen;
	
	int32 fontWidth;
	int32 fontHeight;
	int32 fontLen;
	
	int32 fontHandle;

}T_POINT_INFO;


T_POINT_INFO g_big;
T_POINT_INFO g_medium;
T_POINT_INFO g_small;
T_POINT_INFO g_nowUse;

static uint8 Buf[FONT_DES];		     //字体点阵信息缓存

static mr_screeninfo screen_Size;
static BOOL useSysFontInfo;//TURE标识不存在游戏自带字库，那么使用系统字库; False 标识存在游戏自带字库，使作自带字库

T_POINT_INFO GetSupportFont(MR_FONT_TYPE  font)
{
	switch(font)
		{
		case MR_FONT_BIG:
			if(g_big.isSupport)
				{
				return g_big;
				}
			else if(g_medium.isSupport)
				{
				return g_medium;
				}
			else if(g_small.isSupport)
				{
				return g_small;
				}
		case MR_FONT_MEDIUM:
			if(g_medium.isSupport)
				{
				return g_medium;
				}
			else if(g_big.isSupport)
				{
				return g_big;
				}
			else if(g_small.isSupport)
				{
				return g_small;
				}
		case MR_FONT_SMALL:
			if(g_small.isSupport)
				{
				return g_small;
				}
			else if(g_big.isSupport)
				{
				return g_big;
				}
			else if(g_medium.isSupport)
				{
				return g_medium;
				}
	}
	return g_medium;
}

int32 GetOffSet(uint16 chr, E_CHR_SET flag)
{	//flag TRUE 标识为UNICODE串，FALSE标识为GB串
	uint16 iM = 0;//(一个索引项含16位UNICODE值16位偏移量)
	uint16 iB = 1;
	uint16 iE = 0;
	int32 UValue = 0;

	int32 indexLen = 0;
	uint8 *buf = NULL;
	switch(flag)
	{
		case MR_SET_GB:
			indexLen = g_nowUse.bIndexLen;
			buf = g_nowUse.bIndexBuf;
			break;
		case MR_SET_UNICODE:
			indexLen = g_nowUse.uIndexLen;
			buf = g_nowUse.uIndexBuf;
			break;
		default:
			return 0;
	}

	iE = (uint16)indexLen /5;
	while(iB <= iE)
	{
		iM = (iB + iE)/2;
		UValue = buf[(iM-1)*5];
		UValue = UValue << 8;
		UValue += buf[(iM-1)*5+1];
		if(chr == UValue)
		{
			UValue = buf[(iM-1)*5+2];
			UValue = UValue << 8;
			UValue += buf[(iM-1)*5+3];
			UValue = UValue << 8;
			UValue += buf[(iM-1)*5+4];
			return UValue;
		}
		else if(chr > UValue)
		{
			iB = iM+1;
		}
		else
		{
			iE = iM-1;
		}
	}
	return 0;
}
int32 freeFontData(char *filename,char *path)
{
	
	return MR_FAILED;
}
int32 readIndexData(char * filename,uint8 *buf,int32 *buflen)
{
	return   mrc_readFileFromMrpEx(NULL, filename, (uint8**)&buf, buflen, 0);
}

const uint8* GetgbBitmap(uint16 ch, uint16 font)
{
	int32 offset = GetOffSet(ch,FALSE);
	mrc_memset(Buf,0,FONT_DES);
	if(g_nowUse.isMemBuf == TRUE)
	{
		mrc_memcpy(Buf, g_nowUse.PointBuf + offset, FONT_DES);
	}
	else
	{
		mrc_seek(g_nowUse.fontHandle, offset, MR_SEEK_SET);
		mrc_read(g_nowUse.fontHandle, Buf, FONT_DES);
	}	
	return (uint8*)Buf;
}

const uint8* GetUniBitmap(uint16 ch, uint16 font)
{
	int32 offset = GetOffSet(ch,TRUE);
	if(offset == 0)
		return NULL;
	mrc_memset(Buf,0,FONT_DES);
	if(g_nowUse.isMemBuf == TRUE)
	{
		mrc_memcpy(Buf, g_nowUse.PointBuf + offset, FONT_DES);
	}
	else
	{
		mrc_seek(g_nowUse.fontHandle, offset, MR_SEEK_SET);
		mrc_read(g_nowUse.fontHandle, Buf, FONT_DES);
	}
	return (uint8*)Buf;
}

int32 _drawChar(char* chr, int16 x, int16 y, mr_colourSt colorst, E_CHR_SET flag, uint16 font)
{
	 uint16 chU = 0;
	 const uint8 *current_bitmap;
	 uint16 color=MAKERGB(colorst.r, colorst.g, colorst.b);
	 uint16 *screenBuf = (uint16 *)w_getScreenBuffer();
	 int8 index_I,index_J = 0;
	 uint16 totalPoint = 0;
	 uint16 totalIndex = 0;
        int32 temp = 0;
	  int32 X,Y;
	  mrc_memset(&g_nowUse,0,sizeof(T_POINT_INFO));
	  g_nowUse = GetSupportFont(font);

	 if(!chr)
 	{
 		return MR_FAILED;
 	}
	 if(x > (int16)screen_Size.width || y >(int16)screen_Size.height || x < 0 || y < 0)
 	{
 		return MR_FAILED;
 	}
	 switch(flag)
	{
		case MR_SET_UNICODE:
			chU = *chr;
			chU = chU << 8;
			chU += (uint8)*(chr+1);
			break;
		case MR_SET_GB:
			if((chU = *chr) > 0x80)
				{
				chU = chU << 8;
				chU += (uint8)*(chr+1);
				}
			break;
		default:
			return MR_FAILED;
	}

	if(chU == 0x20||chU == 0xa || chU == 0xd)
	{
		return	MR_SUCCESS;
	}
	switch(flag)
   	{
   		case MR_SET_UNICODE:
			current_bitmap = GetUniBitmap(chU,font);
			if(!current_bitmap)
			{
				return MR_FAILED;
			}
			g_nowUse.fontWidth = *current_bitmap;
			if((int16)g_nowUse.fontWidth + x > (int16)screen_Size.width)
				return MR_FAILED;
			g_nowUse.fontLen = *(current_bitmap + 1);
			current_bitmap += 2;
   			break;
		case MR_SET_GB:
			current_bitmap = GetgbBitmap(chU, font);
			if(!current_bitmap)
				{
				return MR_FAILED;
				}
			g_nowUse.fontWidth = (uint8)*current_bitmap;
			g_nowUse.fontLen = (uint8)*(current_bitmap + 1);
			current_bitmap += 2;
			break;
		default:
			return MR_FAILED;
   	}
	totalPoint = (uint16)(g_nowUse.fontHeight * g_nowUse.fontWidth);
       X = x;
	Y = y;
        for ( index_I = 0; index_I < g_nowUse.fontLen; index_I++)
        {
            temp = current_bitmap[index_I];
            for (index_J = 0; index_J < 8; index_J++)
            {
                totalIndex++;
                if (temp & 0x80)
                {
                    *(screenBuf + Y * screen_Size.width + X) = color;
                }
                X++;
                if ((totalIndex % g_nowUse.fontWidth) == 0)
                {
                    Y++;
                    X= x;
                }
                if (totalIndex >= totalPoint)
                    break;
                temp = temp << 1;
            }
        }
	return MR_SUCCESS;
}
int32 _drawTextLeftByLineN(char* pcText, int16 x, int16 y, mr_screenRectSt rect, mr_colourSt colorst, E_CHR_SET flag, uint16 font, int32 lineStart)
{
	if (!pcText)
	{
		return MR_FAILED;
	}
	if(useSysFontInfo)
		{
		mrc_drawTextLeft(pcText,  x,  y,rect, colorst, 3,font);
		return MR_SUCCESS;
		}

	{
		uint16 ch;
		uint8 a_ch[4];
		uint16* screenBuf = w_getScreenBuffer();
		const char *current_bitmap;
		uint8  *p=(uint8*)pcText;
		int16 chx=x,chy=y;
		int32 totalIndex,totalPoint,X1,Y1,index_I,index_J;
		uint8 temp = 0;
		uint16 color=MAKERGB(colorst.r, colorst.g, colorst.b);
		BOOL sign = FALSE;
		mrc_memset(&g_nowUse,0,sizeof(T_POINT_INFO));
		g_nowUse = GetSupportFont(font);
		a_ch[2]=a_ch[3]=0;
		switch(flag)
			{
			case MR_SET_UNICODE:
				ch = (uint16) ((*p<<8)+*(p+1));
				break;
			case MR_SET_GB:
				if((ch = *p)>0x80)
					{
					ch = ch <<8;
					ch += *(p+1);
					}
				break;
			default:
				return MR_FAILED;
			}//生成unicode/GB编码值

		while(ch)
		{
            if ((ch == 0x0a) || (ch == 0x0d))
			{//换行处理
			chy += (int16)(g_nowUse.fontHeight + INSERT_HEIGHT);
			chx = x;
			//p += 2;//移除后一个空格
			goto sign;
			}
		else if(ch == 0x20)
			{//空格处理
			chx +=8;
			if(chx >(rect.x + rect.w))
				{
				chx = x;
				chy +=  (int16)(g_nowUse.fontHeight+ INSERT_HEIGHT);
				}
			goto sign;
			}
		else
			{
			switch(flag)
				{
				case MR_SET_UNICODE:
					current_bitmap =(char*) GetUniBitmap(ch, font);
					if(!current_bitmap)
						{
						return MR_FAILED;
						}
					g_nowUse.fontWidth = *current_bitmap;
					g_nowUse.fontLen = *(current_bitmap+1);
					current_bitmap += 2;
					break;
				case MR_SET_GB:
					current_bitmap =(char*) GetgbBitmap(ch, font);
					if(!current_bitmap)
						{
						return MR_FAILED;
						}
					g_nowUse.fontWidth = *current_bitmap;
					g_nowUse.fontLen = *(current_bitmap+1);
					current_bitmap += 2;
					break;
				default:
					return MR_FAILED;
				}
			}
			   if((chx + (int16)g_nowUse.fontWidth) > (int16)(rect.x + rect.w))
					{
					chy += 	(int16)(g_nowUse.fontHeight + INSERT_HEIGHT);
					chx = x;
					}
			   if((chy-y)/g_nowUse.fontHeight < lineStart && !sign)
			   {
				   chx = chx + (int16)g_nowUse.fontWidth + INSERT_WIDHT;
				   goto sign;
			   }
			   else if(!sign)
			   {
					sign = TRUE;
					chy = y;
			   }
		       if(((chx+g_nowUse.fontWidth) <= (rect.x + rect.w)) && (chx >= rect.x) &&
					((chy+g_nowUse.fontHeight) <= (rect.y + rect.h)) && (chy >= rect.y) && chx >= 0 && chy >= 0)
				{
				totalPoint = g_nowUse.fontHeight * g_nowUse.fontWidth;
				X1 = Y1 = 0;
				totalIndex = 0;
			        for ( index_I = 0; index_I < g_nowUse.fontLen; index_I++)
			        	{
			            temp = current_bitmap[index_I];
			            for (index_J = 0; index_J < 8; index_J++)
			            		{
				                totalIndex++;
				                if (temp & 0x80)
				                	{
				                    *(screenBuf + (chy + Y1) * screen_Size.width + (chx + X1)) = color;
				                	}
				                X1++;
					     if(!g_nowUse.fontWidth)
							return MR_FAILED;
				                if ((totalIndex % g_nowUse.fontWidth) == 0)
				                	{
				                    Y1++;
				                    X1= 0;
				                	}
				                if (totalIndex >= totalPoint)
				                    break;
				                temp = temp << 1;
			            		}

					}
				}
		chx = chx + (int16)g_nowUse.fontWidth + INSERT_WIDHT;
		sign:
		if(ch <0x80 && !flag)
			{
			p+=1;
			}
		else
			{
			p+=2;
			}   //根据编码类型 设置偏移量
            if(flag == MR_SET_UNICODE)
			{
			ch = (uint16) ((*p<<8)+*(p+1));
			}
		else if(flag == MR_SET_GB)
			{
			if((ch = *p)>0x80)
				{
				ch = ch <<8;
				ch += *(p+1);
				}
			}//生成unicode/GB编码值
         };
	}
	return 0;
}

int32 _drawTextLeft(char* pcText, int16 x, int16 y, mr_screenRectSt rect, mr_colourSt colorst, E_CHR_SET flag, uint16 font)
{
	if (!pcText)
	{
		return MR_FAILED;
	}
	if(useSysFontInfo)
		{
		mrc_drawTextLeft(pcText,  x,  y,rect, colorst, 3,font);
		return MR_SUCCESS;
		}

	{
		uint16 ch;
		uint8 a_ch[4];
		uint16* screenBuf = w_getScreenBuffer();
		const char *current_bitmap;
		uint8  *p=(uint8*)pcText;
		int16 chx=x,chy=y;
		int32 totalIndex,totalPoint,X1,Y1,index_I,index_J;
		uint8 temp = 0;
		uint16 color=MAKERGB(colorst.r, colorst.g, colorst.b);
		mrc_memset(&g_nowUse,0,sizeof(T_POINT_INFO));
		g_nowUse = GetSupportFont(font);
		a_ch[2]=a_ch[3]=0;
		switch(flag)
			{
			case MR_SET_UNICODE:
				ch = (uint16) ((*p<<8)+*(p+1));
				break;
			case MR_SET_GB:
				if((ch = *p)>0x80)
					{
					ch = ch <<8;
					ch += *(p+1);
					}
				break;
			default:
				return MR_FAILED;
			}//生成unicode/GB编码值

		while(ch)
		{
            if ((ch == 0x0a) || (ch == 0x0d))
			{//换行处理
			chy += (int16)(g_nowUse.fontHeight + INSERT_HEIGHT);
			chx = x;
			//p += 2;//移除后一个空格
			goto sign;
			}
		else if(ch == 0x20)
			{//空格处理
			chx +=8;
			if(chx >(rect.x + rect.w))
				{
				chx = x;
				chy +=  (int16)(g_nowUse.fontHeight+ INSERT_HEIGHT);
				}
			goto sign;
			}
		else
			{
			switch(flag)
				{
				case MR_SET_UNICODE:
					current_bitmap =(char*) GetUniBitmap(ch, font);
					if(!current_bitmap)
						{
						return MR_FAILED;
						}
					g_nowUse.fontWidth = *current_bitmap;
					g_nowUse.fontLen = *(current_bitmap+1);
					current_bitmap += 2;
					break;
				case MR_SET_GB:
					current_bitmap =(char*) GetgbBitmap(ch, font);
					if(!current_bitmap)
						{
						return MR_FAILED;
						}
					g_nowUse.fontWidth = *current_bitmap;
					g_nowUse.fontLen = *(current_bitmap+1);
					current_bitmap += 2;
					break;
				default:
					return MR_FAILED;
				}
			}
			if((chx + (int16)g_nowUse.fontWidth) > (int16)(rect.x + rect.w))
				{
				chy += (int16)(g_nowUse.fontHeight + INSERT_HEIGHT);
				chx = x;
				}

		       if(((chx+g_nowUse.fontWidth) <= (rect.x + rect.w)) && (chx >= rect.x) &&
					((chy+g_nowUse.fontHeight) <= (rect.y + rect.h)) && (chy >= rect.y) && chx >= 0 && chy >= 0)
				{
				totalPoint = g_nowUse.fontHeight * g_nowUse.fontWidth;
				X1 = Y1 = 0;
				totalIndex = 0;
			        for ( index_I = 0; index_I < g_nowUse.fontLen; index_I++)
			        	{
			            temp = current_bitmap[index_I];
			            for (index_J = 0; index_J < 8; index_J++)
			            		{
				                totalIndex++;
				                if (temp & 0x80)
				                	{
				                    *(screenBuf + (chy + Y1) * screen_Size.width + (chx + X1)) = color;
				                	}
				                X1++;
					     if(!g_nowUse.fontWidth)
							return MR_FAILED;
				                if ((totalIndex % g_nowUse.fontWidth) == 0)
				                	{
				                    Y1++;
				                    X1= 0;
				                	}
				                if (totalIndex >= totalPoint)
				                    break;
				                temp = temp << 1;
			            		}

					}
				}
		chx = chx + (int16)g_nowUse.fontWidth + INSERT_WIDHT;
		sign:
		if(ch <0x80 && !flag)
			{
			p+=1;
			}
		else
			{
			p+=2;
			}   //根据编码类型 设置偏移量
            if(flag == MR_SET_UNICODE)
			{
			ch = (uint16) ((*p<<8)+*(p+1));
			}
		else if(flag == MR_SET_GB)
			{
			if((ch = *p)>0x80)
				{
				ch = ch <<8;
				ch += *(p+1);
				}
			}//生成unicode/GB编码值
         };
	}
	return 0;
}

int32 _drawText(char* chr, int16 x, int16 y,  mr_colourSt colorst, E_CHR_SET flag, uint16 font)
{
	if (!chr ||y < 0)
	{
		return MR_FAILED;
	}
	//不存在游戏自带字库，那么转到系统字库去。
	if(useSysFontInfo)
		{
		mrc_drawText(chr,x,y,colorst.r,colorst.g,colorst.b,flag,font);
		return MR_SUCCESS;
		}
	//存在游戏自带字库，使用自带字库
	{
		int totalPoint,totalIndex,index_I,index_J;
		uint16 *tempBuf= (uint16 *)chr;
		uint16 ch=0;
		uint16* screenBuf = w_getScreenBuffer();
		int32 temp_mr_screen_w = screen_Size.width;
		int32 X1,Y1,chx,chy;
		const uint8 *current_bitmap;
		uint8  *p=(uint8*)tempBuf;
		uint8 temp = 0;
		uint16 color=MAKERGB(colorst.r, colorst.g, colorst.b);
		mrc_memset(&g_nowUse,0,sizeof(T_POINT_INFO));
		g_nowUse = GetSupportFont(font);
		if(y+(int16)g_nowUse.fontHeight > (int16)screen_Size.height)
			return MR_FAILED;
		switch(flag)
			{
			case MR_SET_UNICODE:
				ch = (uint16) ((*p<<8)+*(p+1));
				break;
			case MR_SET_GB:
				if((ch = *p)>0x80)
					{
					ch =ch << 8;
					ch += *(p+1);
					}
				break;
			default:
				return MR_FAILED;
			}
		chx=x;
		chy=y;

		while(ch)
			{
			X1 = Y1 = 0;
			totalIndex = totalPoint = 0;
			if ((ch == 0x0a) || (ch == 0x0d))
				{//换行直接返回
				return MR_SUCCESS;
				}
			else if(ch == 0x20)
				{//空格则空格
				chx += 8;
				goto sign;
				}
			else
				{
				switch(flag)
					{
					case MR_SET_UNICODE:
						current_bitmap = GetUniBitmap(ch, font);
						if(!current_bitmap)
							{
							return MR_FAILED;
							}
						g_nowUse.fontWidth = *current_bitmap;
						g_nowUse.fontLen = *(current_bitmap+1);
						current_bitmap += 2;
						break;
					case MR_SET_GB:
						current_bitmap = GetgbBitmap(ch, font);
						if(!current_bitmap)
							{
							return MR_FAILED;
							}
						g_nowUse.fontWidth = *current_bitmap;
						g_nowUse.fontLen = *(current_bitmap+1);
						current_bitmap +=2;
						break;
					default:
						return MR_FAILED;
					}
				}
			if((chx+ g_nowUse.fontWidth ) >temp_mr_screen_w)
				return MR_SUCCESS;
			if(chx < 0)
				{
				chx =chx +  g_nowUse.fontWidth + INSERT_WIDHT;//字间距为 4
				goto sign;
				}
			totalPoint = g_nowUse.fontHeight * g_nowUse.fontWidth;
			totalIndex = 0;
		        for ( index_I = 0; index_I < g_nowUse.fontLen; index_I++)
		        {
		            temp = current_bitmap[index_I];
		            for (index_J = 0; index_J < 8; index_J++)
		            		{
			                totalIndex++;
			                if (temp & 0x80)
			                {
			                    *(screenBuf + (chy + Y1) * screen_Size.width + (chx + X1)) = color;
			                }
			                X1++;
				      if(!g_nowUse.fontWidth)
								return MR_FAILED;
			                if ((totalIndex % g_nowUse.fontWidth) == 0)
			                {
			                    Y1++;
			                    X1= 0;
			                }
			                if (totalIndex >= totalPoint)
			                    break;
			                temp = temp << 1;
		            		}
		        }
				chx =chx +  g_nowUse.fontWidth + INSERT_WIDHT;//字间距为 4
				sign:
				if((ch < 0x80) && (flag == MR_SET_GB))
					{
					p+= 1;
					}
				else
					{
					p+=2;
					}
				if(flag)
					{
					ch = (uint16) ((*p<<8)+*(p+1));
					}
				else
					{
					if((ch = *p)>0x80)
						{
						ch =ch << 8;
						ch +=*(p+1);
						}
					}
         			}
		}
	return MR_SUCCESS;
}




int32 _drawTextFromRight(char* pcText, int16 x, int16 y, mr_screenRectSt rect, mr_colourSt colorst,E_CHR_SET flag,  uint16 font)
{
	uint8 tempwidth = 0;
	if (!pcText)
	{
		return MR_FAILED;
	}
	if(useSysFontInfo)
		{
		mrc_drawTextRight(pcText,  x,  y,rect, colorst, flag,font);
		return MR_SUCCESS;
		}
	
	{
		uint16 ch;
		uint16* screenBuf = w_getScreenBuffer();
		const char *current_bitmap;
		uint8  *p=(uint8*)pcText;
		int16 chx = rect.x + rect.w - x, chy=y;      //X为rect区域右边框起始相对位置,应该再减去第一个字的宽度，所以用了
		int32 totalIndex,totalPoint,X1,Y1,index_I,index_J;		
		uint8 temp = 0;
		uint16 color=MAKERGB(colorst.r, colorst.g, colorst.b);		
		mrc_memset(&g_nowUse,0,sizeof(T_POINT_INFO));
		g_nowUse = GetSupportFont(font);
		switch(flag)
			{
			case MR_SET_UNICODE:
				ch = (uint16) ((*p<<8)+*(p+1));
				break;
			case MR_SET_GB:
				if((ch = *p)>0x80)
					{
					ch = ch <<8;
					ch += *(p+1);
					}
				break;
			default:
				return MR_FAILED;
			}//生成unicode/GB编码值
		
		while(ch)
		{
        	if((ch == 0x0a) || (ch == 0x0d))
			{//换行处理
			chy += (int16)(g_nowUse.fontHeight + INSERT_HEIGHT);
			chx = rect.x + rect.w;
			goto sign;	
			}
		else if(ch == 0x20)
			{//空格处理
			chx -=tempwidth;
			if(chx < rect.x)
				{
				chx = (int16)rect.x + (int16)rect.w;
				chy +=  (int16)(g_nowUse.fontHeight+ INSERT_HEIGHT);
				}
			goto sign;
			}
		else
			{
			switch(flag)
				{
				case MR_SET_UNICODE:
					current_bitmap =(char*) GetUniBitmap(ch, font);
					if(!current_bitmap)
						{
						return MR_FAILED;
						}
					g_nowUse.fontWidth = *current_bitmap;
					g_nowUse.fontLen = *(current_bitmap+1);
					current_bitmap += 2;
					break;
				case MR_SET_GB:
					current_bitmap =(char*) GetgbBitmap(ch, font);
					if(!current_bitmap)
						{
						return MR_FAILED;
						}
					g_nowUse.fontWidth = *current_bitmap;
					g_nowUse.fontLen = *(current_bitmap+1);
					current_bitmap += 2;
					break;
				default:
					return MR_FAILED;
				}
			}
			if((chx - (int16)g_nowUse.fontWidth) < (int16)rect.x)
				{
				chy += (int16)(g_nowUse.fontHeight + INSERT_HEIGHT);
				chx = (int16)rect.x + (int16)rect.w;			
				}
			chx -= (int16)g_nowUse.fontWidth;
		       if(( chx >= (int16)rect.x) && ((int16)(chy+g_nowUse.fontHeight) <= (int16)(rect.y + rect.h)) && (chy >= (int16)rect.y) && (chx >= 0) && (chy >= 0))
				{
				totalPoint = g_nowUse.fontHeight * g_nowUse.fontWidth;
				X1 = Y1 = 0;
				totalIndex = 0;
			        for ( index_I = 0; index_I < g_nowUse.fontLen; index_I++)
			        	{
			            temp = current_bitmap[index_I];
			            for (index_J = 0; index_J < 8; index_J++)
			            		{
				                totalIndex++;
				                if (temp & 0x80)
				                	{
				                    *(screenBuf + (chy + Y1) * screen_Size.width + (chx + X1)) = color;
				                	}
				                X1++;
					     if(!g_nowUse.fontWidth)
							return MR_FAILED;
				                if ((totalIndex % g_nowUse.fontWidth) == 0)
				                	{
				                    Y1++;
				                    X1= 0;
				                	}
				                if (totalIndex >= totalPoint)
				                    break;
				                temp = temp << 1;
			            		}

					}
				}
		chx = chx -INSERT_WIDHT;
		sign:
		if(ch <0x80 && !flag)
			{
			p+=1;
			}
		else
			{
			p+=2;
			}   //根据编码类型 设置偏移量         
            if(flag == MR_SET_UNICODE)
			{
			ch = (uint16) ((*p<<8)+*(p+1));
			}
		else if(flag == MR_SET_GB)
			{
			if((ch = *p)>0x80)
				{
				ch = ch <<8;
				ch += *(p+1);
				}
			}//生成unicode/GB编码值
         };
	}
	return MR_SUCCESS;
}

int32 _textWidthHeightLine(char *pcText,int32 *width,int32 *height,int32 *lines,E_CHR_SET flag, uint16 font, int32 linewidth)
{
	   uint16 chU;
	   int32 tempCount = 0;
	   uint8* tempChr = (uint8*)pcText;
	   uint8 *bmpPoint = NULL;
	   mrc_memset(&g_nowUse,0,sizeof(T_POINT_INFO));
	   g_nowUse = GetSupportFont( font);

	   if(!tempChr)
	   {
		   return MR_FAILED;
	   }
	   if(useSysFontInfo)
	   	{
//	   		mrc_textWidthHeight(pcText, flag, font,width, height);
			mrc_unicodeTextWidthHeight((uint16*)pcText, font, width, height);
			return MR_SUCCESS;
	   	}//取平台系统字库信息

	   *width = *height = *lines = 0;
	   switch(flag)
	   	{
	   	case MR_SET_UNICODE:
		   	chU = (uint16) ((*tempChr<<8)+*(tempChr+1));
			break;
	   	case MR_SET_GB:
			chU = *tempChr;
		   	if(chU>0x80)
		   		{
		   		chU = (uint16) ((*tempChr<<8)+*(tempChr+1));
		   		}
			break;
		default:
			return MR_FAILED;
	   	}//获取GB或UNICODE值
	   while(chU)
	   {
		  *height = g_nowUse.fontHeight;
		  if(chU == 0x20)
		  	{
		  	*width += 8;
			tempCount += 8;
			if(tempCount > linewidth)
				{
				*lines += 1;
				tempCount = 8;
				}
			goto NEXT;
		  	}
		  if(chU == 0xa || chU == 0xd)
		  {
				*lines += 1;
				tempCount = 0;
				tempChr +=  2;
				goto sign;				
		  }
		  switch(flag)
		  	{
		  	case MR_SET_UNICODE:
			   	bmpPoint = (uint8*)GetUniBitmap(chU,  font);
				if(!bmpPoint)
					{
					return MR_FAILED;
					}
				break;
		  	case MR_SET_GB:
		  		bmpPoint = (uint8*)GetgbBitmap(chU, font);
				if(!bmpPoint)
					{
					return MR_FAILED;
					}
				break;
			default:
				return MR_FAILED;
		  	}
		   *width += (*bmpPoint + INSERT_WIDHT);
			tempCount += (*bmpPoint + INSERT_WIDHT);
			if((tempCount- INSERT_WIDHT) > linewidth)
				{
				*lines += 1;
				tempCount = (*bmpPoint + INSERT_WIDHT);
				}
		   NEXT:
		   if(chU < 0x80 && flag==MR_SET_GB)
		   	{
		   	tempChr += 1;
		   	}
		   else
		   	{
		   	tempChr +=  2;
		   	}
			/*
		    if(*tempChr == '\0')
				return MR_SUCCESS;
			*/
			sign:
		    switch(flag)
		   	{
		   	case MR_SET_UNICODE:
		   		chU = (uint16) ((*tempChr<<8)+*(tempChr+1));
				break;
		   	case MR_SET_GB:
				chU = *tempChr;
				if(chU>0x80)
				{
					chU = (uint16) ((*tempChr<<8)+*(tempChr+1));
				}
				break;
			default:
				return MR_FAILED;
		   	}//获取GB或UNICODE值
	   }
	   *height += INSERT_HEIGHT;
	   *lines += 1;
	   return MR_SUCCESS;
}



int32 _textWidthHeight(char *pcText,int32 *width,int32 *height,E_CHR_SET flag, uint16 font)
{
	   uint16 chU;
	   uint8* tempChr = (uint8*)pcText;
	   uint8 *bmpPoint = NULL;
	mrc_memset(&g_nowUse,0,sizeof(T_POINT_INFO));

	   g_nowUse = GetSupportFont( font);

	   if(!tempChr)
	   {
		   return MR_FAILED;
	   }
	   if(useSysFontInfo)
	   	{
	//   		mrc_textWidthHeight(pcText, flag, font,width, height);
			mrc_unicodeTextWidthHeight((uint16*)pcText, font, width, height);
			return MR_SUCCESS;
	   	}//取平台系统字库信息
	   	
	   *width = *height = 0;
	   switch(flag)
	   	{
	   	case MR_SET_UNICODE:
		   	chU = (uint16) ((*tempChr<<8)+*(tempChr+1));
			break;
	   	case MR_SET_GB:
			chU = *tempChr;
		   	if(chU>0x80)
		   		{
		   		chU = (uint16) ((*tempChr<<8)+*(tempChr+1));
		   		}
			break;
		default:
			return MR_FAILED;
	   	}//获取GB或UNICODE值

		
	   while(chU)
	   {
		  *height = g_nowUse.fontHeight;
		  if(chU == 0x20)
		  	{
		  	*width += 8;
			goto NEXT;
		  	}
		  switch(flag)
		  	{
		  	case MR_SET_UNICODE:
			   	bmpPoint = (uint8*)GetUniBitmap(chU,  font);
				if(!bmpPoint)
					{
					return MR_FAILED;
					}
				break;
		  	case MR_SET_GB:
		  		bmpPoint = (uint8*)GetgbBitmap(chU, font);
				if(!bmpPoint)
					{
					return MR_FAILED;
					}
				break;
			default:
				return MR_FAILED;
		  	}
		   *width += (*bmpPoint + INSERT_WIDHT);

		   NEXT:
		   if(chU < 0x80 && flag==MR_SET_GB)
		   	{
		   	tempChr += 1;
		   	}
		   else
		   	{
		   	tempChr +=  2;
		   	}
			/*
		    if(*tempChr == '\0')
				return MR_SUCCESS;
			*/

		    switch(flag)
		   	{
		   	case MR_SET_UNICODE:
		   		chU = (uint16) ((*tempChr<<8)+*(tempChr+1));
				break;
		   	case MR_SET_GB:
				chU = *tempChr;
				if(chU>0x80)
				{
					chU = (uint16) ((*tempChr<<8)+*(tempChr+1));
				}
				break;
			default:
				return MR_FAILED;
		   	}//获取GB或UNICODE值
	   }
	   *height += INSERT_HEIGHT;
	   return MR_SUCCESS;
}

static void Font_FontReset(T_POINT_INFO *t_font)
{
	if(t_font->uIndexBuf)
		mrc_freeFileData(t_font->uIndexBuf, t_font->uIndexLen);
	if(t_font->bIndexBuf)
		mrc_freeFileData(t_font->bIndexBuf, t_font->bIndexLen);
	if(t_font->PointBuf)
		mrc_freeFileData(t_font->PointBuf, t_font->PointLen);
	if(t_font->fontHandle && (!t_font->isMemBuf))
	{
		mrc_close(t_font->fontHandle);
		t_font->fontHandle = 0;
	}
	mrc_memset(t_font, 0, sizeof(T_POINT_INFO));
}

static void Font_FontRead(T_POINT_INFO *t_font, E_FONT_TYPE type, int32 isMemLoad, int32 gbsupport, int32 unsupport)
{
	char tmp[64] = {0};
	
	if(t_font->uIndexBuf)
		mrc_freeFileData(t_font->uIndexBuf, t_font->uIndexLen);
	if(t_font->bIndexBuf)
		mrc_freeFileData(t_font->bIndexBuf, t_font->bIndexLen);
	if(t_font->PointBuf)
		mrc_freeFileData(t_font->PointBuf, t_font->PointLen);
	if(unsupport)
	{
		mrc_sprintf(tmp, FONT_UINDEX, type);
		mrc_readFileFromMrpEx(FONT_PATH,(char *)tmp,&(t_font->uIndexBuf),&(t_font->uIndexLen),0);
	}
	if(gbsupport)
	{
		mrc_sprintf(tmp, FONT_GBINDEX, type);
		mrc_readFileFromMrpEx(FONT_PATH,(char *)tmp,&(t_font->bIndexBuf),&(t_font->bIndexLen),0);
	}
	if(isMemLoad)
	{
		mrc_sprintf(tmp, FONT_POINT, type);
		mrc_readFileFromMrpEx(FONT_PATH,(char *)tmp,&(t_font->PointBuf),&(t_font->PointLen),0);
		if(t_font->PointBuf)
		{
			t_font->isMemBuf = TRUE;
			t_font->fontHeight = *t_font->PointBuf;
		}
	}
	else
	{
		char path[128] = {0};
		uint8 * buf = NULL;
		int32 bufLen = 0;
		mrc_sprintf(tmp, FONT_POINT, type);
		mrc_GetMrpInfo(mrc_getPackName(),MRP_FILENAME,(uint8*)path,128);
		if(MR_IS_DIR !=mrc_fileState((char *)path))
		{
			mrc_mkDir((char *)path);
			#if defined (__thumb)
	    		mrc_sleep(100);
			#endif
		}//生成存储路径
		
		mrc_strcat(path, "\\");
		mrc_strcat(path, tmp);
		if(MR_IS_FILE == mrc_fileState((char *)path))
		{//文件合法性检查,如果存在该文件就直接返回成功
			mrc_remove((char *)path);
		}
		buf=(uint8*)mrc_readFileFromMrp((char *)tmp, &bufLen, 0);
		if(buf)
		{
			t_font->fontHeight = *buf;
			t_font->fontHandle = mrc_open((char *)path,MR_FILE_CREATE|MR_FILE_WRONLY);
			if(t_font->fontHandle != 0)
			{
				mrc_write(t_font->fontHandle,buf,bufLen);
				mrc_close(t_font->fontHandle);
			}
			mrc_freeFileData(buf, bufLen);
			t_font->fontHandle = mrc_open((char *)path,MR_FILE_RDONLY);
		}
	}
	
}

static int32 checkFontSupport(int32 checkType)
{
	int32 ret_big = MR_FAILED, ret_medium = MR_FAILED, ret_small = MR_FAILED;
	switch(checkType)
	{
		case 1:
			if(g_big.uIndexBuf)
				ret_big = MR_SUCCESS;
			if(g_medium.uIndexBuf)
				ret_medium = MR_SUCCESS;
			if(g_small.uIndexBuf)
				ret_small = MR_SUCCESS;
			break;
		case 2:
			if(g_big.bIndexBuf)
				ret_big = MR_SUCCESS;
			if(g_medium.bIndexBuf)
				ret_medium = MR_SUCCESS;
			if(g_small.bIndexBuf)
				ret_small = MR_SUCCESS;
			break;
		case 3://MEMLOAD
			if(g_big.PointBuf)
				ret_big = MR_SUCCESS;
			if(g_medium.PointBuf)
				ret_medium = MR_SUCCESS;
			if(g_small.PointBuf)
				ret_small = MR_SUCCESS;
			break;
		case 4://TCARD
			if(g_big.fontHandle)
				ret_big = MR_SUCCESS;
			if(g_medium.fontHandle)
				ret_medium = MR_SUCCESS;
			if(g_small.fontHandle)
				ret_small = MR_SUCCESS;
			break;
		default:
			break;
	}
	if(MR_SUCCESS == ret_big)
	{
		g_big.isSupport = TRUE;
	}
	else
	{
		Font_FontReset(&g_big);
	}
	if(MR_SUCCESS == ret_medium)
	{
		g_medium.isSupport = TRUE;
	}
	else
	{
		Font_FontReset(&g_medium);
	}
	if(MR_SUCCESS == ret_small)
	{
		g_small.isSupport = TRUE;
	}
	else
	{
		Font_FontReset(&g_small);
	}
	return MR_SUCCESS;
}

int32 FontModuleInit(BOOL isUnicodeIndexSupport,BOOL isGBIndexSupport, BOOL isMemLoad)
{

	char path[100]={0};
	char tempForHeight[1]={0};

	mrc_getScreenInfo(&screen_Size);
	
	mrc_memset(&g_big,0,sizeof(T_POINT_INFO));
	mrc_memset(&g_medium,0,sizeof(T_POINT_INFO));
	mrc_memset(&g_small,0,sizeof(T_POINT_INFO));
	mrc_memset(&g_nowUse,0,sizeof(T_POINT_INFO));

	Font_FontRead(&g_big, FONT_TYPE_BIG, isMemLoad, isUnicodeIndexSupport, isGBIndexSupport);
	Font_FontRead(&g_medium, FONT_TYPE_MEDIUM, isMemLoad, isUnicodeIndexSupport, isGBIndexSupport);
	Font_FontRead(&g_small, FONT_TYPE_SAMLL, isMemLoad, isUnicodeIndexSupport, isGBIndexSupport);

	if(isUnicodeIndexSupport)
	{
		checkFontSupport(1);
	}
	if(isGBIndexSupport)
	{
		checkFontSupport(2);
	}
	if(isMemLoad)
	{
		checkFontSupport(3);
	}
	else
	{
		checkFontSupport(4);
	}
	
	if(!g_big.isSupport && !g_medium.isSupport && !g_small.isSupport)
	{
		useSysFontInfo = TRUE;
		FPRINTF("UseSysFont...");
	}
	return MR_SUCCESS;
}
void FontModuleRelease()
{
	mrc_printf("FontRelease B");
	Font_FontReset(&g_big);
	Font_FontReset(&g_medium);
	Font_FontReset(&g_small);
	mrc_printf("FontRelease E");
}
/*
int32 _drawTextAscii(int16 x,int16 y)
{
	   uint8 *buf,*bitmap;
	   int32  fileLen = 0;

	   int32 X1,Y1,X2,Y2,index;
	   uint16 color=MAKERGB(255, 0, 0);
	   uint16 *screenBuf = w_getScreenBuffer();
	   uint8 index_I ,index_J ,charValue= 0;

	   buf = bitmap = NULL;
	   if(mrc_readFileFromMrpEx(NULL,"ascii16.bin",(uint8 * *)&buf, &fileLen, 0) != MR_SUCCESS)
	   {
		   mrc_clearScreen(100,0,100);
		   mrc_refreshScreen(0,0, screen_Size.width, screen_Size.height);
		   return MR_FAILED;
	   }
	   else
	   {
		   char *test = "ascii.bin";
		   int32 handleF = 0;
		   handleF = mrc_open(test,MR_FILE_WRONLY|MR_FILE_CREATE);
		   if(handleF)
		   {
			   mrc_write(handleF,buf,fileLen);
			   mrc_close(handleF);
			   handleF = 0;
		   }
	   }
   	   bitmap = buf;
	   X1= x;
	   Y1 = y;
	   X2  = Y2 = 0;
	   for(index_I = 0; index_I < 128; index_I++)
	   {
		   for(index_J = 0; index_J <32; index_J++)
		   {
			   charValue =*bitmap++;
			   if(!charValue)
			   {
				   int32 iTemp = 0;
				   X2 += 8;
				   iTemp = (X2)/16;
				   if(iTemp)
				   {
					   Y2++;
				   }
				   X2 = X2 % 16;
			   }
			   else
			   {
				   for(index = 0; index < 8; index++)
				   {
					   if(charValue & 0x80)
					   {
						   screenBuf[(Y1+Y2)*screen_Size.width+ (X1+X2)] = color;
					   }
					   ++X2;
					   if(X2 == 16 )
					   {
						   ++Y2;
						   X2=0;
					   }
					   charValue <<=1;
				   }
			   }
		   }
		   X2 = Y2 = 0;
		   X1 += 16;
		   if(X1 > screen_Size.width)
		   {
			   X1 = 0;
			   Y1 +=16;
		   }

	   }
	   mrc_free(buf);
	   buf = NULL;
	   return MR_SUCCESS;
}
*/
