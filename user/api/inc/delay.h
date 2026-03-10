#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

enum sensor_id
{
	SENSOR_ID_KQM = 0,
	SENSOR_ID_DHT11,
	SENSOR_ID_LIGHT,
	SENSOR_ID_BH1750,
	SENSOR_ID_RTC,
};

typedef struct sensordata
{
	float kqmdata[3];
	float dht11data[2];
	uint16_t lightdata[2];
	float bh1750data;
	uint16_t rtcdata[7];

	uint8_t id;
} SENSORDATA;

#define Delay_1us() \
	{               \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
		__nop();    \
	}

void Delay_nus(uint64_t time); // º¯ÊýÉùÃ÷
void Delay_nms(uint64_t time);
#endif
