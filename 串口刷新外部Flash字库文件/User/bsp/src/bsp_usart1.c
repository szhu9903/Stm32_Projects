#include "bsp_usart1.h"

static USART_DATA_TypeDef myUsart1 = {0};

static void USART1_DMA_Config(void);

/*
*********************************************************************************************************
*  函 数 名: USART1_DMA_Config
*  功能说明: USART1 DMA配置
*  形    参：无
*  返 回 值: 无
*********************************************************************************************************
*/
static void USART1_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;

    // 开启DMA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	/*usart1 rx对应dma1，通道5 */            				
  	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)DEBUG_USART_DR_BASE;            		//DMA外设地址
  	DMA_InitStructure.DMA_MemoryBaseAddr    = (u32)usart1->buff;       					    //DMA 存储器0地址
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;  						//外设到存储器模式
  	DMA_InitStructure.DMA_BufferSize         = MAX_BUFF_SIZE;       						//数据传输量 
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;   				//外设非增量模式
  	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;        				//存储器增量模式
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 				//外设数据长度:8位
  	DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;     				//存储器数据长度:8位
  	DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;             				//使用普通模式 
  	DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;         				//中等优先级
	DMA_InitStructure.DMA_M2M 				 = DMA_M2M_Disable;
  	DMA_Init(DMA1_Channel5, &DMA_InitStructure);                             				//初始化DMA Stream
  	
	DMA_Cmd(DMA1_Channel5, ENABLE);
	
}


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
	
	RCC_APB2PeriphClockCmd(USART1_USART_CLK|USART1_RX_GPIO_CLK|USART1_TX_GPIO_CLK, ENABLE);

	/* 配置GPIO */
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = USART1_RX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = USART1_TX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStruct);
	
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
	
	usart1=&myUsart1;
	
	USART1_DMA_Config();
	
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

/*
*********************************************************************************************************
*  函 数 名: USART1_IRQHandler
*  功能说明: USART1 中断函数
*  形    参：无
*  返 回 值: 无
*********************************************************************************************************
*/
void USART1_IRQHandler(void)
{
	static u32 num = 0;
	if(USART_GetITStatus(USART1_USART, USART_IT_IDLE) == SET )                                         
	{
		volatile u8 clear;
		DMA_Cmd(DMA1_Channel5, DISABLE);  /* 暂时关闭dma，数据尚未处理 */
		
		//清除空闲标志位
		clear = USART1_USART->SR;
		clear = USART1_USART->DR;
		
		/* 接收的数据长度 */
		usart1->InfBit.FramLength = MAX_BUFF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel5);
		usart1->InfBit.FramFinishFlag = 1;
		DMA_ClearFlag(DMA1_FLAG_TC5);
		printf("getdata len %d -- %d \r\n", usart1->InfBit.FramLength, num);
		
		FLASH_BufferWrite((u8 *)usart1->buff, num * 1024, 1024);
		num += 1;
		printf("g1 %d \r\n", num);
		/* 重新赋值计数值，必须大于等于最大可能接收到的数据帧数目 */
		DMA_SetCurrDataCounter(DMA1_Channel5, (uint16_t)MAX_BUFF_SIZE);
		/* 此处应该在处理完数据再打开，如在 DataPack_Process() 打开*/
		DMA_Cmd(DMA1_Channel5, ENABLE);
    }
}


