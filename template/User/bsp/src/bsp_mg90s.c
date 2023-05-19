#include "bsp_mg90s.h"



static void MG90S_GPIO_Config(void);
static void MG90S_Tim_Config(void);

/*
*********************************************************************************************************
*  �� �� ��: LED_GPIO_Config
*  ����˵��: ��ʼ������LED��GPIO
*  ��    ��: ��
*  �� �� ֵ: ��
*********************************************************************************************************
*/
static void MG90S_GPIO_Config(void)
{
	/* ��ʼ��GPIO�ṹ�� */
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*����LED��ص�GPIO����ʱ��*/
	RCC_AHB1PeriphClockCmd(MG90S_GPIO_CLK, ENABLE); 

	GPIO_PinAFConfig(MG90S_GPIO_PORT, MG90S_PINSOURCE, MG90S_AF); 
	
	/* GPIO�ṹ������ */		
	GPIO_InitStructure.GPIO_Pin = MG90S_PIN;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;    
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_Init(MG90S_GPIO_PORT, &GPIO_InitStructure);	
}
/*
*********************************************************************************************************
*  �� �� ��: LED_Tim_Mode_Config
*  ����˵��: LED PWM����
*  ��    ��: ��
*  �� �� ֵ: ��
*********************************************************************************************************
*/
static void MG90S_Tim_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/* ������ʱ��ʱ�� */
	RCC_APB1PeriphClockCmd(GENERAL_TIM3_CLK,ENABLE);
	
	/* ��ʱ��ʱ���ṹ������ */		 
	TIM_TimeBaseStructure.TIM_Period = 2000-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Prescaler = 840-1;	
	TIM_TimeBaseInit(GENERAL_TIM3, &TIM_TimeBaseStructure);		
	
	/*PWMģʽ����*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(GENERAL_TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(GENERAL_TIM3, TIM_OCPreload_Enable);
	
	/* ʹ��TIM���ؼĴ���ARR */
	TIM_ARRPreloadConfig(GENERAL_TIM3, ENABLE);			

	/* ʹ�ܼ����� */
	TIM_Cmd(GENERAL_TIM3, ENABLE);	
}


/*
*********************************************************************************************************
*  �� �� ��: TIM2_Init
*  ����˵��: ͨ�ö�ʱ��2��ʼ��
*  ��    ��: ��
*  �� �� ֵ: ��
*********************************************************************************************************
*/
void MG90S_Init(void)
{
	MG90S_GPIO_Config();
	MG90S_Tim_Config();
}

// 2�Ŷ��
void MG90S_Crt(int16_t PWM)
{
  	GENERAL_TIM3->MG90S_CCRx =PWM;
}


/*********************************************END OF FILE**********************/
