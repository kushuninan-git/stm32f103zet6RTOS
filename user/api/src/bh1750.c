#include "bh1750.h"
#include "stdio.h" // 引入stdio.h以支持printf函数

/********************* 私有辅助函数：I2C事件等待（硬件I2C核心函数） *********************/
// 功能：循环检测指定I2C事件是否触发，超时则返回错误，防止程序死锁
// 参数：I2Cx-I2C外设，I2C_EVENT-待检测事件，timeout-超时阈值
// 返回值：SUCCESS-事件触发，ERROR-超时
// 核心讲解：STM32硬件I2C由「专用外设自动完成时序」，软件无需控制SCL/SDA高低，
// 只需通过「事件」判断上一步操作是否完成——事件是硬件I2C的“执行信号”，
// 只有检测到对应事件，才能证明硬件时序完成，下一步操作才不会导致通信错乱
static ErrorStatus I2C_WaitEvent(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT, uint32_t timeout)
{
    uint32_t t = 0; // 超时计数器
    // 循环检测：I2C_CheckEvent返回TRUE表示事件触发，退出循环
    // I2C_CheckEvent：标准库封装的寄存器检测函数，自动读取I2C_SR1/SR2寄存器，
    // 判断标志位组合是否匹配指定事件，无需软件手动操作寄存器，简化开发
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT))
    {
        t++;             // 计数器自增
        if (t > timeout) // 超过阈值，判定硬件故障，返回错误
        {
            return ERROR;
        }
    }
    return SUCCESS; // 事件正常触发，返回成功
}

/********************* 硬件I2C1初始化函数（BH1750_I2C_Init） *********************/
// 核心功能：配置I2C通信所需的硬件环境，包括「GPIO复用模式」和「I2C外设参数」，
// 遵循STM32外设使用原则：先使能时钟→再配置外设→最后使能外设
void BH1750_I2C1_Init(void)
{
    // 定义GPIO和I2C初始化结构体，标准库规范：通过结构体配置外设参数，简洁易维护
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef I2C_InitStruct;

    // 步骤1：使能时钟（STM32所有外设使用前，必须先使能对应总线时钟，核心原则！）
    RCC_APB1PeriphClockCmd(BH1750_I2C_CLK, ENABLE);      // 使能I2C1时钟（APB1总线）
    RCC_APB2PeriphClockCmd(BH1750_I2C_GPIO_CLK, ENABLE); // 使能GPIOB时钟（APB2总线）

    // 步骤2：配置SCL/SDA引脚为「复用开漏输出」（I2C总线强制要求，核心配置！）
    // 配置原因：
    // 1. 复用模式（GPIO_Mode_AF_OD）：让I2C1外设而非CPU内核控制引脚，实现硬件自动生成I2C时序；
    // 2. 开漏输出：I2C总线是「开漏总线」，多个设备共用SCL/SDA，开漏输出才能实现“线与逻辑”（一个设备拉低，总线即低）；
    // 3. 需外接4.7K上拉电阻：开漏输出本身无法输出高电平，上拉电阻让SCL/SDA空闲时为高，符合I2C协议要求
    GPIO_InitStruct.GPIO_Pin = BH1750_SCL_PIN | BH1750_SDA_PIN; // 同时配置SCL/SDA引脚
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;                // 复用开漏输出（I2C必选模式）
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;              // 引脚速率50MHz，适配硬件I2C时序
    GPIO_Init(BH1750_GPIO_PORT, &GPIO_InitStruct);              // 应用配置到GPIOB口

    // 步骤3：配置I2C1核心参数（主模式、100KHz、7位地址，适配BH1750）
    I2C_DeInit(BH1750_I2C);                                                // 先复位I2C1，清除默认配置，避免残留参数影响通信
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;                                // 选择纯I2C模式，而非SMBus模式，适配BH1750等标准I2C设备
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;                        // 时钟占空比2:1，配合100KHz为标准I2C时序
    I2C_InitStruct.I2C_OwnAddress1 = 0x00;                                 // 主机无自身7位地址（I2C主机无需被从机寻址，可随意设0）
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;                               // 使能应答（核心！I2C读操作时，主机需要发送ACK/NACK告知从机是否继续发送）
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 7位地址模式，BH1750使用7位器件地址
    I2C_InitStruct.I2C_ClockSpeed = 100000;                                // I2C时钟频率100KHz（标准模式），BH1750推荐速率，兼容性好
    I2C_Init(BH1750_I2C, &I2C_InitStruct);                                 // 应用配置到I2C1外设

    // 步骤4：使能I2C1外设，配置完成，硬件开始工作，可生成I2C事件和时序
    I2C_Cmd(BH1750_I2C, ENABLE);
}

