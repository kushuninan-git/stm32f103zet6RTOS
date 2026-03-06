#include "pwm.h"
#include "stm32f10x.h"

// TIM3-CH3 PB0 复用
void Pwm_Init()
{
    // 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    // 结构体
    GPIO_InitTypeDef GPIO_InitStructure;

    // IO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 外设 tim3
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // 重装载值 计数周期为1ms
    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
    // 分频值 72m/72
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
    // 分频因子 通常用1
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* 通道输出初始化 */

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;             // pwm模式
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     // 有效电平
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // 输出使能
    TIM_OCInitStructure.TIM_Pulse = 500;                          // 比较值

    // 通道3 0C3
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    // 启动重装载值影子寄存器
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
    // 中断

    //  其他

    // 使能定时器
    TIM_Cmd(TIM3, ENABLE);
}
