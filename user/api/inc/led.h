/**
 * @file led.h
 * @brief LED指示灯驱动头文件
 * @note 编码格式：UTF-8
 * @note 硬件：STM32F10x，使用GPIOE控制LED指示灯
 * @note 引脚：PE2 (LED1)、PE3 (LED2)、PE4 (LED3)、PE5 (LED4)
 * @note 逻辑：低电平亮，高电平灭
 */
#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"

/**
 * @brief LED1引脚定义
 * @details GPIOE Pin2
 */
#define LED1_GPIO_PORT GPIOE
#define LED1_GPIO_PIN GPIO_Pin_2

/**
 * @brief LED2引脚定义
 * @details GPIOE Pin3
 */
#define LED2_GPIO_PORT GPIOE
#define LED2_GPIO_PIN GPIO_Pin_3

/**
 * @brief LED3引脚定义
 * @details GPIOE Pin4
 */
#define LED3_GPIO_PORT GPIOE
#define LED3_GPIO_PIN GPIO_Pin_4

/**
 * @brief LED4引脚定义
 * @details GPIOE Pin5
 */
#define LED4_GPIO_PORT GPIOE
#define LED4_GPIO_PIN GPIO_Pin_5

/**
 * @brief LED1控制宏函数
 * @details 控制LED1的亮、灭、翻转
 */
#define LED1_ON() GPIO_ResetBits(LED1_GPIO_PORT, LED1_GPIO_PIN)                                                                           // 亮：置低电平
#define LED1_OFF() GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN)                                                                            // 灭：置高电平
#define LED1_TOGGLE() GPIO_WriteBit(LED1_GPIO_PORT, LED1_GPIO_PIN, (BitAction)(1 - GPIO_ReadInputDataBit(LED1_GPIO_PORT, LED1_GPIO_PIN))) // 翻转电平

/**
 * @brief LED2控制宏函数
 * @details 控制LED2的亮、灭、翻转
 */
#define LED2_ON() GPIO_ResetBits(LED2_GPIO_PORT, LED2_GPIO_PIN)
#define LED2_OFF() GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN)
#define LED2_TOGGLE() GPIO_WriteBit(LED2_GPIO_PORT, LED2_GPIO_PIN, (BitAction)(1 - GPIO_ReadInputDataBit(LED2_GPIO_PORT, LED2_GPIO_PIN)))

/**
 * @brief LED3控制宏函数
 * @details 控制LED3的亮、灭、翻转
 */
#define LED3_ON() GPIO_ResetBits(LED3_GPIO_PORT, LED3_GPIO_PIN)
#define LED3_OFF() GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN)
#define LED3_TOGGLE() GPIO_WriteBit(LED3_GPIO_PORT, LED3_GPIO_PIN, (BitAction)(1 - GPIO_ReadInputDataBit(LED3_GPIO_PORT, LED3_GPIO_PIN)))

/**
 * @brief LED4控制宏函数
 * @details 控制LED4的亮、灭、翻转
 */
#define LED4_ON() GPIO_ResetBits(LED4_GPIO_PORT, LED4_GPIO_PIN)
#define LED4_OFF() GPIO_SetBits(LED4_GPIO_PORT, LED4_GPIO_PIN)
#define LED4_TOGGLE() GPIO_WriteBit(LED4_GPIO_PORT, LED4_GPIO_PIN, (BitAction)(1 - GPIO_ReadInputDataBit(LED4_GPIO_PORT, LED4_GPIO_PIN)))

/************************* 函数声明 *************************/

/**
 * @brief LED初始化函数
 * @details 配置GPIOE的LED引脚为推挽输出模式
 * @param 无
 * @return 无
 */
void Led_Init(void);

#endif
