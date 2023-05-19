#ifndef __BSP_BEEP_H__
#define __BSP_BEEP_H__

#include "stm32f10x.h"
#include "sys_delay.h"

/***************************** 引脚定义 *********************************/
#define BEEP_PIN              GPIO_Pin_1
#define BEEP_GPIO_PORT        GPIOA
#define BEEP_GPIO_CLK         RCC_APB2Periph_GPIOA


/*****************************************************/
/* 直接操作寄存器方式控制IO */
#define digitalHi(p, i)           	{p->BSRR=i;}     // 设置高电平
#define digitalLo(p, i)           	{p->BRR=i;}     // 设置低电平

/* 定义控制宏 */
#define BEEP_ON 					digitalHi(BEEP_GPIO_PORT, BEEP_PIN)
#define BEEP_OFF 					digitalLo(BEEP_GPIO_PORT, BEEP_PIN)


void BEEP_Init(void);
void Short_Beep(void);
void Long_Beep(void);
void Beep_OK(void);

#endif
