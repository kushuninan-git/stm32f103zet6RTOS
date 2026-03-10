#include "Myi2c_bh1750.h"

void BH1750_SoftI2C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct; // GPIO初始化结构体

    RCC_APB2PeriphClockCmd(BH1750_I2C_GPIO_CLK, ENABLE); // 使能GPIO时钟

    GPIO_InitStruct.GPIO_Pin = BH1750_I2C_SCL_PIN | BH1750_I2C_SDA_PIN; // 同时配置SCL和SDA引脚
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;                       // 开漏输出模式（I2C标准配置）
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;                      // 引脚输出速率50MHz
    GPIO_Init(BH1750_I2C_GPIO_PORT, &GPIO_InitStruct);                  // 初始化GPIO引脚

    BH1750_I2C_SCL_HIGH(); // 初始化SCL为高电平
    BH1750_I2C_SDA_HIGH(); // 初始化SDA为高电平
}

/**
 * @brief  软件I2C发送起始信号
 * @note   I2C起始信号时序：SCL高电平时，SDA由高变低
 * @param  无
 * @retval 无
 */
void BH1750_I2C_Start(void)
{
    BH1750_I2C_SCL_HIGH();          // SCL先置高
    BH1750_I2C_SDA_HIGH();          // SDA置高，进入I2C空闲状态
    Delay_nus(BH1750_I2C_DELAY_US); // 延时保证时序稳定

    BH1750_I2C_SDA_LOW();           // SCL高电平时，SDA拉低（起始信号核心）
    Delay_nus(BH1750_I2C_DELAY_US); // 延时保持起始信号稳定
    BH1750_I2C_SCL_LOW();           // SCL拉低，准备发送/接收数据（后续数据变化需在SCL低电平时进行）
}

/**
 * @brief  软件I2C发送1个字节数据（8位）
 * @note   高位先行，SCL低电平时更新SDA，SCL高电平时锁存数据
 * @param  data：要发送的8位数据
 * @retval 无
 */
void BH1750_I2C_SendByte(uint8_t data)
{

    for (u8 i = 0; i < 8; i++) // 循环发送8位，高位先送
    {
        BH1750_I2C_SCL_LOW(); // SCL拉低，允许SDA电平变化
        Delay_nus(BH1750_I2C_DELAY_US);

        // 判断当前发送位（7-i：从最高位bit7到最低位bit0）
        if (data & (0x80 >> i)) // 0x80是bit7掩码，右移i位对应当前位
        {
            BH1750_I2C_SDA_HIGH(); // 该位为1，SDA置高
        }
        else
        {
            BH1750_I2C_SDA_LOW(); // 该位为0，SDA置低
        }
        Delay_nus(BH1750_I2C_DELAY_US);

        BH1750_I2C_SCL_HIGH(); // SCL置高，锁存当前SDA电平（从机读取）
        Delay_nus(BH1750_I2C_DELAY_US);
    }

    BH1750_I2C_SCL_LOW(); // 发送完成后SCL拉低，释放总线
    Delay_nus(BH1750_I2C_DELAY_US);
}

/**
 * @brief  软件I2C接收1个字节数据（8位）
 * @note   高位先行，SDA需置为输入模式（通过SDA_HIGH()实现开漏输入），SCL高电平时读取数据
 * @param  无
 * @retval 接收到的8位数据
 */
uint8_t BH1750_I2C_ReceiveByte(void)
{
    uint8_t i = 0;
    uint8_t received_data = 0; // 接收数据缓存

    BH1750_I2C_SDA_HIGH(); // 开漏模式下，SDA置高即为输入模式

    for (i = 0; i < 8; i++) // 循环接收8位，高位先收
    {
        received_data <<= 1;  // 数据左移，为接收下一位做准备（高位先行）
        BH1750_I2C_SCL_LOW(); // SCL拉低，允许从机更新SDA电平
        Delay_nus(BH1750_I2C_DELAY_US);

        BH1750_I2C_SCL_HIGH(); // SCL置高，锁存SDA电平（主机读取）
        Delay_nus(BH1750_I2C_DELAY_US);

        if (BH1750_I2C_SDA_READ()) // 读取当前SDA电平
        {
            received_data |= 0x01; // 该位为1，最低位置1
        }
        // 该位为0时，无需操作（左移后最低位默认为0）
    }

    BH1750_I2C_SCL_LOW(); // 接收完成后SCL拉低，释放总线
    Delay_nus(BH1750_I2C_DELAY_US);

    return received_data; // 返回接收到的8位数据
}

