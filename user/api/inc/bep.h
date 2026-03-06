/**
 * @file bep.h
 * @brief 蜂鸣器驱动头文件
 * @note 编码格式：UTF-8
 * @note 硬件：STM32F10x，使用GPIOC控制蜂鸣器
 */
#ifndef _BEP_H_
#define _BEP_H_

#include "stm32f10x.h"

/**
 * @brief 蜂鸣器引脚定义
 */
#define BEEP_GPIO_PORT GPIOC
#define BEEP_GPIO_PIN GPIO_Pin_0
#define BEEP_GPIO_CLK RCC_APB2Periph_GPIOB

/**
 * @brief 蜂鸣器初始化函数
 * @details 配置GPIOC Pin0为推挽输出模式
 * @param 无
 * @return 无
 */
void Bep_Init(void);

/**
 * @brief 蜂鸣器控制函数
 * @details 控制蜂鸣器的开关状态
 * @param state 蜂鸣器状态：0-关闭，1-打开
 * @return 无
 */
void Bep(uint8_t state);

#endif
