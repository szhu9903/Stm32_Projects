#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "stm32f10x.h"

//引脚定义
/*****************************************************/
#define LED_PIN              GPIO_Pin_13
#define LED_GPIO_PORT        GPIOC
#define LED_GPIO_CLK         RCC_APB2Periph_GPIOC

/*****************************************************/
/* 直接操作寄存器的方法控制IO */
#define digitalHi(p,i)       {p->BSRR=i;}    //输出为高电平
#define digitalLo(p,i)       {p->BRR=i;}     //输出低电平
#define digitalToggle(p,i)   {p->ODR ^=i;}   //输出反转状态


/* 定义控制IO的宏 */
#define LED_TOGGLE       	digitalToggle(LED_GPIO_PORT,LED_PIN)
#define LED_OFF          	digitalHi(LED_GPIO_PORT,LED_PIN)
#define LED_ON           	digitalLo(LED_GPIO_PORT,LED_PIN)

void LED_Init(void);

#endif
