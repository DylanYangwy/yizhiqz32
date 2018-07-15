#ifndef __PVD_H
#define __PVD_H
//#include "sys.h" 
#include <stm32f10x.h>

#define SIZE_OF_PVD_WORD	200
#define SIZE_OF_PVD_BIT		1024
#define FLASH_SAVE_BIT_PVD_ADDR	0x0807E800




void PVD_Init(void);




#endif

