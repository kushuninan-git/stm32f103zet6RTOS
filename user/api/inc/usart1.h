#ifndef __USART1_H
#define __USART1_H

#include "stm32f10x.h"
#include <stdio.h>

/* ----------------- 配置参数 ----------------- */
#define USART1_RX_BUF_SIZE 256

/* ----------------- 数据结构 ----------------- */
typedef struct
{
    uint8_t rx_buf[USART1_RX_BUF_SIZE];
    uint16_t rx_len;
    uint8_t rx_over;
} USART1_Data_t;

/* ----------------- 全局变量 ----------------- */
extern USART1_Data_t usart1_data;

/* ----------------- 函数声明 ----------------- */
void USART1_Init(void);
void USART1_SendByte(uint8_t data);
void USART1_SendString(const char *str);
void USART1_SendArray(uint8_t *arr, uint16_t len);
void USART1_ClearRxBuffer(void);

#endif /* __USART1_H */
