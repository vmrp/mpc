
uint8 state;//游戏状态
uint32 score;//分数
uint8 guan;//关数

//人
typedef struct
{
 uint16 x;
 uint16 y;
 uint16 len;//火焰长度
}Man;




//怪
typedef struct
{
 uint16 x;
 uint16 y;
 uint8 mt;//移动方向
 uint32 fen;//杀死分数
}Monster;