/**
 * @brief  软件I2C发送应答/非应答信号
 * @note   应答信号在第9个SCL时钟位发送，SCL低电平时更新SDA，高电平时锁存
 * @param  ack：应答类型，BH1750_I2C_ACK(0)=应答，BH1750_I2C_NACK(1)=非应答
 * @retval 无
 */
void BH1750_I2C_SendAck(uint8_t ack)
{
    BH1750_I2C_SCL_LOW(); // SCL拉低，允许SDA电平变化
    Delay_nus(BH1750_I2C_DELAY_US);

    if (ack == BH1750_I2C_ACK)
    {
        BH1750_I2C_SDA_LOW(); // 发送应答：SDA置低
    }
    else
    {
        BH1750_I2C_SDA_HIGH(); // 发送非应答：SDA置高
    }
    Delay_nus(BH1750_I2C_DELAY_US);

    BH1750_I2C_SCL_HIGH(); // SCL置高，锁存应答信号（从机读取）
    Delay_nus(BH1750_I2C_DELAY_US);

    BH1750_I2C_SCL_LOW();  // 发送完成后SCL拉低
    BH1750_I2C_SDA_HIGH(); // 释放SDA总线
    Delay_nus(BH1750_I2C_DELAY_US);
}

/**
 * @brief  软件I2C接收从机的应答/非应答信号
 * @note   检测第9个SCL时钟位的SDA电平，高=非应答，低=应答
 * @param  无
 * @retval 应答状态：BH1750_I2C_ACK(0)=从机应答，BH1750_I2C_NACK(1)=从机非应答
 */
uint8_t BH1750_I2C_ReceiveAck(void)
{
    uint8_t ack_state = BH1750_I2C_NACK; // 默认为非应答

    BH1750_I2C_SDA_HIGH(); // 释放SDA，由从机控制应答电平
    BH1750_I2C_SCL_LOW();  // SCL拉低，准备检测
    Delay_nus(BH1750_I2C_DELAY_US);

    BH1750_I2C_SCL_HIGH(); // SCL置高，读取从机应答电平
    Delay_nus(BH1750_I2C_DELAY_US);

    if (BH1750_I2C_SDA_READ() == 0) // 检测SDA电平，低电平为应答
    {
        ack_state = BH1750_I2C_ACK;
    }
    // 高电平为非应答，保持默认值

    BH1750_I2C_SCL_LOW(); // 检测完成后SCL拉低
    Delay_nus(BH1750_I2C_DELAY_US);

    return ack_state; // 返回应答状态
}
/**
 * @brief  软件I2C发送停止信号
 * @note   I2C停止信号时序：SCL高电平时，SDA由低变高
 * @param  无
 * @retval 无
 */
void BH1750_I2C_Stop(void)
{
    BH1750_I2C_SCL_LOW();           // SCL先置低，保证SDA可安全变化
    BH1750_I2C_SDA_LOW();           // SDA置低，为停止信号做准备
    Delay_nus(BH1750_I2C_DELAY_US); // 延时保证时序稳定

    BH1750_I2C_SCL_HIGH();          // SCL置高（停止信号需SCL高电平）
    Delay_nus(BH1750_I2C_DELAY_US); // 延时保证SCL稳定高电平
    BH1750_I2C_SDA_HIGH();          // SCL高电平时，SDA拉高（停止信号核心）
    Delay_nus(BH1750_I2C_DELAY_US); // 延时保持停止信号
}

/**
 * @brief  向BH1750发送单字节命令
 * @note   统一处理I2C命令发送逻辑，减少重复代码
 * @param  cmd：要发送的命令字节
 * @retval BH1750_OK：成功
 *         BH1750_ERR_ADDR_ACK：地址应答失败
 *         BH1750_ERR_CMD_ACK：命令应答失败
 */
static uint8_t BH1750_I2C_SendCommand(uint8_t cmd)
{
    BH1750_I2C_Start();
    BH1750_I2C_SendByte((BH1750_DEVICE_ADDR << 1) | 0); // 设备地址 + 写标志
    if (BH1750_I2C_ReceiveAck() != BH1750_I2C_ACK)      // 检测从机应答
    {
        BH1750_I2C_Stop();
        return 1; // 发送失败
    }
    BH1750_I2C_SendByte(cmd); // 发送命令
    if (BH1750_I2C_ReceiveAck() != BH1750_I2C_ACK)
    {
        BH1750_I2C_Stop();
        return 1; // 发送失败
    }
    BH1750_I2C_Stop();
    return 0; // 发送成功
}

