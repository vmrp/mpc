

#define BigNameLen 250
//搜索文件名最大长度

char *FileKind[6]={"图片","音乐","文本", "压缩数据","应用软件","未知类型"};

enum
{
 KIND_PHOTO=0,
 KIND_MUSIC,
 KIND_BOOK,
 KIND_ZIP,
 KIND_MRP,
 KIND_NOTKNOW
};

//获取文件类型
uint32 GetFileKind(char *Name)
{
 char *str=strrchr(Name,'.');
 if(str==NULL)
 return KIND_NOTKNOW;
 switch(0)
 {
 case strcmp(str,".jpg"):
 case strcmp(str,".png"):
 case strcmp(str,".gif"):
 case strcmp(str,".bmp"):
 return KIND_PHOTO;
 break;
 case strcmp(str,".mp3"):
 case strcmp(str,"mid"):
 return KIND_MUSIC;
 break;
 case strcmp(str,".txt"):
 case strcmp(str,".c"):
 case strcmp(str,".h"):
 case strcmp(str,".lrc"):
 case strcmp(str,".wml"):
 return KIND_BOOK;
 break;
 case strcmp(str,".zip"):
 case strcmp(str,".jra"):
 case strcmp(str,".gz"):
 return KIND_ZIP;
 break;
 case strcmp(str,".mrp"):
 return KIND_MRP;
 break;
 }
 return KIND_NOTKNOW;
}


//文件链表结构体
;typedef struct listst
{
 struct listst *last;//上个
 struct listst *next;//下个
 char *name;//文件名
 uint8 mode;//文件类型
}LISTST;

uint32 ListLen=sizeof(LISTST);

LISTST
*First,//开头
*End;//结尾

struct
{
 LISTST *now;//焦点所在
 char *dir;//当前目录
 LISTST *one;//
 uint32 onenum;//
 uint32 num;//当前文件数
 uint32 all;//总文件数
 uint32 timer;//长按定时器句柄
}Focus;

Focus.dir="";
Focus.timer=timercreate();


//链表初始化
void ListStart()
{
 if(First!=NULL)
 {
  free(First->name);
  free(First);
 }
 First=malloc(ListLen);
 memset(First,NULL,ListLen);
}


//文件搜索，建立链表
void FindFile()
{
 DrawMsg("搜索中...");

 Focus.all=0;
 ListStart();

 LISTST *to=First;
 char *buffer=malloc(BigNameLen);

 int len=0,f=findstart(Focus.dir,buffer,BigNameLen);

 int ret=findnext(f,buffer,BigNameLen);
 if(ret!=-1)
 while(1)
 {
 memset(buffer,NULL,BigNameLen);
 if(findnext(f,buffer,BigNameLen)==-1)
 break;
 LISTST *one=malloc(ListLen);
 memset(one,NULL,ListLen);
 one->last=to;
 one->next=NULL;
 int l=strlen(buffer)+ListLen;
 one->name=malloc(l);
 memset(one->name,NULL,l);
 strncpy(one->name,buffer,l);

 char *ptr=FileName(Focus.dir,one->name);
 one->mode=filestate(ptr);
 free(ptr);
 // printf("%s\n",one->name);

 to->next=one;
 to=to->next;
 Focus.all++;
 }
 if(Focus.all>0)
 {
 First=First->next;
 free(First->last);

 End=to->last->next;
 Focus.now=First;
 Focus.num=1;
 }
 else
 {
 DrawMsg("空文件夹");
 sleep(300);
 event(KY_DOWN,_SRIGHT,NULL);
 }
 findstop(f);
 free(buffer);
 Jm=Jm_LIST;
}

//删除文件链表
void DelAll()
{
 LISTST *to=First;

 while(to->next!=NULL)
 {
 to=to->next;
 free(to->last->name);
 free(to->last);
 to->last=NULL; }

}

//输出当前文件列表
void PrintfFileList()
{
 LISTST *to=First;
 printf("\n\n当前目录:%s\n",Focus.dir);

 while(to->next!=NULL)
 {
 printf("%s\n",to->name);
 to=to->next;
 }

}
