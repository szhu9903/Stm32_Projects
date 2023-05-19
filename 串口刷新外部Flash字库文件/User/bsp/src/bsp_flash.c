#include "bsp_flash.h"

static uint16_t SPI_Timeout;
static uint8_t FLASH_TimeoutCallback(uint8_t Code);

/*
*********************************************************************************************************
*  函 数 名: FLASH_Init
*  功能说明: FLASH 初始化 SPI CSS、SCLK MISO MOSI
*  形    参: 无
*  返 回 值: 无
*********************************************************************************************************
*/
void FLASH_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;
	
	RCC_APB2PeriphClockCmd(FLASH_SPI_SCLK_CLK|FLASH_SPI_MISO_CLK|
						   FLASH_SPI_MOSI_CLK|FLASH_SPI_CSS_CLK, ENABLE);
	FLASH_SPI_CLK_CMD(FLASH_SPI_CLK, ENABLE);

	/* 配置GPIO */
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	
	GPIO_InitStruct.GPIO_Pin = FLASH_SPI_SCLK_PIN;
	GPIO_Init(FLASH_SPI_SCLK_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = FLASH_SPI_MISO_PIN;
	GPIO_Init(FLASH_SPI_MISO_PORT, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = FLASH_SPI_MOSI_PIN;
	GPIO_Init(FLASH_SPI_MOSI_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = FLASH_SPI_CSS_PIN;
	GPIO_Init(FLASH_SPI_CSS_PORT, &GPIO_InitStruct);

	SPI_CSS_HIGH();

	/* SPI2 */
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_CRCPolynomial = 7;
	SPI_Init(FLASH_SPI, &SPI_InitStruct);
	
	/* 使能SPI2 */
	SPI_Cmd(FLASH_SPI, ENABLE); 
}

/*
*********************************************************************************************************
*  函 数 名: FLASH_RSByte
*  功能说明: FLASH 发一个字节数据
*  形    参: Byte: 要发送的字节
*  返 回 值: 接收的一个字节
*********************************************************************************************************
*/
uint8_t FLASH_RSByte(uint8_t Byte)
{
	/* 等待清空发送缓冲区，后写入下个数据 */
	SPI_Timeout = SPI_FLAG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET)
	{
		if(!SPI_Timeout--) return FLASH_TimeoutCallback(1);
	}
	SPI_I2S_SendData(FLASH_SPI, Byte);
	
	/* 等待接收缓冲区数据，并读取 */
	SPI_Timeout = SPI_FLAG_TIMEOUT;
	while(SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET)
	{
		if(!SPI_Timeout--) return FLASH_TimeoutCallback(2);
	}
	return SPI_I2S_ReceiveData(FLASH_SPI);
}

/*
*********************************************************************************************************
*  函 数 名: FLASH_ReadByte
*  功能说明: FLASH 接收字节数据
*  形    参: 无
*  返 回 值: 接收的一个字节
*********************************************************************************************************
*/
uint8_t FLASH_ReadByte(void)
{
    return (FLASH_RSByte(Dummy_Byte));
}

/*
*********************************************************************************************************
*  函 数 名: FLASH_ReadID
*  功能说明: 读取FLASH ID
*  形    参: 无
*  返 回 值: FLASH ID
*********************************************************************************************************
*/
uint32_t FLASH_ReadID(void)
{
	uint32_t FLASH_Id=0x00;
	SPI_CSS_LOW();
	FLASH_RSByte(W25X_JedecDeviceID);
    FLASH_Id |= (FLASH_RSByte(Dummy_Byte)<<16);
    FLASH_Id |= (FLASH_RSByte(Dummy_Byte)<<8);
    FLASH_Id |= FLASH_RSByte(Dummy_Byte);
    SPI_CSS_HIGH();
	return FLASH_Id;
}

/*
*********************************************************************************************************
*  函 数 名: FLASH_WriteEnable
*  功能说明: 向FLASH发送 写使能 命令
*  形    参: 无
*  返 回 值: 无
*********************************************************************************************************
*/
void FLASH_WriteEnable(void)
{
    SPI_CSS_LOW();
    FLASH_RSByte(W25X_WriteEnable);
    SPI_CSS_HIGH();
}

/*
*********************************************************************************************************
*  函 数 名: FLASH_WaitForWriteEnd
*  功能说明: 等待FLASH内部数据写入或擦除完毕
*  形    参: 无
*  返 回 值: 无
*********************************************************************************************************
*/
void FLASH_WaitForWriteEnd(void)
{
    uint8_t FLASH_Status = 0;
    SPI_CSS_LOW();
    /* 发送 读状态寄存器 命令 */
    FLASH_RSByte(W25X_ReadStatusReg);
    SPI_Timeout = SPI_FLAG_TIMEOUT;
    /* 若FLASH忙碌，则等待 */
	do
	{
		/* 读取FLASH芯片的状态寄存器 */
		FLASH_Status = FLASH_RSByte(Dummy_Byte);	 

		{
			if((SPI_Timeout--) == 0) 
			{
				FLASH_TimeoutCallback(4);
				return;
			}
		} 
	}while ((FLASH_Status & 0x01) == SET); /* 正在写入标志 */

    SPI_CSS_HIGH();
}

/*
*********************************************************************************************************
*  函 数 名: FLASH_SectorErase
*  功能说明: 擦除FLASH扇区
*  形    参: SectorAddr: 要擦除的扇区地址
*  返 回 值: 无
*********************************************************************************************************
*/
void FLASH_SectorErase(u32 SectorAddr)
{
    /* 发送FLASH写使能命令 */
    FLASH_WriteEnable();
    FLASH_WaitForWriteEnd();
    /* 擦除扇区 */
    SPI_CSS_LOW();
    FLASH_RSByte(W25X_SectorErase);
    FLASH_RSByte((SectorAddr & 0xFF0000) >> 16);
    FLASH_RSByte((SectorAddr & 0xFF00) >> 8);
    FLASH_RSByte(SectorAddr & 0xFF);
    SPI_CSS_HIGH();
    /* 等待擦除完毕*/
    FLASH_WaitForWriteEnd();
}

/*
*********************************************************************************************************
*  函 数 名: FLASH_ChipErase
*  功能说明: 擦除FLASH芯片
*  形    参: 无
*  返 回 值: 无
*********************************************************************************************************
*/
void FLASH_ChipErase(void)
{
    /* 发送FLASH写使能命令 */
    FLASH_WriteEnable();
    FLASH_WaitForWriteEnd();
    /* 擦除扇区 */
    SPI_CSS_LOW();
    FLASH_RSByte(W25X_ChipErase);
    SPI_CSS_HIGH();
    /* 等待擦除完毕*/
    FLASH_WaitForWriteEnd();
}

/*
*********************************************************************************************************
*  函 数 名: FLASH_PageWrite
*  功能说明: 对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
*  形    参: 	pBuffer: 要写入数据的指针
*				WriteAddr: 写入地址
*				NumByteToWrite: 写入数据长度，必须小于等于SPI_FLASH_PerWritePageSize
*  返 回 值: 无
*********************************************************************************************************
*/
void FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
  /* 发送FLASH写使能命令 */
  FLASH_WriteEnable();

  SPI_CSS_LOW();
  /* 写页写指令*/
  FLASH_RSByte(W25X_PageProgram);
  FLASH_RSByte((WriteAddr & 0xFF0000) >> 16);
  FLASH_RSByte((WriteAddr & 0xFF00) >> 8);
  FLASH_RSByte(WriteAddr & 0xFF);

  if(NumByteToWrite > FLASH_PageSize)
  {
     NumByteToWrite = FLASH_PageSize;
     FLASH_ERROR("SPI_FLASH_PageWrite too large!");
  }

  /* 写入数据*/
  while (NumByteToWrite--)
  {
    FLASH_RSByte(*pBuffer);
    pBuffer++;
  }

  SPI_CSS_HIGH();

  /* 等待写入完毕*/
  FLASH_WaitForWriteEnd();
}

