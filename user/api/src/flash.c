#include "flash.h"

/**
 * @brief W25Q64初始化函数
 * @details 初始化SPI2和W25Q64引脚
 * @param 无
 * @return 无
 */
void Flash_Init(void)
{
    /* 1. 使能相关时钟 */
    // 使能GPIOB时钟，用于控制SPI相关引脚
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // 使能AFIO时钟，用于SPI复用功能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // 使能SPI2时钟，用于SPI通信功能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = {0};

    /* 2. 配置SPI通信引脚 */
    // 配置PB13(SPI2_SCK)和PB15(SPI2_MOSI)为复用推挽输出
    // 复用推挽输出模式：用于SPI通信的时钟和数据输出
    // 50MHz速度：满足SPI高速通信需求
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 配置PB14(SPI2_MISO)为浮空输入
    // 浮空输入模式：用于接收从设备返回的数据，不使用内部上拉/下拉电阻
    // 避免影响从设备发送的数据电平
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 配置PB12(W25Q64_CS)为推挽输出
    // 推挽输出模式：用于控制W25Q64芯片的片选信号
    // 可以提供足够的驱动能力

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 高电平：W25Q64芯片未被选中，不参与通信
    GPIO_SetBits(GPIOB, GPIO_Pin_12);

    /* 3. 配置SPI通信参数 */
    SPI_InitTypeDef SPI_InitStructure = {0};

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // 配置为双线全双工模式，同时支持数据的发送和接收
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                      // 配置为主模式，STM32作为主设备，控制通信时钟和数据传输
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  // 配置数据宽度为8位
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                         // 配置时钟极性为低电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                       // 配置时钟相位为第一个边沿采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                          // 配置为软件NSS管理，通过软件控制片选信号
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8; // 配置波特率预分频为8
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                 // 数据高位先传
    SPI_InitStructure.SPI_CRCPolynomial = 7;                           // 配置CRC多项式为7，用于SPI通信的CRC校验

    SPI_Init(SPI2, &SPI_InitStructure); // 初始化SPI2
    SPI_Cmd(SPI2, ENABLE);              // 使能SPI2
}

/**
 * @brief 发送并接收一个字节数据
 * @details 等待发送缓冲区为空，确保之前的数据已经发送完成；
 * 发送数据到SPI2的数据寄存器，数据会被自动从发送缓冲区传输到SPI2的MOSI引脚；
 * 等待接收缓冲区非空，确保从设备已经发送了数据到MISO引脚；
 * 从接收缓冲区读取数据并返回。
 * @param data 要发送的数据
 * @return u8 接收到的数据
 */
u8 SPI_SendAndRecive(u8 data)
{
    // 等待发送缓冲区为空
    // 确保之前的数据已经发送完成
    // 等待发送缓冲区为空，确保之前的数据已经发送完成
    // 这是为了避免数据丢失或冲突，确保数据按照正确的顺序发送
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
        ;

    // 发送数据
    // 发送数据到SPI2的数据寄存器
    // 数据会被自动从发送缓冲区传输到SPI2的MOSI引脚
    // 这是因为SPI2的数据寄存器是一个双缓冲区结构，
    // 当发送缓冲区为空时，新的数据会被自动加载到发送缓冲区，
    // 而当发送缓冲区非空时，数据会被自动从发送缓冲区传输到MOSI引脚
    SPI_I2S_SendData(SPI2, data);

    // 等待接收缓冲区非空
    // 确保从设备已经发送了数据到MISO引脚
    // 等待接收缓冲区非空，确保从设备已经发送了数据到MISO引脚
    // 这是为了避免数据丢失或冲突，确保数据按照正确的顺序接收
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
        ;

    // 返回接收到的数据
    // 从接收缓冲区读取数据并返回
    // 这是因为SPI2的数据寄存器是一个双缓冲区结构，
    // 当接收缓冲区非空时，数据会被自动从MISO引脚传输到接收缓冲区，
    // 而当接收缓冲区为空时，新的数据会被自动加载到接收缓冲区
    return SPI_I2S_ReceiveData(SPI2);
}
/**
 * @brief 从W25Q64芯片读取数据
 * @details 拉低片选信号，选中W25Q64芯片；
 * 发送03H；
 * 发送24位地址；
 * 接收数据，存储到buff缓冲区；
 * 拉高片选信号，取消选中W25Q64芯片。
 * @param buff 存储读取数据的缓冲区
 * @param len 要读取的字节数
 * @param addr 要读取的起始地址
 */
void Flash_ReadData(u8 buff[], u8 len, u32 addr)
{
    // 拉低片选信号，选中W25Q64芯片
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    // 发送03H
    SPI_SendAndRecive(0x03);
    // 发送24位地址
    SPI_SendAndRecive((addr & 0xFF0000) >> 16);
    SPI_SendAndRecive((addr & 0xFF00) >> 8);
    SPI_SendAndRecive((addr & 0xFF) >> 0);
    // 接收数据
    for (u8 i = 0; i < len; i++)
    {
        buff[i] = SPI_SendAndRecive(0xFF);
    }
    // 拉高片选信号，取消选中W25Q64芯片
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
}
/**
 * @brief 擦除W25Q64芯片扇区擦除
 * @details 使能写操作；
 * 拉低片选信号，选中W25Q64芯片；
 * 发送20H；
 * 发送24位地址；
 * 拉高片选信号，取消选中W25Q64芯片；
 * 等待擦除完成。
 * @param addr 要擦除的起始地址
 */
