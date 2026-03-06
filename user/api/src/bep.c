#include "bep.h"
/**
 * @brief 初始化蜂鸣器 PC0
 *
 */
void Bep_Init(void)
{
    // 时钟APB2
    RCC_APB2PeriphClockCmd(BEEP_GPIO_CLK, ENABLE);

    // IO  0011
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.GPIO_Pin = BEEP_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(BEEP_GPIO_PORT, &GPIO_InitStructure);
}

void Bep(uint8_t state)
{
    if (state == 1)
    {
        GPIO_SetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);
    }
    if (state == 0)
    {
        GPIO_ResetBits(BEEP_GPIO_PORT, BEEP_GPIO_PIN);
    }
}
