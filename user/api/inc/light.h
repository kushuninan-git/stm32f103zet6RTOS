#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "stm32f10x.h"

void Light_Init(void);
// 获取光照传感器读取结果
void Light_ReadData(void);
// 调试打印函数
void Light_Debug(void);
extern u16 addbuff[2];
extern uint16_t light_val;

#endif
