/*
 * bsp_lcd.h
 * 液晶显示驱动头文件
 * 包含LCD初始化、显示控制等相关函数声明和宏定义
 */

#ifndef __BSP_LCD_H
#define __BSP_LCD_H

#include "stm32f10x.h"

// LCD参数结构体
typedef struct
{
	uint16_t width;	  // LCD 宽度
	uint16_t height;  // LCD 高度
	uint16_t id;	  // LCD ID
	uint8_t dir;	  // 横屏还是竖屏控制：0，竖屏；1，横屏。
	uint16_t wramcmd; // 开始写gram指令
	uint16_t setxcmd; // 设置x坐标指令
	uint16_t setycmd; // 设置y坐标指令
} _lcd_dev;

// LCD设备结构体 extern声明
extern _lcd_dev lcddev;

// 画笔颜色和背景色 extern声明
extern uint16_t POINT_COLOR;
extern uint16_t BACK_COLOR;

// LCD 背光控制宏定义
#define LCD_LED(x) (x ? (GPIO_SetBits(GPIOB, GPIO_Pin_0)) : (GPIO_ResetBits(GPIOB, GPIO_Pin_0)))

// LCD的FSMC地址结构体
typedef struct
{
	volatile uint16_t LCD_REG; // LCD寄存器地址
	volatile uint16_t LCD_RAM; // LCD RAM地址
} LCD_TypeDef;

// 使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A10作为数据命令区分线
// 注意设置时STM32内部会右移一位对其!
#define LCD_BASE ((uint32_t)(0x6C000000 | 0x000007FE)) // 0111 1111 1110  >>1      011 1111 1111
#define LCD ((LCD_TypeDef *)LCD_BASE)				   // 1000 0000 0000  >>1			 100 0000 0000

// LCD屏幕扫描方向定义
#define L2R_U2D 0 // 从左到右,从上到下
#define L2R_D2U 1 // 从左到右,从下到上
#define R2L_U2D 2 // 从右到左,从上到下
#define R2L_D2U 3 // 从右到左,从下到上

#define U2D_L2R 4 // 从上到下,从左到右
#define U2D_R2L 5 // 从上到下,从右到左
#define D2U_L2R 6 // 从下到上,从左到右
#define D2U_R2L 7 // 从下到上,从右到左

#define DFT_SCAN_DIR L2R_U2D // 默认的扫描方向

// 画笔颜色宏定义
#define WHITE 0xFFFF   // 白色
#define BLACK 0x0000   // 黑色
#define BLUE 0x001F	   // 蓝色
#define BRED 0XF81F	   // 红蓝色
#define GRED 0XFFE0	   // 绿红色
#define GBLUE 0X07FF   // 绿蓝色
#define RED 0xF800	   // 红色
#define MAGENTA 0xF81F // 洋红色
#define GREEN 0x07E0   // 绿色
#define CYAN 0x7FFF	   // 青色
#define YELLOW 0xFFE0  // 黄色
#define BROWN 0XBC40   // 棕色
#define BRRED 0XFC07   // 棕红色
#define GRAY 0X8430	   // 灰色

// GUI颜色宏定义
#define DARKBLUE 0X01CF	 // 深蓝色
#define LIGHTBLUE 0X7D7C // 浅蓝色
#define GRAYBLUE 0X5458	 // 灰蓝色
// 以上三色为PANEL的颜色

#define LIGHTGREEN 0X841F // 浅绿色
// #define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 0XC618 // 浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE 0XA651 // 浅灰蓝色(中间层颜色)
#define LBBLUE 0X2B12	 // 浅棕蓝色(选择条目的反色)

// SSD LCD分辨率设置
#define SSD_HOR_RESOLUTION 800 // LCD水平分辨率
#define SSD_VER_RESOLUTION 480 // LCD垂直分辨率

// SSD LCD驱动参数设置
#define SSD_HOR_PULSE_WIDTH 1	// 水平脉宽
#define SSD_HOR_BACK_PORCH 46	// 水平前廊
#define SSD_HOR_FRONT_PORCH 210 // 水平后廊

#define SSD_VER_PULSE_WIDTH 1  // 垂直脉宽
#define SSD_VER_BACK_PORCH 23  // 垂直前廊
#define SSD_VER_FRONT_PORCH 22 // 垂直前廊

// SSD LCD的几个参数
#define SSD_HT (SSD_HOR_RESOLUTION + SSD_HOR_BACK_PORCH + SSD_HOR_FRONT_PORCH) // 水平总周期
#define SSD_HPS (SSD_HOR_BACK_PORCH)										   // 水平同步位置
#define SSD_VT (SSD_VER_RESOLUTION + SSD_VER_BACK_PORCH + SSD_VER_FRONT_PORCH) // 垂直总周期
#define SSD_VPS (SSD_VER_BACK_PORCH)										   // 垂直同步位置

/**
 * @brief LCD初始化函数
 *
 */
void LCD_Init(void);

/**
 * @brief 清屏函数
 *
 * @param color 清屏颜色
 */
void LCD_Clear(uint16_t color);

/**
 * @brief 设置LCD显示方向
 *
 * @param dir 显示方向
 */
void LCD_Display_Dir(uint8_t dir);

/**
 * @brief 设置光标位置
 *
 * @param Xpos X坐标
 * @param Ypos Y坐标
 */
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);

/**
 * @brief 画点函数
 *
 * @param x X坐标
 * @param y Y坐标
 */
void LCD_DrawPoint(uint16_t x, uint16_t y);

/**
 * @brief 填充函数
 *
 * @param sx 起始X坐标
 * @param sy 起始Y坐标
 * @param ex 结束X坐标
 * @param ey 结束Y坐标
 * @param color 填充颜色
 */
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);

/**
 * @brief 颜色填充函数
 *
 * @param sx 起始X坐标
 * @param sy 起始Y坐标
 * @param ex 结束X坐标
 * @param ey 结束Y坐标
 * @param color 颜色数组
 */
void LCD_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);

/**
 * @brief 显示字符
 *
 * @param x 字符起始X坐标
 * @param y 字符起始Y坐标
 * @param num 字符
 * @param size 字体大小
 * @param mode 显示模式
 */
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode);

/**
 * @brief 显示字符串
 *
 * @param x 字符串起始X坐标
 * @param y 字符串起始Y坐标
 * @param width 字符串宽度
 * @param height 字符串高度
 * @param size 字体大小
 * @param p 字符串指针
 */
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t size, uint8_t *p);

/**
 * @brief 显示图片
 *
 * @param x 图片起始X坐标
 * @param y 图片起始Y坐标
 * @param width 图片宽度
 * @param height 图片高度
 * @param p 图片数据指针
 */
void LCD_ShowPhoto(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t *p);

/**
 * @brief 显示中文字符
 *
 * @param x 起始X坐标
 * @param y 起始Y坐标
 * @param size 字符大小
 * @param p 中文字符点阵数据
 */
void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t size, uint8_t *p);

#endif
