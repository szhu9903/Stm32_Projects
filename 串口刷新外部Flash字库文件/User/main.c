#include "main.h"


int main(void)
{
//	uint8_t Tx_Buffer[10];
	uint8_t Rx_Buffer[20];
//	Tx_Buffer[0] = 0x82;
//	Tx_Buffer[1] = 0x93;
//	Tx_Buffer[2] = 0x04;
//	Tx_Buffer[5] = 0x14;
	
	delay_init();
	bsp_Init();
	
	DHT11_Read_Data();
	delay_ms(500);
	
	printf("init OK!\r\n");
//	Beep_OK();
	
	
	/* Flash 测试 */
	uint32_t fid = FLASH_ReadID();
	printf("flash cs %lx \r\n", fid);
//	
//	FLASH_ChipErase();
	
//	/* 将发送缓冲区的数据写到flash中 */
//	FLASH_BufferWrite(Tx_Buffer, 0, 10);
//	printf("\r\n 写入的数据为：%x, %x, %x, %x \r\t", Tx_Buffer[0], Tx_Buffer[1], Tx_Buffer[2], Tx_Buffer[3]);

	/* 将刚刚写入的数据读出来放到接收缓冲区中 */
	FLASH_BufferRead(Rx_Buffer, 257*256, 20);
	printf("\r\n 读出的数据为：%x, %x, %x, %x \r\n", Rx_Buffer[0], Rx_Buffer[1], Rx_Buffer[2], Rx_Buffer[3]);
	printf("\r\n 读出的数据为：%x, %x, %x, %x \r\n", Rx_Buffer[4], Rx_Buffer[5], Rx_Buffer[6], Rx_Buffer[7]);
	printf("\r\n 读出的数据为：%x, %x, %x, %x \r\n", Rx_Buffer[8], Rx_Buffer[9], Rx_Buffer[10], Rx_Buffer[11]);
	printf("\r\n 读出的数据为：%x, %x, %x, %x \r\n", Rx_Buffer[12], Rx_Buffer[13], Rx_Buffer[14], Rx_Buffer[15]);
	while(1)
	{


	}
}
