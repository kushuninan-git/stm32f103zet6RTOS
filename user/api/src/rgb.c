#include "rgb.h"

// Led初始化
void RGB_Init(void)
{
    // 时钟APB2 - 开启GPIOA的时钟（三个RGB引脚都在GPIOA，只需要开一次！）
    RCC_APB2PeriphClockCmd(RGB_B_GPIO_CLK, ENABLE);

    // IO口初始化结构体
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    // 配置：PA6(蓝)、PA7(绿)、PA8(红) 都是推挽输出，10MHz
    // 正确写法：同端口的多个引脚，用 | 拼接，一次初始化完成！
    GPIO_InitStructure.GPIO_Pin = RGB_B_GPIO_PIN | RGB_G_GPIO_PIN | RGB_R_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(RGB_B_GPIO_PORT, &GPIO_InitStructure); // 只需要初始化一次GPIOA即可

    // ✅ 核心修正：初始化完成后，全部置【高电平】→ RGB灯全部熄灭
    // GPIO_SetBits = 置高电平 = 灯灭  |  GPIO_ResetBits = 置低电平 = 灯亮
    GPIO_SetBits(RGB_B_GPIO_PORT, RGB_B_GPIO_PIN);
    GPIO_SetBits(RGB_G_GPIO_PORT, RGB_G_GPIO_PIN);
    GPIO_SetBits(RGB_R_GPIO_PORT, RGB_R_GPIO_PIN);
}

// 应用函数     蓝灯 PA6
// state:1 亮  0:灭 【注释和实际功能一致】
void RGB_B(uint8_t state)
{
    if (state == 1)
        GPIO_ResetBits(RGB_B_GPIO_PORT, RGB_B_GPIO_PIN); // 亮 → 置低电平
    else if (state == 0)
        GPIO_SetBits(RGB_B_GPIO_PORT, RGB_B_GPIO_PIN); // 灭 → 置高电平
}
// 应用函数     绿灯 PA7
// state:1 亮  0:灭
void RGB_G(uint8_t state)
{
    if (state == 1)
        GPIO_ResetBits(RGB_G_GPIO_PORT, RGB_G_GPIO_PIN); // 亮 → 置低电平
    else if (state == 0)
        GPIO_SetBits(RGB_G_GPIO_PORT, RGB_G_GPIO_PIN); // 灭 → 置高电平
}
// 应用函数     红灯 PA8
// state:1 亮  0:灭
void RGB_R(uint8_t state)
{
    if (state == 1)
        GPIO_ResetBits(RGB_R_GPIO_PORT, RGB_R_GPIO_PIN); // 亮 → 置低电平
    else if (state == 0)
        GPIO_SetBits(RGB_R_GPIO_PORT, RGB_R_GPIO_PIN); // 灭 → 置高电平
}
