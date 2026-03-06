/**
 * @file pwm.h
 * @brief PWM输出驱动头文件
 * @note 编码格式：UTF-8
 * @note 硬件：STM32F10x，使用定时器生成PWM信号
 */
#ifndef _PWM_H_
#define _PWM_H_

#include "stm32f10x.h"

/**
 * @brief PWM初始化函数
 * @details 配置定时器生成PWM信号
 * @param 无
 * @return 无
 */
void Pwm_Init(void);

#endif
