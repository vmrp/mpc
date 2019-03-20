/*贪吃蛇*/
/*by eleqian*/

/*
按键：
上下左右：改变方向
左软键：开始/暂停
右软键：退出
 */

#include "base.h"
/*全局变量*/
int arr[50];
int linc, colc, xside, yside;
int food, last, slen, maxl, state;
int32 timer;

int init()
{
    maxl = 50;

    xside = (SCRW % 10) / 2;
    if(0 == xside)
    {
        xside = 10;
    }
    xside = xside + 10;
    yside = (SCRH % 10) / 2;
    if(0 == yside)
    {
        yside = 10;
    }
    yside = yside + 10;

    colc = (SCRW - xside * 2) / 10;
    linc = (SCRH - yside * 2) / 10;
    timer=timercreate();
    inits();
    return 0;
}

void inits()
{
    int m;

    m = (linc / 2) *colc + colc / 2;
    arr[0]=m;
    arr[1]= m + colc;
    arr[2]= m + colc * 2;

    slen = 3;
    state = 0;
    last = _UP;

    dform();
    dsnake(slen);
    dfood();
}

void go(int s)
{
    int j, l;

    l = arr[slen - 1];
    for(j = slen - 1; j > 0; j = j - 1)
    {
        arr[j]= arr[j - 1];
    }
    arr[0]= arr[0] + s;
    if(0 != isself())
    {
        gameover();
    }

    eat(l);
    dsnake(2);
}

void eat(int l)
{
    if(arr[0] == food)
    {

        if(slen < maxl)
        {
            arr[slen]=l;
            slen = slen + 1;
        }
        else
        {
            dclub(l, 0, 0, 0);
        }
        dfood();

    }
    else
    {
        dclub(l, 0, 0, 0);
    }
}

int isself()
{
    int i, j, r;

    i = arr[0];
    r = 0;

    for(j = 1; j < slen; j = j + 1)
    {
        if(arr[j] == i)
        {
            r = 1;
            j = slen;
        }
    }

    return r;
}

int isate()
{
    int i, r;

    r = 0;

    for(i = 0; i < slen; i = i + 1)
    {
        if(arr[i] == food)
        {
            r = 1;
            i = slen;
        }
    }

    return r;
}

void dform()
{
    cls(0, 0, 0);
    drect(xside - 2, 0, 2, SCRH, 255, 255, 255);
    drect(SCRW - xside, 0, 2, SCRH, 255, 255, 255);
    drect(0, yside - 2, SCRW, 2, 255, 255, 255);
    drect(0, SCRH - yside, SCRW, 2, 255, 255, 255);
    ref(0, 0, SCRW, SCRH);
}

void dfood()
{
    sand(getuptime());
    do
    {
        food = rand() % (colc *linc);
    }
    while(0 != isate());

    dclub(food, 255, 0, 0);
}

void dsnake(int dl)
{
    int i;

    dclub(arr[0], 0, 255, 255);

    for(i = 1; i < dl; i = i + 1)
    {
        dclub(arr[i], 0, 255, 0);
    }
}


void dclub(int i, int r, int g, int b)
{
    int x, y;

    x = 10 *(i % colc) + xside;
    y = 10 *(i / colc) + yside;
    drect(x, y, 10, 10, r / 2, g / 2, b / 2);
    drect(x + 1, y + 1, 8, 8, r, g, b);
    ref(x, y, 10, 10);
}

void gameover()
{
    stop();
    inits();
}

void cd(int k)
{
    if(k == _LEFT)
    {
        if(last != _RIGHT)
        {
            last = k;
        }
    }
    if(k == _RIGHT)
    {
        if(last != _LEFT)
        {
            last = k;
        }
    }
    if(k == _UP)
    {
        if(last != _DOWN)
        {
            last = k;
        }
    }
    if(k == _DOWN)
    {
        if(last != _UP)
        {
            last = k;
        }
    }

    if(last == k)
    {
        stop();
        _timer(0);
        start();
    }
}

int event(int t, int a, int b)
{
    if(t == KY_DOWN)
    {
        if(state == 1)
        {
            cd(a);
        }
        return 0;
    }

    if(t == KY_UP)
    {
        if(a == _SLEFT)
        {
            if(0 == state)
            {
                start();
            }
            else
            {
                stop();
            }
            return 0;
        }
        if(a == _SRIGHT)
        {
            timerdel(timer);
            exit();
        }
    }
    return 0;
}



void start()
{
    timerstart(timer, 500, 0, "_timer", TRUE);
    state = 1;
}

void stop()
{
    timerstop(timer);
    state = 0;
}

void _timer(int32 data)
{
    if(last == _LEFT)
    {
        if(0 == arr[0] % colc)
        {
            gameover();
        }
        else
        {
            go( - 1);
        }
        return ;
    }
    if(last == _DOWN)
    {
        if(arr[0] >= (linc - 1) *colc)
        {
            gameover();
        }
        else
        {
            go(colc);
        }
        return ;
    }
    if(last == _RIGHT)
    {
        if((colc - 1) == arr[0] % colc)
        {
            gameover();
        }
        else
        {
            go(1);
        }
        return ;
    }
    if(last == _UP)
    {
        if(arr[0] < colc)
        {
            gameover();
        }
        else
        {
            go( - colc);
        }
        return ;
    }
}


int pause()
{
    return 0;
}
int resume()
{
    return 0;
}