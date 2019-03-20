/*
* 文件操作函数
* By:天天
* QQ:643552878
* Date:2012.8.20
*/

/*打开文件，返回文件内容字符串指针，申请的内存需自己释放*/
void *FileOpen(char *FileName)
{
 uint32 f,len;
 len=getlen(FileName);
 void *Buffer=(void*)malloc(len);
 f=open(FileName,1);
 read(f,Buffer,len);
 return Buffer;
}


//每次操作数据传递速度大小,单位:字节
#define MAXLEN 10240


/*将FileName复制到FileName2
返回:
成功 >0 失败-1(下同)
*/
int FileCopy(char *FileName,char *FileName2)
{
 if(getlen(FileName)<=0)
 return -1;
 return AddFileTo(FileName,FileName2);
}
/*将FileName复制到FileName2结尾,若FileName2不存在则创建之.
*/
int AddFileTo(char *FileName,char *FileName2)
{
 uint32 Filef,Filef2,
NowLen=0,//复制进度
FileLen=getlen(FileName);//FileName文件长度

 if(FileLen<=0)
 return -1;
 Filef=open(FileName,1);
 Filef2=open(FileName2,8);

 seek(Filef2,0,SEEK_END);
 void *Buffer=(void*)malloc(MAXLEN);

 while(1)
 {
 memset(Buffer,NULL,MAXLEN);
 seek(Filef,NowLen,SEEK_SET);
 uint32 ReadLen=read(Filef,Buffer,MAXLEN); 
 write(Filef2,Buffer,ReadLen);
 ;
 NowLen+=ReadLen;
 if(ReadLen<MAXLEN)
 break;
 }
 free(Buffer);
 close(Filef);
 return close(Filef2);
}

//By:天天