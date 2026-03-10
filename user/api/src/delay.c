#include "delay.h"

// usÑÓÊ±
void Delay_nus(uint64_t time)
{
	while (time--)
	{
		Delay_1us();
	}
}

// msÑÓÊ±
void Delay_nms(uint64_t time)
{
	time *= 1000;
	while (time--)
	{
		Delay_1us();
	}
}
