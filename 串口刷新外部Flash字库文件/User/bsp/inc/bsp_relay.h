#ifndef __BSP_RELAY_H__
#define __BSP_RELAY_H__

#include "stm32f10x.h"

//引脚定义
/*****************************************************/
#define RELAY_PIN              GPIO_Pin_5
#define RELAY_GPIO_PORT        GPIOB
#define RELAY_GPIO_CLK         RCC_APB2Periph_GPIOB

/*****************************************************/
/* 直接操作寄存器的方法控制IO */
#define digitalHi(p,i)       {p->BSRR=i;}    //输出为高电平
#define digitalLo(p,i)       {p->BRR=i;}     //输出低电平

/* 定义控制IO的宏 */
#define RELAY_OFF          	digitalHi(RELAY_GPIO_PORT,RELAY_PIN)
#define RELAY_ON           	digitalLo(RELAY_GPIO_PORT,RELAY_PIN)

void RELAY_Init(void);

#endif