void Flash_Delete(u32 addr)
{
    // 使能写操作
    Flash_WriteEnable();
    // 拉低片选信号，选中W25Q64芯片
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    // 发送20H
    SPI_SendAndRecive(0x20);
    // 发送24位地址
    SPI_SendAndRecive((addr & 0xFF0000) >> 16);
    SPI_SendAndRecive((addr & 0xFF00) >> 8);
    SPI_SendAndRecive((addr & 0xFF) >> 0);
    // 拉高片选信号，取消选中W25Q64芯片
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
    // 等待擦除完成
    //    while (1)
    //    {
    //        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    //        SPI_SendAndRecive(0x05);
    //        if (!(SPI_SendAndRecive(0xFF) & 0x01))
    //        {
    //            break;
    //        }
    //        GPIO_SetBits(GPIOB, GPIO_Pin_12);
    //    }
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
}
/**
 * @brief 使能W25Q64写操作
 * @details 拉低片选信号，选中W25Q64芯片；
 * 发送06H；
 * 拉高片选信号，取消选中W25Q64芯片。
 * @param 无
 * @return 无
 */
void Flash_WriteEnable(void)
{
    // 拉低片选信号，选中W25Q64芯片
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    // 发送06H
    SPI_SendAndRecive(0x06);
    // 拉高片选信号，取消选中W25Q64芯片
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
}
// 写数据到W25Q64芯片
/**
 * @brief 写数据到W25Q64芯片
 * @details 使能写操作；
 * 拉低片选信号，选中W25Q64芯片；
 * 发送02H；
 * 发送24位地址；
 * 发送数据，从buff缓冲区读取数据；
 * 拉高片选信号，取消选中W25Q64芯片；
 * 等待写入完成。
 * @param buff 存储要写入数据的缓冲区
 * @param len 要写入的字节数
 * @param addr 要写入的起始地址
 */
void Flash_WriteData(u8 buff[], u8 len, u32 addr)
{
    // 使能写操作
    Flash_WriteEnable();
    // 拉低片选信号，选中W25Q64芯片
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    // 发送02H
    SPI_SendAndRecive(0x02);
    // 发送24位地址
    SPI_SendAndRecive((addr & 0xFF0000) >> 16);
    SPI_SendAndRecive((addr & 0xFF00) >> 8);
    SPI_SendAndRecive((addr & 0xFF) >> 0);
    // 发送数据
    for (u8 i = 0; i < len; i++)
    {
        SPI_SendAndRecive(buff[i]);
    }
    // 拉高片选信号，取消选中W25Q64芯片
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
    // 等待写入完成
    while (1)
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
        SPI_SendAndRecive(0x05);
        if (!(SPI_SendAndRecive(0xFF) & 0x01))
        {
            break;
        }
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
    }
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
}

// 老师代码
// uint8_t SPI_SendAndRecive(uint8_t data)
//{
//     while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
//         ;
//     SPI_I2S_SendData(SPI2, data);
//     while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
//         ;
//     return SPI_I2S_ReceiveData(SPI2);
// }

// void Flash_ReadData(uint8_t buff[], uint8_t len, uint32_t addr)
// {
//     // 1.csÀ­µÍ
//     GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
//     // 2.·¢ËÍ03H
//     SPI_SendAndRecive(0x03);
//     // 3.·¢ËÍ24bitµØÖ·
//     SPI_SendAndRecive((addr & 0xFF0000) >> 16);
//     SPI_SendAndRecive((addr & 0xFF00) >> 8);
//     SPI_SendAndRecive((addr & 0xFF) >> 0);
//     // 4.±£´æ½ÓÊÕµÄÊý¾Ý
//     for (uint8_t i = 0; i < len; i++)
//         buff[i] = SPI_SendAndRecive(0xFF);
//     // 5.Æ¬Ñ¡À­¸ß
//     GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
// }

// void Flash_Delete(uint32_t addr)
// {
//     // 1.csÀ­µÍ
//     GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
//     // 2.·¢ËÍ03H
//     SPI_SendAndRecive(0x20);
//     // 3.·¢ËÍ24bitµØÖ·
//     SPI_SendAndRecive((addr & 0xFF0000) >> 16);
//     SPI_SendAndRecive((addr & 0xFF00) >> 8);
//     SPI_SendAndRecive((addr & 0xFF) >> 0);
//     // 5.Æ¬Ñ¡À­¸ß
//     GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
// }
// void Flash_Write_Enable(void)
// {
//    // 1.csÀ­µÍ
//    GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_RESET);
//    // 2.·¢ËÍ03H
//    SPI_SendAndRecive(0x06);
//    // 5.Æ¬Ñ¡À­¸ß
//    GPIO_WriteBit(GPIOB, GPIO_Pin_12, Bit_SET);
// }
