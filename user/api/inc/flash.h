/**
 * @file flash.h
 * @brief W25Q64驱动头文件
 * @note 编码格式：UTF-8
 * @note
 */
#ifndef __FLASH_H__
#define __FLASH_H__

#include "stm32f10x.h"

/**
 * @brief W25Q64初始化函数
 * @details 初始化SPI2和FLASH引脚
 * @param 无
 * @return 无
 */
void Flash_Init(void);

/**
 * @brief 发送并接收一个字节数据
 * @details 等待发送缓冲区为空，确保之前的数据已经发送完成；
 * 发送数据到SPI2的数据寄存器，数据会被自动从发送缓冲区传输到SPI2的MOSI引脚；
 * 等待接收缓冲区非空，确保从设备已经发送了数据到MISO引脚；
 * 从接收缓冲区读取数据并返回。
 * @param data 要发送的数据
 * @return u8 接收到的数据
 */
u8 SPI_SendAndRecive(u8 data);

/**
 * @brief 从W25Q64芯片读取数据
 * @details 拉低片选信号，选中W25Q64芯片；
 * 发送03H；
 * 发送24位地址；
 * 接收数据，存储到buff缓冲区；
 * 拉高片选信号，取消选中W25Q64芯片。
 * @param buff 存储读取数据的缓冲区
 * @param len 要读取的字节数
 * @param addr 要读取的起始地址
 */
void Flash_ReadData(u8 buff[], u8 len, u32 addr);
/**
 * @brief 写数据到W25Q64芯片
 * @details 拉低片选信号，选中W25Q64芯片；
 * 发送02H；
 * 发送24位地址；
 * 发送数据，从buff缓冲区读取数据；
 * 拉高片选信号，取消选中W25Q64芯片。
 * @param buff 存储要写入数据的缓冲区
 * @param len 要写入的字节数
 * @param addr 要写入的起始地址
 */
void Flash_WriteData(u8 buff[], u8 len, u32 addr);
/**
 * @brief 使能W25Q64写操作
 * @details 拉低片选信号，选中W25Q64芯片；
 * 发送06H；
 * 拉高片选信号，取消选中W25Q64芯片。
 * @param 无
 * @return 无
 */
void Flash_WriteEnable(void);
/**
 * @brief 擦除W25Q64芯片扇区擦除
 * @details 拉低片选信号，选中W25Q64芯片；
 * 发送20H；
 * 发送24位地址；
 * 拉高片选信号，取消选中W25Q64芯片。
 * @param addr 要擦除的起始地址
 */
void Flash_Delete(u32 addr);

#endif
