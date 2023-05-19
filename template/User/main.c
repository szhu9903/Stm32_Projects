#include "main.h"


int main(void)
{
	
	delay_init();
	bsp_Init();
	
	LED_ON;
	while(1)
	{
		delay_ms(1000);
		LED_TOGGLE;
	}
}
