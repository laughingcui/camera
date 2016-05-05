/***************************************************************
 *CopyRight (C), 2015-2016
 *
 *Filename: beep.h

 *Author: cuishiyang   Version:1.0   Date:2015.3.18
 *
 *Describe:
 *	This file function is driver beep . 
 *	You can open beep and close beep.
 *	
 *History:
 **************************************************************/

#ifndef _BEEP_H_
#define _BEEP_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DEBUG
#define BEEP_ON 	0x100001
#define BEEP_OFF 	0x100002
#define BEEP_DRIVER_FILENAME "/dev/mybeep"

#if 0
#ifdef DEBUG
#define LOG_PRINT(format, args...) printf("[%s@%s, %d] " format, \
		__FUNCTION__,__FILE__, __LINE__, ##args );
#else
#define LOG_PRINT(format, args...)
#endif
#endif

extern int initBeep(void);
extern int startBeep(void);
extern int stopBeep(void);
extern int closeBeep(void);

#ifdef __cplusplus
}
#endif

#endif // _BEEP_H_
