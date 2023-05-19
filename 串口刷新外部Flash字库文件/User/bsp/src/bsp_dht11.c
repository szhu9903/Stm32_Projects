#include "bsp_dht11.h"

static DHT11_Data_TypeDef myDHT11_Data = {0};

static void DHT11_Mode_IPU(void);
static void DHT11_Mode_Out_PP(void);
static uint8_t DHT11_Read_Byte(void);

/*
*********************************************************************************************************
*  �� �� ��: DHT11_Mode_IPU
*  ����˵��: ʹDHT11-DATA���ű�Ϊ��������ģʽ
*  ��    ��: ��
*  �� �� ֵ: ��
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
*  �� �� ��: DHT11_Mode_Out_PP
*  ����˵��: ʹDHT11-DATA���ű�Ϊ�������ģʽ
*  ��    ��: ��
*  �� �� ֵ: ��
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
*  �� �� ��: DHT11_Init
*  ����˵��: ����DHT11 GPIO
*  ��    ��: ��
*  �� �� ֵ: ��
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
*  �� �� ��: DHT11_Read_Byte
*  ����˵��: ��DHT11��ȡһ���ֽ�
*  ��    ��: ��
*  �� �� ֵ: ��
*********************************************************************************************************
*/
static uint8_t DHT11_Read_Byte(void)
{
	uint8_t i, temp=0;

	for(i=0;i<8;i++)    
	{	 
		/* ÿbit��50us�͵�ƽ���ÿ�ʼ����ѯֱ���ӻ����� ��50us �͵�ƽ ����*/  
		while(DHT11_DATA_IN()==Bit_RESET);

		/*DHT11 ��26~28us�ĸߵ�ƽ��ʾ��0������70us�ߵ�ƽ��ʾ��1����
		 *ͨ����� x us��ĵ�ƽ��������������״ ��x ���������ʱ 
		 */
		DHT11_DELAY_US(40); //��ʱx us �����ʱ��Ҫ��������0������ʱ�伴��	   	  

		if(DHT11_DATA_IN()==Bit_SET)/* x us����Ϊ�ߵ�ƽ��ʾ���ݡ�1�� */
		{
			/* �ȴ�����1�ĸߵ�ƽ���� */
			while(DHT11_DATA_IN()==Bit_SET);

			temp|=(uint8_t)(0x01<<(7-i));  //�ѵ�7-iλ��1��MSB���� 
		}
		else	 // x us��Ϊ�͵�ƽ��ʾ���ݡ�0��
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); //�ѵ�7-iλ��0��MSB����
		}
	}
	return temp;
}

/*
*********************************************************************************************************
*  �� �� ��: DHT11_Read_Data
*  ����˵��: һ�����������ݴ���Ϊ40bit��8bit ʪ������ + 8bit ʪ��С�� + 8bit �¶����� + 8bit �¶�С�� + 8bit У��� 
*  ��    ��: 	DHT11_Data: �洢���ݽṹ��
*  �� �� ֵ: ��
*********************************************************************************************************
*/
void DHT11_Read_Data(void)
{  
	uint16_t count;
	/*���ģʽ*/
	DHT11_Mode_Out_PP();
	/*��������*/
	DHT11_DATA_OUT(DHT11_LOW);
	/*��ʱ18ms*/
	DHT11_DELAY_US(20000);

	/*�������� ������ʱ30us*/
	DHT11_DATA_OUT(DHT11_HIGH); 

	DHT11_DELAY_US(30);   //��ʱ30us

	/*������Ϊ���� �жϴӻ���Ӧ�ź�*/ 
	DHT11_Mode_IPU();

	/*�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������*/   
	if(DHT11_DATA_IN()==Bit_RESET)     
	{
		count=0;
		/*��ѯֱ���ӻ����� ��80us �͵�ƽ ��Ӧ�źŽ���*/  
		while(DHT11_DATA_IN()==Bit_RESET)
		{
			count++;
			if(count>1000)  return ;
			DHT11_DELAY_US(10); 
		}  
		
		count=0;
		/*��ѯֱ���ӻ������� 80us �ߵ�ƽ �����źŽ���*/
		while(DHT11_DATA_IN()==Bit_SET)
		{
			count++;
			if(count>1000)  return ;
			DHT11_DELAY_US(10); 
		}
		
		/*��ʼ��������*/   
		dht11->humi_int= DHT11_Read_Byte();
		dht11->humi_deci= DHT11_Read_Byte();
		dht11->temp_int= DHT11_Read_Byte();
		dht11->temp_deci= DHT11_Read_Byte();
		dht11->check_sum= DHT11_Read_Byte();
		
		/*��ȡ���������Ÿ�Ϊ���ģʽ*/
		DHT11_Mode_Out_PP();
		/*��������*/
		DHT11_DATA_OUT(DHT11_HIGH);

		/*����ȡ�������Ƿ���ȷ*/
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

