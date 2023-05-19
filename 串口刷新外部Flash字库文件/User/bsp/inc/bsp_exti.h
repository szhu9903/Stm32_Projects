#ifndef __BSP_EXTI_H__
#define __BSP_EXTI_H__

#include "stm32f10x.h"
#include "bsp_relay.h"
#include "bsp_led.h"
#include <stdio.h>


/***************************** LD2410BÖÐ¶Ï *********************************/
#define LD2410B_INT_GPIO_PORT			GPIOB
#define LD2410B_INT_GPIO_CLK			RCC_APB2Periph_GPIOB
#define LD2410B_INT_GPIO_PIN            GPIO_Pin_8

#define LD2410B_INT_EXTI_PORTSOURCE     GPIO_PortSourceGPIOB	
#define LD2410B_INT_EXTI_PINSOURCE      GPIO_PinSource8

#define LD2410B_INT_EXTI_LINE			EXTI_Line8				
#define LD2410B_INT_EXTI_IRQ            EXTI9_5_IRQn				

#define LD2410B_IRQHandler              EXTI9_5_IRQHandler		


void EXTI_LD2410B_Config(void);
void EXTIx_Init(void);

#endif
