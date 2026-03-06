#include "relay.h"

// Led初始化
void RELAY_Init(void)
{
    // 时钟 GPIOA
    RCC->APB2ENR |= (0x1 << 2);

    GPIOA->CRH &= ~((0x1 << 15));
    GPIOA->CRH &= ~((0x1 << 14));
    GPIOA->CRH |= ((0x1 << 13));
    GPIOA->CRH |= ((0x1 << 12));
    // 外设
    // 中断
    // 其他
}

// 应用函数
// state:1 亮  0:灭 PA11
void RELAY(uint8_t state)
{
    if (state == 1)
        GPIOA->ODR &= ~(0x1 << 11);
    else if (state == 0)
        GPIOA->ODR |= (0x1 << 11);
}
