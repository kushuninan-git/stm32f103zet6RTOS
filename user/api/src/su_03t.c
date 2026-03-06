#include "su_03t.h"
#include "stdio.h"
#include "string.h"
#include "usart1.h"
#include "led.h"
#include "jx_uart_send.h"
#include "kqm.h"
#include "dht11.h"

SU03T su03t = {0};

// 语音模块   PC12/UART5_TX   PD2/UART5_RX
void Su03t_Init(void)
{
    // 时钟配置：GPIOC, GPIOD 和 UART5
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

    // PC12 (TX) 复用推挽输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // PD2 (RX) 浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    // 输入模式无需配置Speed
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    // UART5配置
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART5, &USART_InitStructure);

    // 中断配置
    USART_ITConfig(UART5, USART_IT_RXNE, ENABLE); // 接收非空中断
    USART_ITConfig(UART5, USART_IT_IDLE, ENABLE); // 空闲中断

    // NVIC配置
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = UART5_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    // 使能UART5
    USART_Cmd(UART5, ENABLE);
}

// 发送单个字节
void Su03t_SendByte(uint8_t txdata)
{
    /* 将数据放入UART5发送数据寄存器*/
    USART_SendData(UART5, txdata);

    /* 等待发送数据寄存器为空 */
    while (USART_GetFlagStatus(UART5, USART_FLAG_TXE) == RESET)
    {
    }
}

// 发送字节数组
void Su03t_SendArray(uint8_t *Array, uint16_t Length)
{
    // 入参校验
    if (Array == NULL || Length == 0)
        return;

    for (uint16_t i = 0; i < Length; i++)
    {
        Su03t_SendByte(Array[i]); // 依次发送每个字节
    }
}

// 发送字符串
void Su03t_SendString(uint8_t *str)
{
    if (str == NULL)
        return;

    while (*str != '\0')
    {
        Su03t_SendByte(*str);
        str++;
    }
}

// UART5中断服务函数
void UART5_IRQHandler(void)
{
    uint8_t data = 0;
    // 接收非空中断
    if (USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
    {
        // 缓冲区长度限制为3（匹配rxbuff[3]），避免越界
        if (su03t.rxnum < sizeof(su03t.rxbuff))
        {
            su03t.rxbuff[su03t.rxnum++] = USART_ReceiveData(UART5);
        }
        else
        {
            // 防止溢出，读取数据丢弃
            USART_ReceiveData(UART5);
        }
        USART_ClearITPendingBit(UART5, USART_IT_RXNE);
    }

    // 空闲中断（一帧数据接收完成）
    if (USART_GetITStatus(UART5, USART_IT_IDLE) == SET)
    {
        su03t.over = 1; // 标记接收完成
        // 必须先读SR再读DR清除空闲中断标志
        data = UART5->SR;
        data = UART5->DR;
        (void)data; // 防止未使用变量警告
    }
}

// SU03T数据解析函数
SU03T_Status_t Su03t_ReadData(void)
{
    SU03T_Status_t status = SU03T_NO_DATA;

    // 仅当接收完成时处理
    if (su03t.over == 1)
    {
        // 检查数据长度是否为3（帧头+数据+校验和）
        if (su03t.rxnum == 3)
        {
            // 校验帧头（SU03T通用帧头0xAA）
            if (su03t.rxbuff[0] == 0xAA)
            {
                uint8_t check_sum = 0;
                // 累加前2个字节计算校验和（帧头+数据）
                for (uint8_t i = 0; i < 2; i++)
                {
                    check_sum += su03t.rxbuff[i];
                }

                // 校验和匹配才处理数据
                if (check_sum == su03t.rxbuff[2])
                {
                    // 解析有效数据
                    su03t.data = su03t.rxbuff[1];
                    status = SU03T_OK;
                }
                else
                {
                    status = SU03T_ERR_CHECKSUM;
                }
            }
            else
            {
                status = SU03T_ERR_HEADER;
            }
        }
        else
        {
            status = SU03T_ERR_LEN;
        }

        // 重置接收状态，准备下一次接收
        su03t.over = 0;
        su03t.rxnum = 0;
        memset(su03t.rxbuff, 0, sizeof(su03t.rxbuff));
    }

    return status;
}

// SU03T调试打印函数
void Su03t_Debug(void)
{
    printf("SU03T有效数据: %d (0x%02X)\r\n", su03t.data, su03t.data);
}

// SU03T语音命令处理函数
void Su03t_Handle_Command(void)
{
    // 如果没有有效数据（data为0），直接返回
    if (su03t.data == 0)
    {
        return;
    }

    switch (su03t.data)
    {
    case 0x01: // 语音模块返回0x01时，打开LED1
        LED1_ON();
        break;

    case 0x10: // 语音模块返回0x10时，关闭LED1
        LED1_OFF();
        break;

    case 0x06: // 语音模块返回0x06时，开始传输二氧化碳浓度
        _uart_co2_2(kqm.co2);
        break;

    case 0x07: // 语音模块返回0x07时，开始传输甲醛浓度
        _uart_ch2o_2(kqm.ch2o);
        break;

    case 0x08: // 语音模块返回0x08时，开始传输VOC浓度
        _uart_voc_2(kqm.voc);
        break;

    case 0x09: // 语音模块返回0x09时，开始传输湿度
        _uart_hum_2(dht.hum);
        break;

    case 0x0A: // 语音模块返回0x0A时，开始传输温度
        _uart_tem_2(dht.tem);
        break;

    default:
        break;
    }

    // 处理完成后清除数据，防止重复执行
    su03t.data = 0;
}
