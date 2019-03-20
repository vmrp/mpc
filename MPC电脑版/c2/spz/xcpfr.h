//表达式求值
#include "c2/spz/stack.h"

void intowrxcp(char *xcp)
{
int in,n,num,k=1,a;
stack[0][0]=stack[0][1]=stack[0][2]=0;
for(;n<strlen(xcp);n++)
if(('0'<=xcp[n])&&(xcp[n]<='9')){
in=1;
num=num*10+xcp[n]-48;}
else {
if(in==1){
in=0;
push(num,0);
num=0;}
if(xcp[n]=='(')push('(',2);
else if(xcp[n]==')')while((a=put(2))!='('){
push(a,0);
push(stack[0][0],1);}
else while(k){
a=stack[stack[0][2]][2];
if((((xcp[n]=='*')||(xcp[n]=='/'))&&((a=='+')||(a=='-'))||(a=='('))||(!stack[0][2])){
push(xcp[n],2);
k=0;}
else {
push(put(2),0);
push(stack[0][0],1);}
}
k=1;
}
if(in==1)push(num,0);
while(stack[0][2]>0){
push(put(2),0);
push(stack[0][0],1);}
gbxcp();
}

int gbxcp()
{
int num,t;
stack[max][0]=stack[max][1]=stack[max][2]=1;
stack[0][2]=0;
while(stack[max][0]<=stack[0][0])
if(stack[stack[max][1]][1]==stack[max][0]){
stack[max][1]++;
num=put(2);
switch(get(0)){
case '+':
push(num+put(2),2);
break;
case '-':
push(put(2)-num,2);
break;
case '*':
push(num*put(2),2);
break;
case '/':
push(put(2)/num,2);
break;}
}
else push(t=get(0),2);
return put(2);
}

