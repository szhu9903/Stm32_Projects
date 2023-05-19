#ifndef __BSP_AT24C02_H__
#define __BSP_AT24C02_H__

#include "stm32f10x.h"
#include <stdio.h>

/* AT24C01/02每页有8个字节 */
#define I2C_PageSize           8

/***************************** 引脚定义 *********************************/
#define AT24C02_I2C                      	I2C1
#define AT24C02_STM32_ADDRESS7      		0X98
#define AT24C02_ADDRESS						0xA0
#define AT24C02_I2C_CLK                 	RCC_APB1Periph_I2C1
#define AT24C02_I2C_BAUDRATE				400000

#define AT24C02_I2C_SCL_PORT				GPIOB
#define AT24C02_I2C_SCL_CLK         		RCC_APB2Periph_GPIOB
#define AT24C02_I2C_SCL_PIN					GPIO_Pin_6

#define AT24C02_I2C_SDA_PORT				GPIOB
#define AT24C02_I2C_SDA_CLK         		RCC_APB2Periph_GPIOB
#define AT24C02_I2C_SDA_PIN					GPIO_Pin_7

/***************************** 超时等待 *********************************/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

/***************************** 调试 *********************************/
#define AT24C02_DEBUG_ON         0

#define AT24C02_INFO(fmt,arg...)           printf("<<-EEPROM-INFO->> "fmt"\n",##arg)
#define AT24C02_ERROR(fmt,arg...)          printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)
#define AT24C02_DEBUG(fmt,arg...)          do{\
                                          if(EEPROM_DEBUG_ON)\
                                          printf("<<-EEPROM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

/***************************** 函数定义 *********************************/
void AT24C02_Init(void);
uint32_t AT24C02_ByteWrite(u8 byte, u8 WriteAddr);
uint32_t AT24C02_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);

#endif
