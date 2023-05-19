#include "bsp_exti.h"


void EXTI_LD2410B_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(LD2410B_INT_GPIO_CLK|RCC_APB2Periph_AFIO, ENABLE);
	
	/*开启GPIO口*/
	GPIO_InitStructure.GPIO_Pin = LD2410B_INT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(LD2410B_INT_GPIO_PORT, &GPIO_InitStructure);
	
	/* 配置 NVIC */
	NVIC_InitStructure.NVIC_IRQChannel = LD2410B_INT_EXTI_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
    /* 连接 EXTI 中断源 到LD2410B引脚 */
    GPIO_EXTILineConfig(LD2410B_INT_EXTI_PORTSOURCE,
                        LD2410B_INT_EXTI_PINSOURCE);

    /* 配置 EXTI 中断源 */
    EXTI_InitStructure.EXTI_Line = LD2410B_INT_EXTI_LINE;
    /* 中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 上升沿触发 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    /* 使能中断/事件线 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

}

/*
*********************************************************************************************************
*  函 数 名: EXTIx_Init
*  功能说明: 外部中断配置
*  形    参: 无
*  返 回 值: 无
*********************************************************************************************************
*/
void EXTIx_Init(void)
{
	/* 人体感应外部GPIO中断 */
	EXTI_LD2410B_Config();
	
}


void LD2410B_IRQHandler(void)
{
    //确保是否产生了EXTI Line中断
    if (EXTI_GetITStatus(LD2410B_INT_EXTI_LINE) != RESET) {

		if(GPIO_ReadInputDataBit(LD2410B_INT_GPIO_PORT, LD2410B_INT_GPIO_PIN) == 1)
		{
			RELAY_ON;
			LED_ON;
		}
		else
		{
			RELAY_OFF;
			LED_OFF;
		}
		
        //清除中断标志位
        EXTI_ClearITPendingBit(LD2410B_INT_EXTI_LINE);
    }
}
