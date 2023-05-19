#ifndef __BSP_MG90S_H__
#define	__BSP_MG90S_H__

#include "stm32f4xx.h"

/***************************** GPIO���� *********************************/
#define MG90S_GPIO_PORT        			GPIOA
#define MG90S_GPIO_CLK					RCC_AHB1Periph_GPIOA
#define MG90S_PIN              			GPIO_Pin_6

#define MG90S_PINSOURCE					GPIO_PinSource6
#define MG90S_AF						GPIO_AF_TIM3

#define MG90S_CCRx                 CCR1

/***************************** ��ʱ������ *********************************/
#define GENERAL_TIM3           			TIM3
#define GENERAL_TIM3_CLK       			RCC_APB1Periph_TIM3


/***************************** �������� *********************************/
void MG90S_Init(void);
void MG90S_Crt(int16_t PWM);

#endif /* __BSP_MG90S_H__ */

