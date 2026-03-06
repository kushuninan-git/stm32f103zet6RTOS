/**
 * @file dht11.h
 * @brief DHT11温湿度传感器驱动头文件
 * @note 编码格式：UTF-8
 * @note 硬件：STM32F10x，DHT11数据引脚接GPIOG Pin11
 */
#ifndef _DHT11_H_
#define _DHT11_H_

#include "stm32f10x.h"

// DHT11引脚输出低电平：复位GPIOG Pin11
#define DHT_OUT_L GPIO_WriteBit(GPIOG, GPIO_Pin_11, Bit_RESET)
// DHT11引脚输出高电平：置位GPIOG Pin11
#define DHT_OUT_H GPIO_WriteBit(GPIOG, GPIO_Pin_11, Bit_SET)
// 读取DHT11引脚输入电平：读取GPIOG Pin11状态
#define DHT_IN GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11)

// DhT11传感器结构体：存储温湿度数据
typedef struct
{
    float tem; // 温度值（单位：℃）
    float hum; // 湿度值（单位：%RH）
} DHT;
extern DHT dht;
// 函数声明：读取DHT11温湿度数据
void Dht11_ReadData(void);
// 函数声明：初始化DHT11引脚
void Dht11_Init(void);

#endif
