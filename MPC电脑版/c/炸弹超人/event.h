

int event(int type, int p1, int p2)
{
 if(KY_DOWN == type)
 {
 switch(p1)
  {
  case _SRIGHT:
  exitapp();break;
  case _1:
  break;
  }
 }
 return 0;
}



void exitapp()
{
 ShowTt();
 BmpFree();

 exit();
}