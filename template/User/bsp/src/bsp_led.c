#include "bsp_led.h"

/*
*********************************************************************************************************
*  函 数 名: LED_Init
*  功能说明: LED引脚初始化
*  形    参: 无
*  返 回 值: 无
*********************************************************************************************************
*/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);
	
	/* LED */
	GPIO_InitStructure.GPIO_Pin = LED_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);
	
	LED_OFF;
}
