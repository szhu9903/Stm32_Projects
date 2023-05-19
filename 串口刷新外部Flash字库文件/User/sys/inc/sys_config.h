#ifndef __SYS_CONFIG_H
#define __SYS_CONFIG_H	 

#include "stm32f10x.h"

/***************************** 标志定义 *********************************/
#define FALSE 0
#define TRUE 1
#define ON  0
#define OFF 1

/***************************** 数据缓冲区 *********************************/
#define MAX_BUFF_SIZE 	1024

typedef struct
{
	char buff[MAX_BUFF_SIZE];
	union {
		__IO u16 InfAll;
		struct {
			  __IO u16 FramLength       :15;                               // 14:0 
			  __IO u16 FramFinishFlag   :1;                                // 15 
		  } InfBit;
	}; 
}USART_DATA_TypeDef;

/***************************** 帧类型 *********************************/
#define FRAME_TYPE_QUERY    	0x01
#define FRAME_TYPE_SET      	0x02
#define FRAME_TYPE_RESPONSE 	0x03

/***************************** 发送 指令类型 指令编码 *********************************/
#define CMD_TYPE_HEART_BEAT_DATA                                0x00
#define CMD_CODE_SEND_HEART_BEAT_DATA_TO_SERVER                 0x01
#define CMD_CODE_SET_HEART_BEAT_DATA_INTERVAL                   0x02

#define CMD_TYPE_SEND_LOGIN_DATA				                0x01
#define CMD_CODE_SEND_LOGIN_DATA_TO_SERVER                  	0x01

#define CMD_TYPE_LED_DATA									    0x02
#define CMD_CODE_SET_LED_COLOR									0x01

#define CMD_TYPE_DHT11_DATA									    0x03
#define CMD_CODE_SEND_DHT11_DATA_TO_SERVER						0x01
#define CMD_CODE_QUERY_DHT11_DATA								0x02

#define CMD_TYPE_RTC_DATA									    0x04
#define CMD_CODE_RTC_SET_DATETIME								0x01

#endif
