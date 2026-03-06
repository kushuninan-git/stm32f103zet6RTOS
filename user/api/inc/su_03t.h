/**
 * @file su_03t.h
 * @brief SU-03T语音模块驱动头文件
 * @note 编码格式：UTF-8
 * @note 硬件：STM32F10x，使用UART5与SU-03T语音模块通信
 * @note 引脚：PC12 (UART5_TX)、PD2 (UART5_RX)
 */
#ifndef __SU_03T_H
#define __SU_03T_H

#include "stm32f10x.h"

/**
 * @brief SU-03T语音模块数据结构体
 * @details 存储语音模块通信数据和解析结果
 */
typedef struct
{
    uint8_t rxbuff[3]; // 接收缓冲区，存储3字节数据（帧头+数据+校验和）
    uint8_t rxnum;     // 接收字节数
    uint8_t over;      // 接收完成标志（1=完成）
    uint8_t data;      // 解析后的有效数据
} SU03T;

// 全局语音模块数据结构体
extern SU03T su03t;

/**
 * @brief SU-03T语音模块初始化函数
 * @details 配置UART5串口通信，设置为9600波特率
 * @param 无
 * @return 无
 */
void Su03t_Init(void);

/**
 * @brief SU-03T发送单个字节函数
 * @details 向语音模块发送单个字节数据
 * @param txdata 要发送的字节数据
 * @return 无
 */
void Su03t_SendByte(uint8_t txdata);

/**
 * @brief SU-03T发送字节数组函数
 * @details 向语音模块发送指定长度的字节数组
 * @param Array 要发送的字节数组
 * @param Length 数组长度
 * @return 无
 */
void Su03t_SendArray(uint8_t *Array, uint16_t Length);

/**
 * @brief SU-03T发送字符串函数
 * @details 向语音模块发送以\0结尾的字符串
 * @param str 要发送的字符串
 * @return 无
 */
void Su03t_SendString(uint8_t *str);

/**
 * @brief SU-03T数据解析状态枚举
 */
typedef enum
{
    SU03T_OK = 0,       // 解析成功
    SU03T_NO_DATA,      // 无数据或未接收完成
    SU03T_ERR_HEADER,   // 帧头错误
    SU03T_ERR_CHECKSUM, // 校验和错误
    SU03T_ERR_LEN       // 数据长度错误
} SU03T_Status_t;

/**
 * @brief SU-03T语音模块数据解析函数
 * @details 解析语音模块返回的数据包，提取有效指令数据
 * @param 无
 * @return 解析状态 SU03T_Status_t
 */
SU03T_Status_t Su03t_ReadData(void);

/**
 * @brief SU-03T调试打印函数
 */
void Su03t_Debug(void);

/**
 * @brief SU-03T语音命令处理函数
 * @details 根据语音模块返回的数据执行相应操作（如控制LED、发送传感器数据等）
 */
void Su03t_Handle_Command(void);

#endif
