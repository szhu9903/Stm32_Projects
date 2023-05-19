#include "bsp_dht11.h"

static DHT11_Data_TypeDef myDHT11_Data = {0};

static void DHT11_Mode_IPU(void);
static void DHT11_Mode_Out_PP(void);
static uint8_t DHT11_Read_Byte(void);

/*
*********************************************************************************************************
*  函 数 名: DHT11_Mode_IPU
*  功能说明: 使DHT11-DATA引脚变为上拉输入模式
*  形    参: 无
*  返 回 值: 无
*********************************************************************************************************
*/
static void DHT11_Mode_IPU(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 
}

/*
*********************************************************************************************************
*  函 数 名: DHT11_Mode_Out_PP
*  功能说明: 使DHT11-DATA引脚变为推挽输出模式
*  形    参: 无
*  返 回 值: 无
*********************************************************************************************************
*/
static void DHT11_Mode_Out_PP(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
													   
	GPIO_InitStructure.GPIO_Pin = DHT11_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(DHT11_PORT, &GPIO_InitStructure);	 	 
}

/*
*********************************************************************************************************
*  函 数 名: DHT11_Init
*  功能说明: 配置DHT11 GPIO
*  形    参: 无
*  返 回 值: 无
*********************************************************************************************************
*/
void DHT11_Init(void)
{		

	RCC_APB2PeriphClockCmd(DHT11_CLK, ENABLE); 
	
	DHT11_Mode_Out_PP();
	
	dht11=&myDHT11_Data;
	
}

/*
*********************************************************************************************************
*  函 数 名: DHT11_Read_Byte
*  功能说明: 从DHT11读取一个字节
*  形    参: 无
*  返 回 值: 无
*********************************************************************************************************
*/
static uint8_t DHT11_Read_Byte(void)
{
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	 
		/* 每bit以50us低电平标置开始，轮询直到从机发出 的50us 低电平 结束*/  
		while(DHT11_DATA_IN()==Bit_RESET);

		/*DHT11 以26~28us的高电平表示“0”，以70us高电平表示“1”，
		 *通过检测 x us后的电平即可区别这两个状 ，x 即下面的延时 
		 */
		DHT11_DELAY_US(40); //延时x us 这个延时需要大于数据0持续的时间即可	   	  

		if(DHT11_DATA_IN()==Bit_SET)/* x us后仍为高电平表示数据“1” */
		{
			/* 等待数据1的高电平结束 */
			while(DHT11_DATA_IN()==Bit_SET);

			temp|=(uint8_t)(0x01<<(7-i));  //把第7-i位置1，MSB先行 
		}
		else	 // x us后为低电平表示数据“0”
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); //把第7-i位置0，MSB先行
		}
	}
	return temp;
}

/*
*********************************************************************************************************
*  函 数 名: DHT11_Read_Data
*  功能说明: 一次完整的数据传输为40bit，8bit 湿度整数 + 8bit 湿度小数 + 8bit 温度整数 + 8bit 温度小数 + 8bit 校验和 
*  形    参: 	DHT11_Data: 存储数据结构体
*  返 回 值: 无
*********************************************************************************************************
*/
void DHT11_Read_Data(void)
{  
	uint16_t count;
	/*输出模式*/
	DHT11_Mode_Out_PP();
	/*主机拉低*/
	DHT11_DATA_OUT(DHT11_LOW);
	/*延时18ms*/
	DHT11_DELAY_US(20000);

	/*总线拉高 主机延时30us*/
	DHT11_DATA_OUT(DHT11_HIGH); 

	DHT11_DELAY_US(30);   //延时30us

	/*主机设为输入 判断从机响应信号*/ 
	DHT11_Mode_IPU();

	/*判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行*/   
	if(DHT11_DATA_IN()==Bit_RESET)     
	{
		count=0;
		/*轮询直到从机发出 的80us 低电平 响应信号结束*/  
		while(DHT11_DATA_IN()==Bit_RESET)
		{
			count++;
			if(count>1000)  return ;
			DHT11_DELAY_US(10); 
		}  
		
		count=0;
		/*轮询直到从机发出的 80us 高电平 标置信号结束*/
		while(DHT11_DATA_IN()==Bit_SET)
		{
			count++;
			if(count>1000)  return ;
			DHT11_DELAY_US(10); 
		}
		
		/*开始接收数据*/   
		dht11->humi_int= DHT11_Read_Byte();
		dht11->humi_deci= DHT11_Read_Byte();
		dht11->temp_int= DHT11_Read_Byte();
		dht11->temp_deci= DHT11_Read_Byte();
		dht11->check_sum= DHT11_Read_Byte();
		
		/*读取结束，引脚改为输出模式*/
		DHT11_Mode_Out_PP();
		/*主机拉高*/
		DHT11_DATA_OUT(DHT11_HIGH);

		/*检查读取的数据是否正确*/
//		if(dht11->check_sum == dht11->humi_int + dht11->humi_deci + dht11->temp_int+ dht11->temp_deci)
//			return TRUE;
//		else 
//			return FALSE;
	}
	else
	{		
//		return FALSE;
	}   
}