/********************* BH1750传感器初始化（I2C纯写操作：配置工作模式） *********************/
// 核心功能：通过I2C「主发从收」协议，向BH1750发送工作模式指令，让传感器进入连续高分辨率工作状态
// 通信链路：起始信号→发写地址→发模式指令→停止信号，对应I2C事件：EV5→EV6（发送模式）→EV8_2
ErrorStatus BH1750_Init(void)
{
    // 先初始化硬件I2C1，为后续I2C通信做准备
    BH1750_I2C1_Init();

    // 步骤1：主机发送I2C起始信号，等待EV5事件（I2C_EVENT_MASTER_MODE_SELECT）
    I2C_GenerateSTART(BH1750_I2C, ENABLE); // 硬件自动生成起始信号：SCL高电平时，SDA由高拉低（I2C协议规定的通信开始标志）
    // 等待EV5：检测I2C_SR1寄存器SB位=1（起始位已发送），证明硬件已完成起始信号时序
    // 为什么要等？只有起始信号发送成功，总线上的BH1750才会检测到通信开始，后续发地址才有效
    if (I2C_WaitEvent(BH1750_I2C, I2C_EVENT_MASTER_MODE_SELECT, BH1750_I2C_TIMEOUT) == ERROR)
    {
        I2C_GenerateSTOP(BH1750_I2C, ENABLE);
        printf("Init EV5 failed!\r\n");
        return ERROR;
    }

    // 步骤2：主机发送BH1750写地址，等待EV6事件（发送模式：I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED）
    // I2C_Send7bitAddress：发送7位器件地址，硬件自动拼接「写位0」为8位地址（0x46）
    // 第三个参数：I2C_Direction_Transmitter=0，表示主机为发送方，BH1750为接收方（主发从收）
    I2C_Send7bitAddress(BH1750_I2C, BH1750_ADDR, I2C_Direction_Transmitter);
    // 等待EV6（发送模式）：检测SR1.ADDR=1+SR2.MSL=1+SR1.TXE=1，证明：
    // 1. BH1750检测到地址匹配，在第9个SCL时钟发送了ACK（应答）；
    // 2. 主机进入发送模式，发送寄存器为空，可继续发送数据/指令
    if (I2C_WaitEvent(BH1750_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED, BH1750_I2C_TIMEOUT) == ERROR)
    {
        I2C_GenerateSTOP(BH1750_I2C, ENABLE);
        printf("Init EV6 failed!\r\n");
        return ERROR;
    }

    // 步骤3：主机发送BH1750工作模式指令，等待EV8_2事件（I2C_EVENT_MASTER_BYTE_TRANSMITTED）
    I2C_SendData(BH1750_I2C, BH1750_MODE_H); // 将模式指令0x10写入I2C数据寄存器DR，硬件自动通过SDA发送给BH1750
    // 等待EV8_2：检测SR1.BTF=1（字节传输完成），证明：
    // 1. 指令已完全发送到BH1750；
    // 2. BH1750接收完8位指令后，再次发送了ACK；
    // 3. 主机发送寄存器为空，字节传输完成
    if (I2C_WaitEvent(BH1750_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED, BH1750_I2C_TIMEOUT) == ERROR)
    {
        I2C_GenerateSTOP(BH1750_I2C, ENABLE);
        printf("Init EV8_2 failed!\r\n");
        return ERROR;
    }

    // 步骤4：主机发送I2C停止信号，结束写操作，延时等待BH1750测量完成
    I2C_GenerateSTOP(BH1750_I2C, ENABLE); // 硬件自动生成停止信号：SCL高电平时，SDA由低拉高（I2C协议规定的通信结束标志）
    Delay_nms(150);                       // 延时150ms（大于120ms），原因：BH1750接收到模式指令后，需要120ms完成内部采样和AD转换，
                                          // 只有转换完成，内部寄存器才会存入有效光照数据，否则后续读取为无效值

    return SUCCESS;
}

/********************* 读取BH1750光照强度（I2C纯读操作：获取16位测量数据） *********************/
float bh1750_hw_lux = 0.0f;

