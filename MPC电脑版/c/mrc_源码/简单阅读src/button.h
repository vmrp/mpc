#ifndef BUTTON_H
#define BUTTON_H
//龙魂噬心(6369)
//2012-7-17


#define BUTTONFOCUSCLR 0x0055ff
#define BUTTON

typedef struct
{
int x;int y;int w;int h;
int32 upclr;
int32 downclr;
int32 titclr;
char* title;
int is_getfocus;
}Button,*PButton;

/*创建按纽*/

PButton buttonCreate(int x,int y,int w,int h,int32 upclr,int32 downclr,int32 titclr,char* title);



/*删除按钮*/

void buttonDel(PButton a);



/*设置按纽得到焦点*/
void setButtonFocus(PButton a);


/*设置按纽失去焦点*/
void setButtonClearFocus(PButton a);


/*按钮获得焦点*/

void buttonGetFocus(PButton a);



/*按钮被单击*/
void buttonClick(PButton a);



/*按钮被释放*/
void buttonReleace(PButton a);


/*绘制按钮*/
void drawButton(PButton a);


#endif
