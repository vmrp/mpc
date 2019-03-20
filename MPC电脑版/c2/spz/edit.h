//±à¼­¿ò
#define LG 180
#define STS 100
int *KEDY,NTD;

void PRIN(int X,int Y,int W,int H)
{
char *B,C;
int XD;
P(0);
drect(X,Y,W,H,255,255,255);
if(KEDY[6]>W-15)X+=W-15-KEDY[6];
if(KEDY[9]){
C=*((char*)KEDY+NTD-1);
textwh(&C,0,2,KEDY+7,KEDY+8);
drect(X+KEDY[6]-KEDY[7],Y,KEDY[7],KEDY[4],150,350,290);}
if(*(B=(char*)KEDY+STS))
dtext(B,X,Y,0,0,0,0,2);
}

void CK()
{
timerstart(*KEDY,80,0,"P",1);
}

void P(int MD)
{
char *B=(char*)KEDY+NTD;
int X;
KEDY[5]++;
if(KEDY[9]>0){
KEDY[9]--;
if(!KEDY[9])PRIN(KEDY[1],KEDY[2],KEDY[3],KEDY[4]);}
textwh(B,0,2,KEDY+6,KEDY+8);
textwh((char*)KEDY+STS,0,2,KEDY+7,KEDY+8);
if((KEDY[6]=KEDY[7]-KEDY[6])<=KEDY[3]-15)
X=KEDY[1]+KEDY[6];
else X=KEDY[1]+KEDY[3]-15;
if(!((KEDY[5]%10)<5))drect(X-1,KEDY[2],2,KEDY[4],0,0,0);
else drect(X-1,KEDY[2],2,KEDY[4],255,255,255);
ref(KEDY[1],KEDY[2],KEDY[3],KEDY[4]);
}

void MODE()
{
int X=KEDY[1]+KEDY[3]+4,Y=KEDY[2]-4;
char A;
if(KEDY[11]==0)A='0';
else if(KEDY[11]==1)A='A';
else A='a';
drect(X,Y,15,KEDY[4]+8,100,150,200);
dtext(&A,X,Y+4,255,255,255,0,2);
ref(X,Y,20,KEDY[4]+8);
}

void SP()
{
char *M=(char*)KEDY+--NTD;
memmove(M,M+1,strlen(M+1)+1);
}

void GET(char L)
{
char *M=(char*)KEDY+NTD;
if(NTD<LG){
NTD++;
memmove(M+1,M,strlen(M));
M[0]=L;}
}

void startedit(int X,int Y,int W,char *B)
{
int H;
lcd(0);
KEDY=(int*)malloc(LG+1);
textwh("H",0,2,KEDY,&H);
H+=8;
memset(KEDY,0,LG+1);
memmove((char*)KEDY+STS,B,strlen(B));
KEDY[13]=(char*)malloc(SCRW*2*H)-(char*)KEDY;
memmove((char*)KEDY+KEDY[13],getscrbuf()+Y*SCRW,SCRW*2*H);
KEDY[0]=timercreate();
KEDY[1]=X+4;
KEDY[2]=Y+4;
KEDY[3]=W-8;
KEDY[4]=H-8;
KEDY[11]=1;
NTD=STS;
CK();
drect(X,Y,W,H,100,200,150);
ref(X,Y,W,H);
MODE();
PRIN(KEDY[1],KEDY[2],KEDY[3],KEDY[4]);
}

char* endedit()
{
char *D;
if(KEDY!=NULL){
D=(char*)malloc(strlen((char*)KEDY+STS)+1);
memmove(D,(char*)KEDY+STS,strlen((char*)KEDY+STS)+1);
memmove(getscrbuf()+(KEDY[2]-4)*SCRW,(char*)KEDY+KEDY[13],(KEDY[4]+8)*SCRW*2);
free((char*)KEDY+KEDY[13]);
timerdel(*KEDY);
free(KEDY);
KEDY=NULL;
ref(0,0,SCRW,SCRH);}
return D;
}

void play(int P)
{
char *B=(char*)KEDY;
int Q;
if(KEDY!=NULL){
if((P==_LEFT)&&(NTD>STS))NTD--;
else if((P==_RIGHT)&&(B[NTD]))NTD++;
else if((P==_SRIGHT)&&(B[NTD-1]))SP();
else if(P==_POUND){
KEDY[11]++;KEDY[11]%=3;
MODE();}
if(P<11){
if(KEDY[11]==0){
if(P-10){
KEDY[9]=0;GET(P+48);}
else {
if(KEDY[9]&&P)SP();
if(KEDY[9])
switch(KEDY[12]){
case '+':
KEDY[12]='-';break;
case '-':
KEDY[12]='*';break;
case '*':
KEDY[12]='/';break;
case '/':
KEDY[12]='(';break;
case '(':
KEDY[12]=')';break;
case ')':
KEDY[12]='+';break;}
else KEDY[12]='+';
GET(KEDY[12]);
KEDY[9]=10;
}
}
else { 
if(KEDY[10]==P){
if(KEDY[9]&&P)SP();
if(P==_7||P==_9)KEDY[12]=++KEDY[12]%4;
else if(P==_1)KEDY[12]=++KEDY[12]%15;
else if(P==_STAR){
KEDY[12]++;
if(KEDY[12]==5)KEDY[12]=32;
if(KEDY[12]==36)KEDY[12]=0;
}
else KEDY[12]=++KEDY[12]%3;
}
else KEDY[12]=0;
KEDY[9]=10;
if(KEDY[11]==1)Q=65;
else Q=97;
switch(P){
case 2:
GET(Q+KEDY[12]);
break;
case 3:
GET(Q+3+KEDY[12]);
break;
case 4:
GET(Q+6+KEDY[12]);
break;
case 5:
GET(Q+9+KEDY[12]);
break;
case 6:
GET(Q+12+KEDY[12]);
break;
case 7:
GET(Q+15+KEDY[12]);
break;
case 8:
GET(Q+19+KEDY[12]);
break;
case 9:
GET(Q+22+KEDY[12]);
break;
case _0:
GET(' ');
break;
case _1:
GET(33+KEDY[12]);
break;
case _STAR:
GET(91+KEDY[12]);
break;
}
}
}
else KEDY[9]=0;
KEDY[10]=P;
PRIN(KEDY[1],KEDY[2],KEDY[3],KEDY[4]);}
}