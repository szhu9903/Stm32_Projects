#ifndef __BSP_DHT11_H_
#define __BSP_DHT11_H_

#include "stm32f10x.h"
#include "sys_delay.h"
#include "sys_config.h"

#define DHT11_HIGH  1
#define DHT11_LOW   0


/***************************** 引脚定义 *********************************/
#define DHT11_CLK     RCC_APB2Periph_GPIOB
#define DHT11_PIN     GPIO_Pin_9           
#define DHT11_PORT    GPIOB

typedef struct
{
	uint8_t  humi_int;		//湿度的整数部分
	uint8_t  humi_deci;	 	//湿度的小数部分
	uint8_t  temp_int;	 	//温度的整数部分
	uint8_t  temp_deci;	 	//温度的小数部分
	uint8_t  check_sum;	 	//校验和
}DHT11_Data_TypeDef;

extern DHT11_Data_TypeDef *dht11;

#define DHT11_DELAY_US(us)  	delay_us(us)
#define DHT11_DELAY_MS(ms)  	delay_ms(ms)

#define DHT11_DATA_OUT(a)		if (a)\
                                   GPIO_SetBits(DHT11_PORT,DHT11_PIN);\
								else \
                                   GPIO_ResetBits(DHT11_PORT,DHT11_PIN)

#define DHT11_DATA_IN()	  		GPIO_ReadInputDataBit(DHT11_PORT, DHT11_PIN)


/***************************** 函数定义 *********************************/
void DHT11_Init(void);
void DHT11_Read_Data(void);

#endif //__DHT11_H_