/**
 * @brief  初始化BH1750传感器
 * @note   初始化I2C引脚并配置传感器为连续高分辨率模式，增加失败检测
 * @param  无
 * @retval BH1750_OK：初始化成功
 *         BH1750_ERR_ADDR_ACK：地址应答失败
 *         BH1750_ERR_CMD_ACK：命令应答失败
 */
uint8_t BH1750_Sensor_Init(void)
{
    BH1750_SoftI2C_Init(); // 初始化软件I2C引脚

    // 发送连续高分辨率模式命令，检测是否成功
    if (BH1750_I2C_SendCommand(BH1750_MODE_CONTINUOUS_HIGH) != 0)
    {
        return 1; // 初始化失败
    }

    Delay_nms(180); // 等待传感器初始化完成（连续高分辨率模式需要180ms）

    return 0; // 初始化成功
}

/**
 * @brief  读取BH1750传感器的光照强度
 * @note   读取两个字节数据并转换为光照强度值，增加错误检测
 * @param  无
 * @retval 光照强度值（单位：lx），读取失败返回 -1.0f
 */
float BH1750_Sensor_ReadLight(void)
{
    uint8_t high_byte, low_byte;
    uint16_t raw_data;
    float light_intensity;

    // 发送起始信号并读取数据
    BH1750_I2C_Start();
    BH1750_I2C_SendByte((BH1750_DEVICE_ADDR << 1) | 1); // 设备地址 + 读标志
    if (BH1750_I2C_ReceiveAck() != BH1750_I2C_ACK)      // 检测从机应答
    {
        BH1750_I2C_Stop();
        return -1.0f; // 读取失败
    }

    // 读取高字节
    high_byte = BH1750_I2C_ReceiveByte();
    BH1750_I2C_SendAck(BH1750_I2C_ACK);

    // 读取低字节
    low_byte = BH1750_I2C_ReceiveByte();
    BH1750_I2C_SendAck(BH1750_I2C_NACK); // 最后一个字节后发送非应答

    BH1750_I2C_Stop();

    // 计算原始数据
    raw_data = (high_byte << 8) | low_byte;

    // 数据范围校验（BH1750最大原始值为65535）
    if (raw_data > 65535)
    {
        return -1.0f; // 数据无效
    }

    // 转换为光照强度（单位：lx），BH1750的分辨率为1lx时，1count=1/1.2lx
    light_intensity = (float)raw_data / 1.2f;

    return light_intensity;
}

/**
 * @brief  检测BH1750传感器是否在线
 * @note   通过发送起始信号并检测从机应答来判断传感器状态
 * @param  无
 * @retval 0：传感器在线
 *         1：传感器离线
 */
uint8_t BH1750_CheckSensor(void)
{
    BH1750_I2C_Start();
    BH1750_I2C_SendByte((BH1750_DEVICE_ADDR << 1) | 0); // 设备地址 + 写标志
    if (BH1750_I2C_ReceiveAck() == BH1750_I2C_ACK)      // 检测从机应答
    {
        BH1750_I2C_Stop();
        return 0; // 传感器在线
    }
    BH1750_I2C_Stop();

    return 1; // 传感器离线
}

/**
 * @brief  设置BH1750传感器为断电模式
 * @note   断电模式下传感器功耗极低
 * @param  无
 * @retval 无
 */
void BH1750_SetPowerDown(void)
{
    BH1750_I2C_SendCommand(0x00); // 断电模式命令
}

/**
 * @brief  设置BH1750传感器为一次高分辨率模式
 * @note   一次模式下，传感器测量完成后自动进入断电模式
 * @param  无
 * @retval 无
 */
void BH1750_SetOneTimeMode(void)
{
    BH1750_I2C_SendCommand(BH1750_MODE_ONE_TIME_HIGH); // 一次高分辨率模式

    Delay_nms(180); // 等待测量完成（一次高分辨率模式需要180ms）
}

/**
 * @brief  设置BH1750传感器为连续测量模式
 * @note   连续模式下，传感器会持续测量光照强度
 * @param  无
 * @retval 无
 */
void BH1750_SetContinuousMode(void)
{
    BH1750_I2C_SendCommand(BH1750_MODE_CONTINUOUS_HIGH); // 连续高分辨率模式

    Delay_nms(180); // 等待传感器稳定（连续高分辨率模式需要180ms）
}
