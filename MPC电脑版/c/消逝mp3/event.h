void kz(int p1)
{
if(p1==7||p1==9)
jm2=jm;
if(p1==7)
{ if(jm>1){jm--;j=1;}    else jm=3;
}

else if(p1==9)
{ if(jm<3){jm++;j=1;} 
  else jm=1;
}
if(p1==7||p1==9)pypy(p1);

switch(jm)
{
case 1:mp3_1();break;
case 2:mp3_2();break;
case 3:lrc();break;
case 4:list();break;
case 5:set();break;
case 6:help();break;
case 7:gy();break;
case 8:yesexit();break;
}

REF;

}

//换歌
void HG(int p1)
{
 int t;
 if(p1 != 4||p1 != 6)t=1;
 
 if(p1==4)
 {if(y>0)y--;else y=HowLen;}
 
 if(p1==6)
 {if(y<HowLen)y++;else y=0;}
 
if(t)
{
 char *name=malloc(200);
 sprintf(name,"%s/%s",PATH,FileListHead+y*NAMELEN);
 MP3(6," ",v);
 MP3(2,name,v);
 MP3(3," ",v);
 MP3(8," ",v);
 free(name);
 paly=1;
 mp3Head=y;
 kz(0);}
}

void hpf()
{
if(pf==1)
{img(day,0,0);
memcpy(bei,scr,SCRW*SCRH*2);
r=g=b=0;
}
else if(pf==2)
{img(night,0,0);
memcpy(bei,scr,SCRW*SCRH*2);
r=g=b=210;
}

}


//界面平移！勿动！
void pypy(int p1)
{

switch(jm)
{
case 1:mp3_1();break;
case 2:mp3_2();break;
case 3:lrc();break;
}
memcpy(bei,scr,SCRW*SCRH*2);
/*
switch(jm2)
{
case 1:mp3_1();break;
case 2:mp3_2();break;
case 3:lrc();break;
}
*/

effsetcon(0,0,SCRW,SCRH,138,138,138);

int px;
if(p1==7)
for(px==tw*2;px<=SCRW;px+=PV)
{
bmpshowflip(bei,px-SCRW,0,SCRW,SCRW,SCRH,BM_COPY,0,0,0);
ref(px-SCRW,0,SCRW,SCRH);
}

if(p1==9)
for(px=SCRW-tw*2;px>=0;px-=PV)
{
bmpshowflip(bei,px,0,SCRW,SCRW,SCRH,BM_COPY,0,0,0);
ref(px,0,SCRW,SCRH);

}
hpf();
}

int event(int type,int p1,int p2)
{
 if(type==KY_DOWN)
switch(p1)
{ case _STAR:
 exitapp();break;

case _0:
if(pf==1)pf=2;
else pf=1;
hpf();kz(0);break;

case _POUND:
printscr();break;
}


switch(jm)
{
case 1:
kzevent(type,p1,p2);
break;

case 2:
mp3event(type,p1,p2);
break;
case 3:
lrcevent(type,p1,p2);
break;

case 4:
listevent(type,p1,p2);
break;

case 5:
setevent(type,p1,p2);
break;

case 6:
helpevent(type,p1,p2);
break;

case 7:
gyevent(type,p1,p2);
break;

case 8:
yesevent(type,p1,p2);
break;
}

return 0;
}

void kzevent(int type,int p1,int p2)
{
if(type==KY_DOWN)
switch(p1)
{
 case _SRIGHT:
jm=8;kz(0);break;

case _UP:
if(j>1)j--;
else j=5;
kz(0);
break;

case _DOWN:
if(j<5)j++;
else j=1;kz(0);
break;

case _SLEFT:
jm=j+3;j=1;kz(0);
break;

case _7:case _9:
kz(p1);break;

}

}

void mp3event(int type,int p1,int p2)
{
if(type==KY_DOWN)
switch(p1)
{
case _SRIGHT:
jm=8;kz(0);break;

case _SELECT:
case _SLEFT:
if(paly)
{paly--;MP3(4,"",v);}
else 
{paly++;MP3(5,"",v);}
kz(p1);
break;

case _7:case _9:
kz(p1);break;

case _UP:
if(paly)
{ if(v<5)v++;MP3(8,"",v);}
break;

case _DOWN:
if(paly)
{ if(v>0)v--;MP3(8,"",v);}
break;

case _4:case _6:
HG(p1);
kz(0);break;

}

}
//设置界面长按(300ms/次)
void setchangan(int data)
{
if(data==_DOWN)
{if(j<7)j++;else j=1;}
else 
{if(j>1)j--;else j=7;}
kz(0);
}


void lrcevent(int type,int p1,int p2)
{
if(type==KY_DOWN)
switch(p1)
{
case _SRIGHT:
jm=8;kz(0);break;
case _7:case _9:
kz(p1);
break;

}

}
void setevent(int type,int p1,int p2)
{
if(type==KY_DOWN)
switch(p1)
{
 case _UP:
 case _DOWN:
 setchangan(p1);
 anf=timercreate();
 timerstart(anf,SHU,p1,"setchangan",1);

 break;

case _SLEFT:
if(j==1)
{loop=1;
MP3(6,"",v);MP3(3,"",v);
}
else if(j==2)
{ loop=0;
  MP3(6,"",v);MP3(3,"",v);
}
else if(j==3)
{loop=2;
 MP3(6,"",v);MP3(3,"",v);
}
else if(j==4)
exitpaly=1;
else if(j==5)
exitpaly=0;

else if(j==6)
{ pf=1;hpf(); }

else if(j==7)
{pf=2; hpf(); }
kz(0);
break;

case _SRIGHT:
if(anf)
{timrdel(anf);anf=NULL;}
j=jm=1;kz(0);break;

}

else if(type==KY_UP)
{
if(p1==_UP||p1==_DOWN)
{
if(anf)
{timerdel(anf);anf=NULL;}
}

}

}

void helpevent(int type,int p1,int p2)
{
if(KY_DOWN==type&&p1==_SRIGHT)
{jm=1;kz(0);}
}

void gyevent(int type,int p1,int p2)
{helpevent(type,p1,p2);}

void yesevent(int type,int p1,int p2)
{
if(KY_DOWN==type)
switch(p1)
{
case _SRIGHT:
jm=1;kz(0);break;
case _SLEFT:
exitapp();break;
}
}