/*
*********************************************************************************************************
*  函 数 名: FLASH_BufferWrite
*  功能说明: 对FLASH写入数据，调用本函数写入数据前需要先擦除扇区
*  形    参: 	pBuffer: 要写入数据的指针
*				WriteAddr: 写入地址
*				NumByteToWrite: 写入数据长度
*  返 回 值: 无
*********************************************************************************************************
*/
void FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	uint8_t StartAddr, OneCount, PageUNum, PageModNum;
	StartAddr = WriteAddr % FLASH_PageSize;
	OneCount = FLASH_PageSize - StartAddr;
	PageUNum = NumByteToWrite / FLASH_PageSize;
	PageModNum = NumByteToWrite % FLASH_PageSize;
	if(StartAddr == 0)
	{
		while(PageUNum != 0)
		{
			FLASH_PageWrite(pBuffer, WriteAddr, FLASH_PageSize);
			PageUNum--;
			WriteAddr += FLASH_PageSize;
			pBuffer += FLASH_PageSize;
		}
		FLASH_PageWrite(pBuffer, WriteAddr, PageModNum);
	}
	else
	{
		if(PageUNum == 0)
		{
			/* 写第一页 */

			if(OneCount > PageModNum)
			{
				FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
			}
			else
			{
				FLASH_PageWrite(pBuffer, WriteAddr, OneCount);
				WriteAddr +=  OneCount;
				pBuffer += OneCount;
				
				FLASH_PageWrite(pBuffer, WriteAddr, PageModNum-OneCount);
			}
		
		}
		else
		{
			NumByteToWrite -= OneCount;
			PageUNum = NumByteToWrite / FLASH_PageSize;
			PageModNum = NumByteToWrite % FLASH_PageSize;
			
		    FLASH_PageWrite(pBuffer, WriteAddr, OneCount);
			WriteAddr +=  OneCount;
			pBuffer += OneCount;
			
			while(PageUNum != 0)
			{
				FLASH_PageWrite(pBuffer, WriteAddr, FLASH_PageSize);
				PageUNum--;
				WriteAddr += FLASH_PageSize;
				pBuffer += FLASH_PageSize;
			}
			FLASH_PageWrite(pBuffer, WriteAddr, PageModNum);
		}

	}

}

/*
*********************************************************************************************************
*  函 数 名: FLASH_BufferRead
*  功能说明: 读取FLASH数据
*  形    参: 	pBuffer: 存储读出数据的指针
*				ReadAddr: 读取地址
*				NumByteToRead: 读取数据长度
*  返 回 值: 无
*********************************************************************************************************
*/
void FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
    /* 选择FLASH: CS低电平 */
    SPI_CSS_LOW();

    /* 发送 读 指令 */
    FLASH_RSByte(W25X_ReadData);
    FLASH_RSByte((ReadAddr & 0xFF0000) >> 16);
    FLASH_RSByte((ReadAddr& 0xFF00) >> 8);
    FLASH_RSByte(ReadAddr & 0xFF);

    /* 读取数据 */
    while (NumByteToRead--)
    {
        *pBuffer = FLASH_RSByte(Dummy_Byte);
        pBuffer++;
    }

    /* 停止信号 FLASH: CS 高电平 */
    SPI_CSS_HIGH();
}


static uint8_t FLASH_TimeoutCallback(uint8_t Code)
{
	FLASH_ERROR("FLASH Error:%d\r\n", Code);
	return 0;
}

