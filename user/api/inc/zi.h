#ifndef __ZI_H
#define __ZI_H
#include <stdint.h>

// 字符索引枚举
typedef enum {
    FONT_ZI_NI = 0,    // "你"
    FONT_ZI_HAO,       // "好"
    FONT_ZI_SHI,       // "世"
    FONT_ZI_JIE,       // "界"
    FONT_ZI_ZHONG,     // "中"
    FONT_ZI_GUO,       // "国"
    FONT_ZI_TIAN,      // "天"
    FONT_ZI_DI,        // "地"
    FONT_ZI_REN,       // "人"
    FONT_ZI_NIAN       // "年"
    // 可在此添加更多字符索引
} FontIndex;

// 函数声明
void Font_StoreToFlash(void);           // 将字库存储到Flash
void Font_ReadChar(uint8_t index, uint8_t* data);  // 从Flash读取指定字符
void Font_DisplayChar(uint16_t x, uint16_t y, uint8_t index);  // 显示指定字符
uint8_t Font_GetCharCount(void);        // 获取字符数量

#endif
