#ifndef __BSP__FLASH_H__
#define __BSP__FLASH_H__

#include "stm32f10x.h"
#include <stdio.h>

#define FLASH_PageSize              256

//引脚定义
/*******************************************************/
#define FLASH_SPI					    SPI2
#define FLASH_SPI_CLK					RCC_APB1Periph_SPI2
#define FLASH_SPI_CLK_CMD				RCC_APB1PeriphClockCmd

#define FLASH_SPI_SCLK_PORT				GPIOB
#define FLASH_SPI_SCLK_CLK         		RCC_APB2Periph_GPIOB
#define FLASH_SPI_SCLK_PIN				GPIO_Pin_13

#define FLASH_SPI_MISO_PORT				GPIOB
#define FLASH_SPI_MISO_CLK         		RCC_APB2Periph_GPIOB
#define FLASH_SPI_MISO_PIN				GPIO_Pin_14

#define FLASH_SPI_MOSI_PORT				GPIOB
#define FLASH_SPI_MOSI_CLK         		RCC_APB2Periph_GPIOB
#define FLASH_SPI_MOSI_PIN				GPIO_Pin_15
#define FLASH_SPI_MOSI_AF               GPIO_AF_SPI1
#define FLASH_SPI_MOSI_PINSOURCE		GPIO_PinSource5

#define FLASH_SPI_CSS_PORT				GPIOB
#define FLASH_SPI_CSS_CLK         		RCC_APB2Periph_GPIOB
#define FLASH_SPI_CSS_PIN				GPIO_Pin_12
/*******************************************************/

/* 控制开始 停止位 CSS电平 */
#define SPI_CSS_LOW()					GPIO_ResetBits(FLASH_SPI_CSS_PORT, FLASH_SPI_CSS_PIN)	
#define SPI_CSS_HIGH()					GPIO_SetBits(FLASH_SPI_CSS_PORT, FLASH_SPI_CSS_PIN)		

/*等待超时时间*/
#define SPI_FLAG_TIMEOUT         		((uint16_t)0x1000)

#define FLASH_DEBUG_ON         			0
#define FLASH_INFO(fmt,arg...)          printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)         printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)         do{\
                                        if(FLASH_DEBUG_ON)\
                                        printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                        }while(0)

										
/*FLASH常用命令*/
#define W25X_WriteEnable                0x06
#define W25X_WriteDisable               0x04
#define W25X_ReadStatusReg              0x05
#define W25X_WriteStatusReg             0x01
#define W25X_ReadData                   0x03
#define W25X_FastReadData               0x0B
#define W25X_FastReadDual               0x3B
#define W25X_PageProgram                0x02
#define W25X_BlockErase                 0xD8
#define W25X_SectorErase                0x20
#define W25X_ChipErase                  0xC7
#define W25X_PowerDown                  0xB9
#define W25X_ReleasePowerDown           0xAB
#define W25X_DeviceID                   0xAB
#define W25X_ManufactDeviceID           0x90
#define W25X_JedecDeviceID              0x9F
/*其它*/
#define sFLASH_ID                       0XEF4015
#define Dummy_Byte                      0xFF
										

void FLASH_Init(void);
uint8_t FLASH_RSByte(uint8_t Byte);
uint8_t FLASH_ReadByte(void);
uint32_t FLASH_ReadID(void);
void FLASH_WriteEnable(void);
void FLASH_WaitForWriteEnd(void);
void FLASH_SectorErase(u32 SectorAddr);
void FLASH_ChipErase(void);
void FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
void FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);

#endif 
