/**
 * @file kqm.h
 * @brief 空气质量传感器驱动头文件
 * @note 编码格式：UTF-8
 * @note 硬件：STM32F10x，使用USART2与空气质量传感器通信
 * @note 引脚：PA2 (USART2_TX)、PA3 (USART2_RX)
 */
#ifndef __KQM_H
#define __KQM_H

#include "stm32f10x.h"

/**
 * @brief 空气质量传感器数据结构体
 * @details 存储传感器通信数据和解析结果
 */
typedef struct
{
    uint8_t rxbuff[10]; // 接收缓冲区，存储传感器原始数据
    uint8_t rxnum;      // 接收字节数
    uint8_t over;       // 接收完成标志（1=完成）
    float voc;          // VOC浓度（单位：mg/m³）
    float ch2o;         // 甲醛浓度（单位：mg/m³）
    double co2;         // CO2浓度（单位：ppm）
} KQM;

// 全局空气质量传感器数据结构体
extern KQM kqm;

/**
 * @brief 空气质量传感器初始化函数
 * @details 配置USART2串口通信，设置为9600波特率
 * @param 无
 * @return 无
 */
void Kqm_Init(void);

/**
 * @brief 空气质量传感器数据解析函数(重命名为读取数据)
 * @details 解析传感器返回的数据包，提取VOC、甲醛、CO2浓度数据
 * @param 无
 * @return 无，解析结果存储在全局kqm结构体中
 */
void Kqm_ReadData(void);

/**
 * @brief 空气质量传感器调试打印函数
 */
void Kqm_Debug(void);

#endif
