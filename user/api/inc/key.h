/**
 * @file key.h
 * @brief 按键驱动头文件
 * @note 编码格式：UTF-8
 * @note 硬件：STM32F10x
 * @note 引脚：KEY1(PA0)、KEY2(PC4)、KEY3(PC5)、KEY4(PC6)
 */
#ifndef _KEY_H_
#define _KEY_H_

#include "stm32f10x.h"

// 按键参数配置
#define KEY_FILTER_TIME 20 // 消抖时间(ms)
#define KEY_LONG_TIME 800  // 长按判定时间(ms)

// 按键返回值定义
#define KEY_NONE 0   // 无按键
#define KEY1_PRESS 1 // KEY1短按
#define KEY1_LONG 2  // KEY1长按
#define KEY2_PRESS 3 // KEY2短按
#define KEY2_LONG 4  // KEY2长按
#define KEY3_PRESS 5 // KEY3短按
#define KEY3_LONG 6  // KEY3长按
#define KEY4_PRESS 7 // KEY4短按
#define KEY4_LONG 8  // KEY4长按

// 按键状态定义
typedef enum
{
    KEY_STATE_IDLE = 0,   // 空闲
    KEY_STATE_DEBOUNCE,   // 消抖中
    KEY_STATE_PRESSED,    // 已按下
    KEY_STATE_LONG_PRESS, // 长按中
    KEY_STATE_RELEASE     // 等待松开
} KeyState_t;

// 按键信息结构体
typedef struct
{
    GPIO_TypeDef *port; // GPIO端口
    uint16_t pin;       // GPIO引脚
    uint8_t active;     // 有效电平(0-低电平有效, 1-高电平有效)
    uint8_t id_short;   // 短按返回值的ID
    uint8_t id_long;    // 长按返回值的ID

    // 内部状态变量
    KeyState_t state;    // 当前状态
    uint16_t press_time; // 按下时刻的时间戳
} Key_t;

// 函数声明
void KEY_Init(void);    // 初始化
uint8_t KEY_Scan(void); // 扫描按键

#endif
