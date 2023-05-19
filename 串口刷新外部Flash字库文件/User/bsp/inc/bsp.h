/*
*********************************************************************************************************
*
*    ģ������ : BSPģ��
*    �ļ����� : bsp.h
*    ˵    �� : ���ǵײ�����ģ�����е�h�ļ��Ļ����ļ��� Ӧ�ó���ֻ�� #include bsp.h ���ɣ�
*              ����Ҫ#include ÿ��ģ��� h �ļ�
*    
*    Copyright (C), 2013-2014, ���������� www.armfly.com
*    
*********************************************************************************************************
*/

#ifndef __BSP_H__
#define __BSP_H__

#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_beep.h"
#include "bsp_usart1.h"
#include "bsp_exti.h"
#include "bsp_at24c02.h"
#include "bsp_flash.h"
#include "bsp_relay.h"
#include "bsp_dht11.h"

/* �ṩ������C�ļ����õĺ��� */
void bsp_Init(void);
void bsp_Idle(void);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