// 核心功能：通过I2C「主收从发」协议，从BH1750读取16位大端格式数据，转换为实际光照强度（lx）
// 通信链路：起始信号→发读地址→读高字节（发ACK）→读低字节（发NACK）→停止信号，对应I2C事件：EV5→EV6（接收模式）→EV7×2
// 核心区别：读操作的ACK/NACK由「主机主动控制」，写操作的ACK由「从机自动发送」
void BH1750_HW_ReadData(void)
{
    uint8_t buf[2];    // 存储读取的16位数据：buf[0]=高字节(MSB)，buf[1]=低字节(LSB)
    uint16_t raw_data; // 合并后的16位原始数据

    // 步骤1：主机重新发送I2C起始信号，等待EV5事件
    // 为什么重新发？BH1750的读操作是独立的通信阶段，需要重新发送起始信号告知从机“即将进行读操作”
    I2C_GenerateSTART(BH1750_I2C, ENABLE);
    if (I2C_WaitEvent(BH1750_I2C, I2C_EVENT_MASTER_MODE_SELECT, BH1750_I2C_TIMEOUT) == ERROR)
    {
        I2C_GenerateSTOP(BH1750_I2C, ENABLE);
        I2C_AcknowledgeConfig(BH1750_I2C, ENABLE);
        // printf("Read EV5 failed!\r\n");
        return;
    }

    // 步骤2：主机发送BH1750读地址，等待EV6事件（接收模式：I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED）
    // I2C_Send7bitAddress：发送7位器件地址，硬件自动拼接「读位1」为8位地址（0x47）
    // 第三个参数：I2C_Direction_Receiver=1，表示主机为接收方，BH1750为发送方（主收从发）
    I2C_Send7bitAddress(BH1750_I2C, BH1750_ADDR, I2C_Direction_Receiver);
    // 等待EV6（接收模式）：检测SR1.ADDR=1+SR2.MSL=1+SR1.RXNE=1，证明：
    // 1. BH1750检测到读地址匹配，发送了ACK；
    // 2. 主机进入接收模式，接收寄存器为空，准备接收数据
    if (I2C_WaitEvent(BH1750_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED, BH1750_I2C_TIMEOUT) == ERROR)
    {
        I2C_GenerateSTOP(BH1750_I2C, ENABLE);
        I2C_AcknowledgeConfig(BH1750_I2C, ENABLE);
        // printf("Read EV6 failed!\r\n");
        return;
    }

    // 步骤3：读取高字节（MSB），先使能ACK，再等待EV7事件
    I2C_AcknowledgeConfig(BH1750_I2C, ENABLE); // 使能ACK，原因：高字节不是最后一个字节，主机需要发送ACK告知BH1750“继续发送下一个字节（低字节）”
    // 等待EV7事件（I2C_EVENT_MASTER_BYTE_RECEIVED）：检测SR1.RXNE=1（接收寄存器非空），
    // 证明BH1750已将高字节数据送入主机接收寄存器，此时读取才有效（若提前读取，寄存器为空，得到无效值）
    if (I2C_WaitEvent(BH1750_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED, BH1750_I2C_TIMEOUT) == ERROR)
    {
        I2C_GenerateSTOP(BH1750_I2C, ENABLE);
        I2C_AcknowledgeConfig(BH1750_I2C, ENABLE);
        // printf("Read EV7-1 failed!\r\n");
        return;
    }
    buf[0] = I2C_ReceiveData(BH1750_I2C); // 读取接收寄存器中的高字节，硬件自动清除RXNE标志位，为下一次接收做准备

    // 步骤4：读取低字节（LSB），先禁用ACK，再等待EV7事件（核心！读操作关键）
    I2C_AcknowledgeConfig(BH1750_I2C, DISABLE); // 禁用ACK，原因：低字节是最后一个字节，主机需要发送NACK告知BH1750“数据读取完成，停止发送”
                                                // 若不禁用ACK，主机会发送ACK，BH1750会继续发送无效数据，导致I2C总线死锁
    // 等待EV7事件，检测BH1750已将低字节送入接收寄存器
    if (I2C_WaitEvent(BH1750_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED, BH1750_I2C_TIMEOUT) == ERROR)
    {
        I2C_GenerateSTOP(BH1750_I2C, ENABLE);
        I2C_AcknowledgeConfig(BH1750_I2C, ENABLE);
        // printf("Read EV7-2 failed!\r\n");
        return;
    }
    buf[1] = I2C_ReceiveData(BH1750_I2C); // 读取接收寄存器中的低字节

    // 步骤5：主机发送停止信号，恢复ACK使能，结束I2C读操作
    I2C_GenerateSTOP(BH1750_I2C, ENABLE);      // 硬件自动生成停止信号，结束读通信，BH1750回到工作状态
    I2C_AcknowledgeConfig(BH1750_I2C, ENABLE); // 恢复ACK使能，原因：下一次I2C通信（如重新配置BH1750）是写操作，
                                               // 需要主机检测BH1750的ACK，若ACK禁用，会导致写地址失败

    // 步骤6：数据转换与校准（结合BH1750器件手册）
    raw_data = (buf[0] << 8) | buf[1];      // 合并16位原始数据：高字节左移8位，与低字节按位或（大端格式必须高字节在前）
    bh1750_hw_lux = (float)raw_data / 1.2f; // 计算实际光照强度
}

// 调试打印函数
void BH1750_HW_Debug(void)
{
    printf("BH1750 Light: %.1f lx\r\n", bh1750_hw_lux);
}
