/**
 * @file rtc.h
 * @brief 实时时钟(RTC)驱动头文件
 * @note 编码格式：UTF-8
 * @note 硬件：STM32F10x，使用内部RTC外设
 * @note 时钟源：优先使用LSE（外部32.768kHz晶振），失败则切换到LSI（内部40kHz晶振）
 */
#ifndef _RTC_H_
#define _RTC_H_

#include "stm32f10x.h"

extern struct tm *info;

/**
 * @brief RTC初始化函数
 * @details 配置RTC外设，设置时钟源和分频系数
 * @details 优先使用LSE时钟源，若LSE启动失败则切换到LSI
 * @param 无
 * @return 无
 */
void RTC_Init(void);

/**
 * @brief 获取当前时间函数
 * @details 从RTC获取当前时间戳，转换为本地时间并打印
 * @param 无
 * @return 无
 */
void RTC_GetTime(void);

/**
 * @brief 更新RTC时间函数
 * @details 设置RTC时间戳，同步RTC寄存器
 * @param sec 时间戳（秒数）
 * @return 无
 */
void RTC_UpData_Time(uint32_t sec);

#endif
