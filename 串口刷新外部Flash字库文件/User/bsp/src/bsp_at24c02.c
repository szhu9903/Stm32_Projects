#include "bsp_at24c02.h"


static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT; 
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);


/*
*********************************************************************************************************
*  函 数 名: AT24C02_Init
*  功能说明: AT24C02初始化 I2C PB8、PB9
*  形    参: 无
*  返 回 值: 无
*********************************************************************************************************
*/
void AT24C02_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C1_InitStruct;
	
	RCC_APB2PeriphClockCmd(AT24C02_I2C_SCL_CLK|AT24C02_I2C_SDA_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(AT24C02_I2C_CLK, ENABLE);

	/* 配置GPIO */
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	
	GPIO_InitStruct.GPIO_Pin = AT24C02_I2C_SCL_PIN;
	GPIO_Init(AT24C02_I2C_SCL_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = AT24C02_I2C_SDA_PIN;
	GPIO_Init(AT24C02_I2C_SDA_PORT, &GPIO_InitStruct);


	/* 配置I2C1 */
	I2C1_InitStruct.I2C_ClockSpeed = AT24C02_I2C_BAUDRATE;
	I2C1_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C1_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C1_InitStruct.I2C_OwnAddress1 = AT24C02_STM32_ADDRESS7;
	I2C1_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C1_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(AT24C02_I2C, &I2C1_InitStruct);
	
	/* 使能I2C1 */
	I2C_Cmd(AT24C02_I2C, ENABLE); 
	I2C_AcknowledgeConfig(AT24C02_I2C, ENABLE);  
}

/*
*********************************************************************************************************
*  函 数 名: AT24C02_ByteWrite
*  功能说明: 写一个字节到AT24C02中
*  形    参: 	pBuffer: 要写入的byte
*				WriteAddr: 写入到AT24C02的地址
*  返 回 值: 正常返回1，异常返回0
*********************************************************************************************************
*/
uint32_t AT24C02_ByteWrite(u8 byte, u8 WriteAddr)
{
	/* Send STRAT condition */
	I2C_GenerateSTART(AT24C02_I2C, ENABLE);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(AT24C02_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
	}

	/* Send AT24C02 address for write */
	I2C_Send7bitAddress(AT24C02_I2C, AT24C02_ADDRESS, I2C_Direction_Transmitter);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(AT24C02_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
	}    
	  
	/* Send the AT24C02's internal address to write to */
	I2C_SendData(AT24C02_I2C, WriteAddr);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(AT24C02_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  
	{
		if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
	} 
	
	/* Send the byte to be written */
	I2C_SendData(AT24C02_I2C, byte); 
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(AT24C02_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
	} 

	/* Send STOP condition */
	I2C_GenerateSTOP(AT24C02_I2C, ENABLE);

	return 1;
}

/*
*********************************************************************************************************
*  函 数 名: AT24C02_BufferRead
*  功能说明: 从AT24C02里面读取一块数据
*  形    参: 	pBuffer: 存放从AT24C02读取的数据的缓冲区指针
*				ReadAddr: 接收数据的AT24C02的地址
*				NumByteToRead: 读取的字节数
*  返 回 值: 正常返回1，异常返回0
*********************************************************************************************************
*/
uint32_t AT24C02_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{
	//等待I2C空闲
	I2CTimeout = I2CT_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(AT24C02_I2C, I2C_FLAG_BUSY))   
    {
		if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);
    }
	
	/* 发送起始指令 */
	I2C_GenerateSTART(AT24C02_I2C, ENABLE);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(AT24C02_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
	}    

	/* 发送 AT24C02 address */
	I2C_Send7bitAddress(AT24C02_I2C, AT24C02_ADDRESS, I2C_Direction_Transmitter);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(AT24C02_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
	}    
	  
	/* 发送读取的地址 */
	I2C_SendData(AT24C02_I2C, ReadAddr);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(AT24C02_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  
	{
		if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
	} 
	
	
	/* 发送开始 */  
	I2C_GenerateSTART(AT24C02_I2C, ENABLE);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(AT24C02_I2C, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(13);
	}
	
	/* 发送 AT24C02 address */
	I2C_Send7bitAddress(AT24C02_I2C, AT24C02_ADDRESS, I2C_Direction_Receiver);
	I2CTimeout = I2CT_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(AT24C02_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(14);
	}
	
	/* While there is data to be read */
	while(NumByteToRead)  
	{
		if(NumByteToRead == 1)
		{
			/* Disable Acknowledgement */
			I2C_AcknowledgeConfig(AT24C02_I2C, DISABLE);
			/* Send STOP Condition */
			I2C_GenerateSTOP(AT24C02_I2C, ENABLE);
		}

		I2CTimeout = I2CT_LONG_TIMEOUT;
		while(I2C_CheckEvent(AT24C02_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)  
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
		} 	

		/* Read a byte from the device */
		*pBuffer = I2C_ReceiveData(AT24C02_I2C);
		/* Point to the next location where the byte read will be saved */
		pBuffer++; 
		/* Decrement the read bytes counter */
		NumByteToRead--;
	}
	/* Enable Acknowledgement to be ready for another reception */
	I2C_AcknowledgeConfig(AT24C02_I2C, ENABLE);

	return 1;
}

/*
*********************************************************************************************************
*  函 数 名: I2C_TIMEOUT_UserCallback
*  功能说明: I2C等待事件超时的情况下会调用这个函数来处理
*  形    参: 	errorCode：错误代码，可以用来定位是哪个环节出错.
*  返 回 值: 返回0，表示IIC读取失败.
*********************************************************************************************************
*/
static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
    /* 使用串口printf输出错误信息，方便调试 */
    AT24C02_ERROR("I2C 等待超时!errorCode = %d", errorCode);
    return 0;
}

