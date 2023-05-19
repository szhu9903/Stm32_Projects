/*
*********************************************************************************************************
*
*    模块名称 : BSP模块
*    文件名称 : bsp.h
*    说    明 : 这是底层驱动模块所有的h文件的汇总文件。 应用程序只需 #include bsp.h 即可，
*              不需要#include 每个模块的 h 文件
*    
*    Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
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

/* 提供给其他C文件调用的函数 */
void bsp_Init(void);
void bsp_Idle(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
