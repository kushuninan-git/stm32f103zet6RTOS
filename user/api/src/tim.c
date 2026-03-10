#include "tim.h"
#include "lvgl.h"
/************************* 全局1ms计时变量 *************************/
// 定时器每1ms中断一次，该变量+1，全局所有模块可调用，精准无误差
volatile uint32_t sys_ms = 0;
// FreeRTOS运行时间统计变量
volatile uint32_t CPU_RunTime = 0;

/************************* TIM6初始化函数 - 产生精准1ms中断 *************************/
// 核心参数：72MHz主频 - 预分频71 - 自动重装999 = 精准1ms
void TIM_Init(void)
{
    // 结构体
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 开启定时器6时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    // 配置定时器核心参数
    TIM_TimeBaseStructure.TIM_Period = TIM6_PERIOD;             // 自动重装值 999
    TIM_TimeBaseStructure.TIM_Prescaler = TIM6_PRESCALER;       // 预分频值 71
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                // 时钟不分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    // 配置TIM6中断优先级 - 定时器自己配置自身NVIC，符合规范
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; // 抢占优先级3 低于按键中断
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // 子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能定时器更新中断
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);

    // 使能定时器6
    TIM_Cmd(TIM6, ENABLE);
}

/************************* TIM6中断服务函数 - 纯计时，无任何业务逻辑 *************************/
// 定时器6中断服务函数
void TIM6_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
			lv_tick_inc(1);
        sys_ms++;                                   // 维持原有的1ms时基，保证按键等业务逻辑不变
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update); // 必须清除中断标志位
    }
}
