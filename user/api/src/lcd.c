#include "lcd.h"
#include "haimian.h"
#include "Icon_wendu.h"
#include "Icon_shidu.h"
#include "Icon_bh1750.h"
#include "Icon_light.h"
#include "Icon_yanwu.h"
#include "Icon_co2.h"
#include "Icon_ch2o.h"
#include "Icon_voc.h"
#include "gif_1.h"
#include "gif_2.h"
#include "gif_3.h"
#include "gif_4.h"

// lcd UI界面初始化
void LCD_UI_Init(void)
{
    LCD_Clear(BLACK); // 清屏

    LCD_SetCursor(0, 0); // 设置光标位置
    LCD_ShowString(0, 0, 128, 16, 16, "Hello World!");
    LCD_ShowPhoto(0, 16, 240, 240, (uint8_t *)gImage_haimian);    // 显示图片
    LCD_ShowPhoto(3, 3, 20, 19, (uint8_t *)gImage_Icon_wendu);    // 温度
    LCD_ShowPhoto(3, 26, 20, 19, (uint8_t *)gImage_Icon_shidu);   // 湿度
    LCD_ShowPhoto(3, 49, 20, 19, (uint8_t *)gImage_Icon_light);   // 光照
    LCD_ShowPhoto(3, 72, 20, 19, (uint8_t *)gImage_Icon_yanwu);   // 烟感
    LCD_ShowPhoto(3, 95, 20, 19, (uint8_t *)gImage_Icon_co2);     // CO2
    LCD_ShowPhoto(3, 118, 20, 19, (uint8_t *)gImage_Icon_ch2o);   // CH2O
    LCD_ShowPhoto(3, 141, 20, 20, (uint8_t *)gImage_Icon_voc);    // VOC
    LCD_ShowPhoto(3, 164, 20, 20, (uint8_t *)gImage_Icon_bh1750); // bh1750
}

// LCD显示数据更新
void LCD_Update_Data(void)
{
    // 显示温度数值在温度图标右侧
    char temp_str[12];
    sprintf(temp_str, "%.2f C", dht.tem); // 与实际数据一致，保留两位小数
    LCD_ShowString(25, 3, 128, 16, 16, (uint8_t *)temp_str);

    // 显示湿度数值在湿度图标右侧
    char hum_str[12];
    sprintf(hum_str, "%.2f %%RH", dht.hum); // 与实际数据一致，保留两位小数
    LCD_ShowString(25, 26, 128, 16, 16, (uint8_t *)hum_str);

    // 显示光照数值在光照图标右侧
    char light_str[12];
    sprintf(light_str, "%d", light_val); // 直接显示ADC原始值
    LCD_ShowString(25, 49, 128, 16, 16, (uint8_t *)light_str);

    // 显示烟感数值在烟感图标右侧
    // char yanwu_str[12];
    // sprintf(yanwu_str, "%d", addbuff[1]);
    // LCD_ShowString(25, 72, 128, 16, 16, (uint8_t *)yanwu_str);

    // 显示CO2数值在CO2图标右侧
    char co2_str[12];
    sprintf(co2_str, "%.2f ppm", kqm.co2); // 与实际数据一致，保留两位小数
    LCD_ShowString(25, 95, 128, 16, 16, (uint8_t *)co2_str);

    // 显示CH2O数值在CH2O图标右侧
    char ch2o_str[12];
    sprintf(ch2o_str, "%.2f ppm", kqm.ch2o); // 与实际数据一致，保留两位小数
    LCD_ShowString(25, 118, 128, 16, 16, (uint8_t *)ch2o_str);

    // 显示VOC数值在VOC图标右侧
    char voc_str[12];
    sprintf(voc_str, "%.2f ppm", kqm.voc); // 与实际数据一致，保留两位小数
    LCD_ShowString(25, 141, 128, 16, 16, (uint8_t *)voc_str);

    // 显示bh1750数值在bh1750图标右侧
    // char bh1750_str[12];
    // sprintf(bh1750_str, "%.2f lx", ); // 与实际数据一致，保留两位小数
    // LCD_ShowString(25, 164, 128, 16, 16, (uint8_t *)bh1750_str);
}

// LCD显示GIF动画
void LCD_Show_GIF(void)
{
    // 显示gif_1
    LCD_ShowPhoto(3, 180, 50, 50, (uint8_t *)gImage_gif_1);
    Delay_nms(100);
    // 显示gif_2
    LCD_ShowPhoto(3, 180, 50, 50, (uint8_t *)gImage_gif_2);
    Delay_nms(100);
    // 显示gif_3
    LCD_ShowPhoto(3, 180, 50, 50, (uint8_t *)gImage_gif_3);
    Delay_nms(100);
    // 显示gif_4
    LCD_ShowPhoto(3, 180, 50, 50, (uint8_t *)gImage_gif_4);
    Delay_nms(100);
}
