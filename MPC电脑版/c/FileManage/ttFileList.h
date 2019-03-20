/*
* ttFileList.h
* 文件列表封装
* By:天天
* QQ:643552878
* Date:2012.8.5
*/

/*文件名最大长度*/
#define MAXNAME 200

/*最大文件数*/
#define MAXFILE 500

/*文件结构体*/
typedef struct filepoint
{
 struct filepoint *prev;//前1个
 struct filepoint *next;//后1个
 char *name;//文件名
 uint8 type;//文件类型
 uint32 len;//文件长度
}*FilePoint;

/*文件结构体长度*/
uint32 FilePointLen=sizeof(struct filepoint);

/*文件列表结构体*/
typedef struct filelist
{
 FilePoint first;//列表开头
 FilePoint last;//列表结尾
 FilePoint now;//焦点所在
 char *path;//当前目录
 uint32 FileN;//文件数
 uint32 NowN;//焦点编码
}*FileList;

/*文件列表结构体长度*/
uint32 FileListLen=sizeof(struct filelist);


/*
* 创建一个文件列表
* 返回:
* 成功:列表结构指针 失败:NULL
*/
void* FileListCreate()
{
 FileList list=(struct filelist*)malloc(FileListLen);
 if(list==NULL)
 return NULL;
 memset(list,NULL,FileListLen);
 list->path="";
 return list;
}

/*
* 删除文件列表
* list: 文件结构体指针
*/
void FileListDel(FileList list)
{
 FilePointAllDel(list->first); 
 free(list->path);
 free(list);
 list=NULL;
}

/*
* 创建一个文件结点
* 返回:
* 成功:结点指针 失败:NULL
*/
void* FilePointCreate()
{
 FilePoint one=(struct filepoint*)malloc(FilePointLen);
 if(one==NULL)
 return NULL;
 memset(one,NULL,FilePointLen);
 return one;
}

/*删除一个文件结点*/
void FilePointDel(FilePoint one)
{
 free(one->name);
 free(one);
 one=NULL;
}

/*删除所有文件结点*/
void FilePointAllDel(FilePoint first)
{
 FilePoint now=first;
 while(now->next!=NULL)
 {
  now=now->next;
  FilePointDel(now->prev);
 }
}

/*
* 文件搜索
* list:搜索文件列表 
*/
void FindPath(FileList list)
{
 if(list==NULL)
 return;

 list->first=FilePointCreate();
 FilePoint to=list->first;
 list->FileN=0;

 char *buffer=(char*)malloc(MAXNAME+FilePointLen);

 uint32 f=findstart(list->path,buffer,MAXNAME);
 findnext(f,buffer,MAXNAME);

 while(list->FileN<=MAXFILE)
 {
 memset(buffer,NULL,MAXNAME);
 if(-1==findnext(f,buffer,MAXNAME))
 break;

//新增1个结点
 FilePoint one=FilePointCreate();

//搜索结果写入键表
 uint32 len=strlen(buffer)+FilePointLen;
 one->name=malloc(len);
 strncpy(one->name,buffer,len);
//获取文件类型、长度
 char *str=(char*)malloc(strlen(list->path)+strlen(one->name)+1);
 sprintf(str,"%s/%s",list->path,one->name);
 one->type=GetFileKind(str);
 if(one->type!=Is_Dir)
 one->len=getlen(str);
 else
 one->len=strlen(str);
 free(str);

//连接链表
 one->prev=to;
 to->next=one;
 to=to->next;
 list->FileN++;
 }
 findstop(f);
 free(buffer);

 list->first=list->first->next;
 FilePointDel(list->first->prev);
 list->last=to;
 list->now=list->first;
 list->NowN=1;
}


//获取文件类型字符串，type:文件类型
char* GetFileKindStr(uint8 type)
{
 char *FileKindStr[7]={"未知类型","文件夹","压缩数据", "音乐","图片","文本","软件"};
 return FileKindStr[type];
}

//^
enum
{
 Is_Not=0,
 Is_Dir,
 Is_Zip,
 Is_Music,
 Is_Photo,
 Is_Book,
 Is_Mrp
};

//获取文件类型
int GetFileKind(char *Name)
{
//return filestate(Name);

 char *str;
 if(filestate(Name)==IS_DIR)
 return Is_Dir;
 str=strrchr(Name,'.');
 if(str==NULL)
 return Is_Not;
 switch(0)
 {
 case strcmp(str,".jpg"):
 case strcmp(str,".png"):
 case strcmp(str,".gif"):
 case strcmp(str,".bmp"):
 return Is_Photo;
 break;

 case strcmp(str,".mp3"):
 case strcmp(str,".wav"):
 case strcmp(str,".amr"):
 case strcmp(str,".mid"):
 return Is_Music;
 break;

 case strcmp(str,".txt"):
 case strcmp(str,".c"):
 case strcmp(str,".h"):
 case strcmp(str,".lrc"):
 case strcmp(str,".wml"):
 return Is_Book;
 break;

 case strcmp(str,".zip"):
 case strcmp(str,".jra"):
 case strcmp(str,".gz"):
 return Is_Zip;
 break;

 case strcmp(str,".mrp"):
 return Is_Mrp;
 break;
 }
 return Is_Not;
}

//By:天天