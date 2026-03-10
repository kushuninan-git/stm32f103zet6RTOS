#include "dht11.h"
DHT dht = {0};
// PG11  ¿ªÂ©
void Dht11_Init(void)
{
	// Ê±ÖÓ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	// IO
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;		  // Òý½Å
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; // ËÙ¶È
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  // Ä£Ê½
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}
#define DHT_OUT_L GPIO_WriteBit(GPIOG, GPIO_Pin_11, Bit_RESET)
#define DHT_OUT_H GPIO_WriteBit(GPIOG, GPIO_Pin_11, Bit_SET)
#define DHT_IN GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11)
#include "delay.h"
void Dht11_ReadData(void)
{
	// Í¨ÐÅ
	// ÆðÊ¼ MCU·¢ËÍ18-30msµÄµÍµçÆ½
	DHT_OUT_L;
	Delay_nms(25);
	DHT_OUT_H;
	// ÏìÓ¦ DHT11·¢ËÍ 83usµÍµçÆ½  87usµÄ¸ßµçÆ½
	uint8_t outtime = 0;
	while (DHT_IN == 1)
	{
		Delay_nus(1);
		outtime++;
		if (outtime > 100)
			return;
	}
	outtime = 0;
	while (DHT_IN == 0)
	{
		Delay_nus(1);
		outtime++;
		if (outtime > 100)
			return;
	}
	// Êý¾Ý´«Êä DHT11·¢ËÍ40Î»Êý¾Ý ¸ßÎ»ÏÈ´«
	uint8_t buff[5] = {0};
	for (uint8_t i = 0; i < 40; i++)
	{
		outtime = 0;
		while (DHT_IN == 1)
		{
			Delay_nus(1);
			outtime++;
			if (outtime > 100)
				return;
		}
		outtime = 0;
		while (DHT_IN == 0)
		{
			Delay_nus(1);
			outtime++;
			if (outtime > 100)
				return;
		}
		Delay_nus(40);

		if (DHT_IN == 1) // Êý¾Ý1
			buff[4 - i / 8] |= 0x1 << (7 - i % 8);
		else // Êý¾Ý0
			buff[4 - i / 8] &= ~(0x1 << (7 - i % 8));
	}

	// Êý¾Ý´¦Àí
	// Êý¾ÝÐ£Ñé 4:Êª¶ÈÕûÊý 3:Êª¶ÈÐ¡Êý 2:ÎÂ¶ÈÕûÊý  1:ÎÂ¶ÈÐ¡Êý 0£ºÐ£Ñé
	if (buff[4] + buff[3] + buff[2] + buff[1] != buff[0])
		return;
	// Êý¾Ý¼ÆËã
	if (buff[1] >= 128)
		dht.tem = (buff[2] + (buff[1] - 128) * 0.1) * -1;
	else
		dht.tem = (buff[2] + (buff[1]) * 0.1);

	dht.hum = buff[4];

	//	printf("tem:%.1f\thum:%.1f\r\n",dht.tem,dht.hum);
}
