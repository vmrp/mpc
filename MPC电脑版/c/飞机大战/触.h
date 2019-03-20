  void Eveu(int tpe,int px,int py)
{
 CEP();
  if((px<=40)&&(py>=320&&py<=360))
{
  if(tpe==2)event(0,_SLEFT,0);
   if(tpe==3)event(1,_SLEFT,0);
}
  if((px<=40)&&(py>=360&&py<=400))
{
 if(tpe==2)event(0,_SEND,0);
 if(tpe==3)event(1,_SEND,0);

}
  if((px>=200&&px<=240)&&(py>=320&&py<=360))
{
  if(tpe==2)event(0,_SRIGHT,0);
  if(tpe==3)event(1,_SRIGHT,0);
}
  if((px>=200&&px<=240)&&(py>=360&&py<=400)){}

  if((px>=40&&px<=90)&&(py>=320&&py<=340))
{
 if(tpe==2)event(0,_1,0);
 if(tpe==3)event(1,_1,0);

}
  if((px>=40&&px<=90)&&(py>=380&&py<=400))
{
 if(tpe==2)event(0,_7,0);
 if(tpe==3)event(1,_7,0);
}
  if((px>=150&&px<=200)&&(py>=320&&py<=340))
{
  if(tpe==2)event(0,_3,0);
  if(tpe==3)event(1,_3,0);
}
  if((px>=150&&px<=200)&&(py>=380&&py<=400))
{
 if(tpe==2)event(0,_9,0);
 if(tpe==3)event(1,_9,0);
}
  if((px>=90&&px<=150)&&(py>=320&&py<=340))
{
 if(tpe==2)event(0,_UP,0);
 if(tpe==3)event(1,_UP,0);
}
  if((px>=40&&px<=90)&&(py>=340&&py<=380))
{
 if(tpe==2)event(0,_LEFT,0);
 if(tpe==3)event(1,_LEFT,0);
}
  if((px>=150&&px<=200)&&(py>=340&&py<=380))
{
 if(tpe==2)event(0,_RIGHT,0);
 if(tpe==3)event(1,_RIGHT,0);
}
  if((px>=90&&px<=150)&&(py>=380&&py<=400))
{
 if(tpe==MS_DOWN)event(KY_DOWN,_DOWN,0);
 if(tpe==MS_UP)event(KY_UP,_DOWN,0);
}
 
 if((px>=90&&px<=150)&&(py>=340&&py<=380))
{
 if(tpe==2)event(0,_SELECT,0);
 if(tpe==3)event(1,_SELECT,0);

}

break;
}
