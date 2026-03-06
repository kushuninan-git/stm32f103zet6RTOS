#ifndef __BH1750_H
#define __BH1750_H

// 引入STM32F1标准库核心头文件，提供外设寄存器、函数定义
#include "stm32f10x.h"
// 引入I2C头文件，提供I2C相关函数定义
#include "stm32f10x_i2c.h"
// 引入延时头文件，需自行实现delay_ms()，BH1750转换需要延时等待
#include "delay.h"

/********************* BH1750 核心参数宏定义（结合I2C协议） *********************/
// BH1750 7位基础地址(ADDR接GND)
// 原因：I2C协议规定8位通信地址格式为「7位器件地址+1位读写位」，STM32硬件I2C要求传入7位地址，
// 硬件会自动拼接读写位（写=0/读=1），因此直接使用7位基础地址
#define BH1750_ADDR 0x46
// 注：使用时直接传入BH1750_ADDR，STM32硬件会自动处理读写位
// I2C通信超时时间（约10ms@72MHz）
// 原因：I2C通信故障时需要合理的超时时间，避免程序死锁，同时给硬件足够的响应时间
#define BH1750_I2C_TIMEOUT 0xFFFF
// BH1750连续高分辨率模式指令（推荐）
// 原因：此模式下测量分辨率1lx，测量时间120ms，可连续采集，适配绝大多数场景，符合I2C从机指令规范
#define BH1750_MODE_H 0x10
// 连续低分辨率模式（4lx，16ms）、一次高分辨率模式（测一次后断电），按需选择
#define BH1750_MODE_L 0x13
#define BH1750_MODE_ONE_H 0x20

/********************* STM32硬件I2C1配置宏（结合芯片引脚映射） *********************/
#define BH1750_I2C I2C1                          // 选用I2C1外设（STM32F103主流选择）
#define BH1750_I2C_CLK RCC_APB1Periph_I2C1       // I2C1挂载在APB1总线，对应时钟使能位
#define BH1750_I2C_GPIO_CLK RCC_APB2Periph_GPIOB // GPIOB挂载在APB2总线，对应时钟使能位
#define BH1750_SCL_PIN GPIO_Pin_6                // PB6 → I2C1_SCL，原因：STM32F103硬件I2C1默认复用引脚
#define BH1750_SDA_PIN GPIO_Pin_7                // PB7 → I2C1_SDA，硬件I2C必须使用芯片指定的复用引脚，否则外设无法控制引脚
#define BH1750_GPIO_PORT GPIOB                   // SCL/SDA对应GPIOB口

/********************* 函数声明 *********************/
void BH1750_I2C1_Init(void);             // 硬件I2C1初始化：配置GPIO+I2C外设参数，为I2C通信做硬件准备
ErrorStatus BH1750_HW_Init(void);           // BH1750传感器初始化
void BH1750_HW_ReadData(void); // 读取光照强度

/**
 * @brief BH1750调试打印函数
 */
void BH1750_HW_Debug(void);
extern float bh1750_hw_lux;

#endif
