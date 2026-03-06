#ifndef _LCD_H_
#define _LCD_H_

#include "stm32f10x.h"

#include "bsp_lcd.h"
#include "dht11.h"
#include "light.h"
#include "kqm.h"
#include "Myi2c_bh1750.h"
#include "stdio.h"
#include "delay.h"

void LCD_UI_Init(void);
void LCD_Update_Data(void);
void LCD_Show_GIF(void);

#endif
