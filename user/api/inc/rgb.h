/**
 * @file rgb.h
 * @brief RGB LED驱动头文件
 * @note 编码格式：UTF-8
 * @note 硬件：STM32F10x，使用GPIOA控制RGB LED
 * @note 引脚：PA6 (蓝)、PA7 (绿)、PA8 (红)
 */
#ifndef _RGB_H_
#define _RGB_H_

#include "stm32f10x.h"

/**
 * @brief RGB LED蓝色通道引脚定义
 * @details GPIOA Pin6
 */
#define RGB_B_GPIO_PORT GPIOA
#define RGB_B_GPIO_PIN GPIO_Pin_6
#define RGB_B_GPIO_CLK RCC_APB2Periph_GPIOA

/**
 * @brief RGB LED绿色通道引脚定义
 * @details GPIOA Pin7
 */
#define RGB_G_GPIO_PORT GPIOA
#define RGB_G_GPIO_PIN GPIO_Pin_7
#define RGB_G_GPIO_CLK RCC_APB2Periph_GPIOA

/**
 * @brief RGB LED红色通道引脚定义
 * @details GPIOA Pin8
 */
#define RGB_R_GPIO_PORT GPIOA
#define RGB_R_GPIO_PIN GPIO_Pin_8
#define RGB_R_GPIO_CLK RCC_APB2Periph_GPIOA

/**
 * @brief RGB LED初始化函数
 * @details 配置RGB LED的三个通道引脚为推挽输出模式
 * @param 无
 * @return 无
 */
void RGB_Init(void);

/**
 * @brief 控制RGB LED蓝色通道
 * @details 设置蓝色通道的开关状态
 * @param state 状态：0-关闭，1-打开
 * @return 无
 */
void RGB_B(uint8_t state);

/**
 * @brief 控制RGB LED绿色通道
 * @details 设置绿色通道的开关状态
 * @param state 状态：0-关闭，1-打开
 * @return 无
 */
void RGB_G(uint8_t state);

/**
 * @brief 控制RGB LED红色通道
 * @details 设置红色通道的开关状态
 * @param state 状态：0-关闭，1-打开
 * @return 无
 */
void RGB_R(uint8_t state);

#endif
