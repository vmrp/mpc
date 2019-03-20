#ifndef _MAIN_H_
#define _MAIN_H

typedef enum
{
	E_PAY,
	E_MAX
}E_MODULE_VALUE;

void SubModule_notifyMain(E_MODULE_VALUE moduleValue, int32 msg);

typedef void (*PayEntry_CB)(void);


#endif
