#include "bsp_usart1.h"


/*
*********************************************************************************************************
*  函 数 名: USART1_Config
*  功能说明: USART1 配置
*  形    参：无
*  返 回 值: 无
*********************************************************************************************************
*/
static void USART1_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART1_InitStruct;
	
	RCC_AHB1PeriphClockCmd(USART1_RX_GPIO_CLK|USART1_TX_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(USART1_USART_CLK, ENABLE);

	/* 配置GPIO */
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	
	GPIO_InitStruct.GPIO_Pin = USART1_RX_GPIO_PIN;
	GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStruct);
	GPIO_PinAFConfig(USART1_RX_GPIO_PORT, USART1_RX_GPIO_PINSOURCE, USART1_RX_GPIO_AF);
	GPIO_InitStruct.GPIO_Pin = USART1_TX_GPIO_PIN;
	GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStruct);
	GPIO_PinAFConfig(USART1_TX_GPIO_PORT, USART1_TX_GPIO_PINSOURCE, USART1_TX_GPIO_AF);
	
	/* 配置串口 */
	USART1_InitStruct.USART_BaudRate = USART1_USART_BAUDRATE;
	USART1_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART1_InitStruct.USART_StopBits = USART_StopBits_1;
	USART1_InitStruct.USART_Parity = USART_Parity_No;
	USART1_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART1_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1_USART, &USART1_InitStruct);
	
	/* 配置中断 */
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	/* 使能串口接收中断 已准备好读取接收到的数据 */
	USART_ITConfig(USART1_USART, USART_IT_IDLE, ENABLE); 						   //使能串口空闲中断 
	USART_DMACmd(USART1_USART, USART_DMAReq_Rx, ENABLE);                           //使能USART1DMA接收
	USART_ClearFlag(USART1_USART, USART_FLAG_TC);
	
	/* 使能串口 */
	USART_Cmd(USART1_USART, ENABLE);
	
}

/*
*********************************************************************************************************
*  函 数 名: USART1_Init
*  功能说明: USART1 初始化
*  形    参：无
*  返 回 值: 无
*********************************************************************************************************
*/
void USART1_Init(void)
{
	USART1_Config();
	
}

/*
*********************************************************************************************************
*  函 数 名: USART1_SendByte
*  功能说明: 发送一个byte
*  形    参: str: 一个byte
*  返 回 值: 无
*********************************************************************************************************
*/
void USART1_SendByte(uint8_t ch)
{
	USART_SendData(USART1_USART, ch);
	while (USART_GetFlagStatus(USART1_USART, USART_FLAG_TXE) == RESET);	
}

/*
*********************************************************************************************************
*  函 数 名: USART1_SendString
*  功能说明: 发送一个字符串
*  形    参: str: 字符串指针
*  返 回 值: 无
*********************************************************************************************************
*/
void USART1_SendString(char *str)
{
	while(*str)
	{
		USART1_SendByte(*str);
		str++;
	}
	/* 等待发送完成 */
	while(USART_GetFlagStatus(USART1_USART,USART_FLAG_TC)==RESET);
}

/*
*********************************************************************************************************
*  函 数 名: fputc
*  功能说明: 重定向c库函数printf到串口DEBUG_USART，重定向后可使用printf函数
*  形    参：ch 字符串
*  返 回 值: 无
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口DEBUG_USART */
	USART1_SendByte((uint8_t)ch);

	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USART1_USART, USART_FLAG_TXE) == RESET);		

	return (ch);
}


