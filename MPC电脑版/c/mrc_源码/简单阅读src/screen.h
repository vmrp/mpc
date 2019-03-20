#ifndef SCREEN_H 
#define SCREEN_H
#define SCREENFILE "screen.buf"


int ScreenShut(char *filename,int x,int y,int w,int h,int is_hand);


void saveScreenBuf (void);

int readScreenBuf (void);

#endif 