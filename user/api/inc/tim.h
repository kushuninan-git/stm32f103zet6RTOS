#ifndef __TIM_H
#define __TIM_H
#include "stm32f10x.h"

/************************* TIM2核心配置参数 无简写 可直接修改 *************************/
#define TIM6_PRESCALER 71 // 预分频值，固定71，适配72MHz主频
#define TIM6_PERIOD 999   // 自动重装值，固定999，组合实现精准1ms中断

/************************* 全局变量声明 所有模块可引用 *************************/
extern volatile uint16_t sys_ms;

/************************* 函数声明 *************************/
void TIM_Init(void);

#endif
