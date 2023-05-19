#include "bsp_mg90s.h"



static void MG90S_GPIO_Config(void);
static void MG90S_Tim_Config(void);

/*
*********************************************************************************************************
*  函 数 名: LED_GPIO_Config
*  功能说明: 初始化控制LED的GPIO
*  形    参: 无
*  返 回 值: 无
*********************************************************************************************************
*/
static void MG90S_GPIO_Config(void)
{
	/* 初始化GPIO结构体 */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启LED相关的GPIO外设时钟*/
	RCC_AHB1PeriphClockCmd(MG90S_GPIO_CLK, ENABLE); 

	GPIO_PinAFConfig(MG90S_GPIO_PORT, MG90S_PINSOURCE, MG90S_AF); 
	
	/* GPIO结构体配置 */		
	GPIO_InitStructure.GPIO_Pin = MG90S_PIN;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(MG90S_GPIO_PORT, &GPIO_InitStructure);	
}
/*
*********************************************************************************************************
*  函 数 名: LED_Tim_Mode_Config
*  功能说明: LED PWM配置
*  形    参: 无
*  返 回 值: 无
*********************************************************************************************************
*/
static void MG90S_Tim_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/* 开启定时器时钟 */
	RCC_APB1PeriphClockCmd(GENERAL_TIM3_CLK,ENABLE);
	
	/* 定时器时基结构体配置 */		 
	TIM_TimeBaseStructure.TIM_Period = 2000-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Prescaler = 840-1;	
	TIM_TimeBaseInit(GENERAL_TIM3, &TIM_TimeBaseStructure);		
	
	/*PWM模式配置*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(GENERAL_TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(GENERAL_TIM3, TIM_OCPreload_Enable);
	
	/* 使能TIM重载寄存器ARR */
	TIM_ARRPreloadConfig(GENERAL_TIM3, ENABLE);			

	/* 使能计数器 */
	TIM_Cmd(GENERAL_TIM3, ENABLE);	
}


/*
*********************************************************************************************************
*  函 数 名: TIM2_Init
*  功能说明: 通用定时器2初始化
*  形    参: 无
*  返 回 值: 无
*********************************************************************************************************
*/
void MG90S_Init(void)
{
	MG90S_GPIO_Config();
	MG90S_Tim_Config();
}

// 2号舵机
void MG90S_Crt(int16_t PWM)
{
  	GENERAL_TIM3->MG90S_CCRx =PWM;
}


/*********************************************END OF FILE**********************/
