#ifndef __SYS_DELAY_H__
#define __SYS_DELAY_H__ 	

#include "stm32f10x.h"

void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif
