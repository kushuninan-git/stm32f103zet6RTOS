/**
 * @file Myi2c_bh1750.h
 * @brief 软件I2C实现的BH1750光照传感器驱动头文件
 * @note 编码格式：UTF-8
 * @note 硬件：STM32F10x，使用软件I2C与BH1750通信
 * @note 引脚：PB6 (SCL)、PB7 (SDA)
 */
#ifndef __MY_I2C_BH1750_H
#define __MY_I2C_BH1750_H

#include "stm32f10x.h"
#include "delay.h"

/**
 * @brief BH1750核心参数宏定义
 */
#define BH1750_DEVICE_ADDR 0x23           // BH1750器件地址（ADDR接GND时）
#define BH1750_MODE_CONTINUOUS_HIGH 0x10  // 连续高分辨率模式（1lx，120ms）
#define BH1750_MODE_CONTINUOUS_HIGH2 0x11 // 连续高分辨率模式2（0.5lx，120ms）
#define BH1750_MODE_CONTINUOUS_LOW 0x13   // 连续低分辨率模式（4lx，16ms）
#define BH1750_MODE_ONE_TIME_HIGH 0x20    // 一次高分辨率模式

/**
 * @brief 软件I2C引脚定义
 */
#define BH1750_I2C_PERIPH I2C1                    // 选用I2C1外设（STM32F103主流选择）
#define BH1750_I2C_PERIPH_CLK RCC_APB1Periph_I2C1 // I2C1挂载在APB1总线，对应时钟使能位
#define BH1750_I2C_GPIO_CLK RCC_APB2Periph_GPIOB  // GPIOB挂载在APB2总线，对应时钟使能位
#define BH1750_I2C_SCL_PIN GPIO_Pin_6             // PB6 → I2C1_SCL
#define BH1750_I2C_SDA_PIN GPIO_Pin_7             // PB7 → I2C1_SDA
#define BH1750_I2C_GPIO_PORT GPIOB                // SCL/SDA对应GPIOB口

/**
 * @brief I2C延时参数
 * @details 推荐值：72MHz时设置为5，约100KHz通信速率
 */
#define BH1750_I2C_DELAY_US 5

/**
 * @brief SCL引脚操作宏函数
 */
#define BH1750_I2C_SCL_HIGH() GPIO_SetBits(BH1750_I2C_GPIO_PORT, BH1750_I2C_SCL_PIN)
#define BH1750_I2C_SCL_LOW() GPIO_ResetBits(BH1750_I2C_GPIO_PORT, BH1750_I2C_SCL_PIN)

/**
 * @brief SDA引脚操作宏函数
 */
#define BH1750_I2C_SDA_HIGH() GPIO_SetBits(BH1750_I2C_GPIO_PORT, BH1750_I2C_SDA_PIN)
#define BH1750_I2C_SDA_LOW() GPIO_ResetBits(BH1750_I2C_GPIO_PORT, BH1750_I2C_SDA_PIN)
#define BH1750_I2C_SDA_READ() GPIO_ReadInputDataBit(BH1750_I2C_GPIO_PORT, BH1750_I2C_SDA_PIN)

/**
 * @brief I2C应答信号定义
 */
#define BH1750_I2C_ACK 0
#define BH1750_I2C_NACK 1

/**
 * @brief 软件I2C基础函数
 */
void BH1750_SoftI2C_Init(void);         // 初始化软件I2C引脚
void BH1750_I2C_Start(void);            // 发送I2C起始信号
void BH1750_I2C_Stop(void);             // 发送I2C停止信号
void BH1750_I2C_SendAck(uint8_t ack);   // 发送应答信号（0=ACK，1=NACK）
uint8_t BH1750_I2C_ReceiveAck(void);    // 接收应答信号
void BH1750_I2C_SendByte(uint8_t data); // 发送一个字节
uint8_t BH1750_I2C_ReceiveByte(void);   // 接收一个字节

/**
 * @brief BH1750传感器操作函数
 */
uint8_t BH1750_Sensor_Init(void);    // 初始化BH1750传感器，返回0成功，1失败
float BH1750_Sensor_ReadLight(void); // 读取光照强度（单位：lx），失败返回-1.0f
void BH1750_SetPowerDown(void);      // 设置传感器为断电模式
void BH1750_SetOneTimeMode(void);    // 设置传感器为一次测量模式
void BH1750_SetContinuousMode(void); // 设置传感器为连续测量模式
uint8_t BH1750_CheckSensor(void);    // 检测传感器是否在线，返回0在线，1离线

#endif /* __MY_I2C_BH1750_H */
