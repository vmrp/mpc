#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "window.h"







extern char *tempfile_path;
extern char cdir[];



int InitApplication(VOID);

int ExitApplication(VOID);

int PauseApplication(VOID);

int ResumeApplication(VOID);


#endif
