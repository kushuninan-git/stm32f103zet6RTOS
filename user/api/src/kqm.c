#include "kqm.h"
#include "stdio.h"
#include "string.h"

KQM kqm = {0};

// USART2初始化 PA2 TX PA3 RX
void Kqm_Init(void)
{
    // 时钟 GPIOA和USART2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // PA2(TX) 复用推挽输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // PA3(RX) 浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART2配置
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    // 中断配置
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 接收非空中断
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE); // 空闲中断

    // NVIC配置
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1; // 与USART1区分子优先级
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    // 使能USART2
    USART_Cmd(USART2, ENABLE);
}
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
// 全局信号量，用于任务间通信
// extern SemaphoreHandle_t xSemaphore1;
extern SemaphoreHandle_t xSemaphore1;
void USART2_IRQHandler(void)
{
    uint8_t data = 0;
    // 接收非空中断
    if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
    {
        // 限制缓冲区长度，避免溢出（传感器数据固定8字节）
        if (kqm.rxnum < 8)
        {
            kqm.rxbuff[kqm.rxnum++] = USART_ReceiveData(USART2);
        }
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }

    // 空闲中断（一帧数据接收完成）
    if (USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
    {
        kqm.over = 1; // 标记接收完成
                      // // 通知任务解析数据
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(xSemaphore1, &xHigherPriorityTaskWoken);
        // // 任务切换，确保高优先级任务立即运行
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        // 必须先读SR再读DR清除空闲中断标志
        data = USART2->SR;
        data = USART2->DR;
        (void)data;
    }
}

// 传感器数据读取函数
void Kqm_ReadData(void)
{
    // 仅当接收完成且数据长度足够时解析
    if (kqm.over == 1 && kqm.rxnum >= 8)
    {
        // 校验帧头
        if (kqm.rxbuff[0] == 0x5F)
        {
            uint8_t check_sum = 0;
            // 累加前7个字节计算校验和
            for (uint8_t i = 0; i < 7; i++)
            {
                check_sum += kqm.rxbuff[i];
            }

            // 校验和匹配才处理数据
            if (check_sum == kqm.rxbuff[7])
            {
                // 解析数据（注意字节序：高字节在前）
                kqm.voc = (kqm.rxbuff[1] << 8 | kqm.rxbuff[2]) * 0.1;
                kqm.ch2o = (kqm.rxbuff[3] * 256 + kqm.rxbuff[4]) * 0.01;
                kqm.co2 = (kqm.rxbuff[5] << 8 | kqm.rxbuff[6]) * 1;
            }
            else
            {
                printf("kqm 校验和错误\r\n");
            }
        }
        else
        {
            printf("kqm 帧头错误\r\n");
        }
    }
    else if (kqm.over == 1)
    {
        printf("kqm 数据长度不足\r\n");
    }

    // 重置接收状态
    kqm.over = 0;
    kqm.rxnum = 0;
    memset(kqm.rxbuff, 0, sizeof(kqm.rxbuff));
}

// 传感器调试打印函数
void Kqm_Debug(void)
{
    printf("voc: %.1f\t ch2o: %.2f\t co2: %.0f\r\n", kqm.voc, kqm.ch2o, kqm.co2);
}
