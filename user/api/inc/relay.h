/**
 * @file relay.h
 * @brief 继电器驱动头文件
 * @note 编码格式：UTF-8
 * @note 硬件：STM32F10x，使用GPIO控制继电器
 */
#ifndef _RELAY_H_
#define _RELAY_H_

#include "stm32f10x.h"

/**
 * @brief 继电器初始化函数
 * @details 配置继电器控制引脚为推挽输出模式
 * @param 无
 * @return 无
 */
void RELAY_Init(void);

/**
 * @brief 继电器控制函数
 * @details 控制继电器的开关状态
 * @param state 继电器状态：0-关闭，1-打开
 * @return 无
 */
void RELAY(uint8_t state);

#endif
