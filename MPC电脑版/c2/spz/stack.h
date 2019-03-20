//栈,表达式处理用
#define max 50
int stack[max+1][3];

int put(int n)
{
if(stack[0][n]>0)return stack[stack[0][n]--][n];
else pt();
}

int get(int n)
{
if(stack[max][n]<=stack[0][n])return stack[stack[max][n]++][n];
else pt();
}

int push(int k,int n)
{
if(stack[0][n]<max){
stack[++stack[0][n]][n]=k;
return 0;}
else pt();
}

void pt()
{
rst.x=20;
print("数据出现问题，可能是由于表达式出错，由于本人异常懒惰，同时为了方便您，12秒后强制退出，请快速检查表达式",4000);
print(st,6000);   
print("欢迎使用，拜",2500);
exitApp();
}