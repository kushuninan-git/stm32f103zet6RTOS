#include "usart1.h"

/* ----------------- 全局变量 ----------------- */
USART1_Data_t usart1_data = {0};

/**
 * @brief  USART1初始化（PA9=TX, PA10=RX，115200 8N1）
 */
void USART1_Init(void)
{
    // 1. 时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    // 2. GPIO配置
    GPIO_InitTypeDef GPIO_InitStructure;
    // PA9(TX) - 复用推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    // PA10(RX) - 浮空输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3. USART参数配置
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    // 4. NVIC中断配置
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 5. 使能中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

    // 6. 使能USART1
    USART_Cmd(USART1, ENABLE);

    // 7. 清除初始状态
    (void)USART_ReceiveData(USART1);
}

/**
 * @brief  发送1个字节
 */
void USART1_SendByte(uint8_t data)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ;
    USART_SendData(USART1, data);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        ;
}

/**
 * @brief  发送字符串
 */
void USART1_SendString(const char *str)
{
    while (str && *str)
    {
        USART1_SendByte((uint8_t)*str++);
    }
}

/**
 * @brief  发送数组
 */
void USART1_SendArray(uint8_t *arr, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        USART1_SendByte(arr[i]);
    }
}

/**
 * @brief  清空接收缓冲区
 */
void USART1_ClearRxBuffer(void)
{
    usart1_data.rx_len = 0;
    usart1_data.rx_over = 0;
}

/**
 * @brief  printf重定向（串口1输出）
 */
int fputc(int ch, FILE *f)
{
    USART1_SendByte((uint8_t)ch);
    return ch;
}

/**
 * @brief  USART1中断服务函数
 */
void USART1_IRQHandler(void)
{
    uint8_t clear_helper;

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uint8_t res = USART_ReceiveData(USART1);
        if (usart1_data.rx_over == 0 && usart1_data.rx_len < USART1_RX_BUF_SIZE)
        {
            usart1_data.rx_buf[usart1_data.rx_len++] = res;
        }
    }

    if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        clear_helper = USART1->SR;
        clear_helper = USART1->DR;
        (void)clear_helper;

        usart1_data.rx_over = 1;
    }

    if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
    {
        clear_helper = USART1->SR;
        clear_helper = USART1->DR;
        (void)clear_helper;
    }
}
