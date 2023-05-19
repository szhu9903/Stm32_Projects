#ifndef __BSP_USART1_H__
#define __BSP_USART1_H__

#include "stm32f10x.h"
#include "sys_config.h"
#include "bsp_flash.h"
#include <stdio.h>
#include <string.h>

extern USART_DATA_TypeDef *usart1;

//Òý½Å¶¨Òå
/*******************************************************/
#define USART1_USART                    USART1

#define USART1_USART_CLK                RCC_APB2Periph_USART1
#define USART1_USART_BAUDRATE			115200

#define USART1_RX_GPIO_PORT				GPIOA
#define USART1_RX_GPIO_CLK         		RCC_APB2Periph_GPIOA
#define USART1_RX_GPIO_PIN				GPIO_Pin_10
#define USART1_RX_GPIO_AF               GPIO_AF_USART1
#define USART1_RX_GPIO_PINSOURCE		GPIO_PinSource10

#define USART1_TX_GPIO_PORT				GPIOA
#define USART1_TX_GPIO_CLK         		RCC_APB2Periph_GPIOA
#define USART1_TX_GPIO_PIN				GPIO_Pin_9
#define USART1_TX_GPIO_AF               GPIO_AF_USART1
#define USART1_TX_GPIO_PINSOURCE		GPIO_PinSource9

#define DEBUG_USART_DR_BASE               (USART1_BASE+0x04)	

void USART1_Init(void);
void USART1_SendByte(uint8_t ch);
void USART1_SendString(char *str);
void DMA_USART1_Send(u8 *data,u16 size);
#endif
