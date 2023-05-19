#include "bsp_relay.h"

/*
*********************************************************************************************************
*  函 数 名: RELAY_Init
*  功能说明: 继电器控制引脚初始化
*  形    参: 无
*  返 回 值: 无
*********************************************************************************************************
*/
void RELAY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RELAY_GPIO_CLK, ENABLE);
	
	/* RELAY */
	GPIO_InitStructure.GPIO_Pin = RELAY_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RELAY_GPIO_PORT, &GPIO_InitStructure);
	
	RELAY_OFF;
}
