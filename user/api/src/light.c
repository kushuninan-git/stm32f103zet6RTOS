#include "light.h"
#include "usart1.h"


/**
 * @brief  光照传感器ADC采集配置函数
 * @note   功能说明：初始化ADC1+DMA1通道1，实现对PA5(光照)和PC1(烟雾)引脚模拟电压的连续DMA采集
 *         硬件对应：
 *         - PA5引脚（模拟输入）-> ADC1通道5
 *         - PC1引脚（模拟输入）-> ADC1通道11
 *         - DMA1通道1循环传输数据
 * @param  无
 * @retval 无
 */

/**
 * @brief  ADC采集数据缓冲区
 * @note   存储ADC1转换结果，长度为20，用于DMA循环传输
 */
u16 addbuff[2] = {0};

/**
 * @brief  光照传感器ADC采集配置函数
 * @param  无
 * @retval 无
 */
uint16_t light_val = 0;

void Light_Init(void)
{
    /* 1. 使能DMA1时钟
     * 说明：ADC采集数据通过DMA传输，需先开启DMA时钟
     */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    /* 2. 使能ADC1、GPIOA和GPIOC时钟
     * 说明：
     * - ADC1挂载在APB2总线，需开启对应时钟
     * - GPIOA为光照传感器引脚(PA5)所在端口
     * - GPIOC为烟雾传感器引脚(PC1)所在端口
     * - ADC外设需要分频，设置为PCLK2/6
     */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    /* 3. 定义并配置GPIO初始化结构体
     * 说明：将引脚配置为模拟输入模式
     */
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 3.1 配置PA5引脚（光照传感器）为模拟输入模式 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; /* 模拟输入模式 */
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 3.2 配置PC1引脚（烟雾传感器）为模拟输入模式 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* 4. 配置DMA1通道1（ADC1数据传输专用）
     * 步骤：复位DMA通道 → 配置核心参数 → 初始化 → 使能通道
     */
    DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(DMA1_Channel1); /* 恢复DMA至初始状态 */

    /* 4.1 配置DMA核心参数 */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;             /* ADC1数据寄存器地址 */
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&addbuff;                  /* 内存缓冲区地址 */
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          /* 传输方向：外设→内存 */
    DMA_InitStructure.DMA_BufferSize = 20;                                      /* 缓冲区大小：20个数据单元 */
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            /* 外设地址增量：禁用 */
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     /* 内存地址增量：启用 */
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; /* 外设数据宽度：半字(16位) */
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         /* 内存数据宽度：半字(16位) */
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             /* DMA模式：循环模式 */
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                         /* DMA优先级：高 */
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                /* M2M模式：禁用 */

    /* 4.2 初始化并使能DMA1通道1 */
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);

    /* 5. 配置ADC1核心参数 */
    ADC_InitTypeDef ADC_InitStructure;

    /* 5.1 配置ADC基本参数 */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  /* ADC模式：独立模式 */
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                        /* 扫描模式：启用（多通道采集） */
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  /* 连续转换模式：启用 */
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; /* 触发方式：无（软件触发） */
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              /* 数据对齐方式：右对齐 */
    ADC_InitStructure.ADC_NbrOfChannel = 2;                             /* 规则组通道个数：2个 */

    /* 5.2 初始化ADC1 */
    ADC_Init(ADC1, &ADC_InitStructure);

    /* 6. 配置ADC1规则通道
     * 说明：设置通道优先级和采样时间
     */
    /* 6.1 配置通道5（PA5，光照传感器），优先级1，采样时间55.5周期 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_55Cycles5);
    /* 6.2 配置通道11（PC1，烟雾传感器），优先级2，采样时间55.5周期 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_55Cycles5);

    /* 7. 使能ADC1的DMA请求
     * 说明：ADC转换完成后自动触发DMA传输
     */
    ADC_DMACmd(ADC1, ENABLE);

    /* 8. 使能ADC1外设 */
    ADC_Cmd(ADC1, ENABLE);

    /* 9. ADC校准（必须执行，保证采集精度）
     * 步骤：复位校准寄存器 → 等待复位完成 → 启动校准 → 等待校准完成
     */
    ADC_ResetCalibration(ADC1); /* 复位ADC1校准寄存器 */
    while (ADC_GetResetCalibrationStatus(ADC1))
    {
    }

    ADC_StartCalibration(ADC1); /* 启动ADC1校准 */
    while (ADC_GetCalibrationStatus(ADC1))
    {
    }

    /* 10. 启动ADC1软件触发转换
     * 说明：无外部触发时，通过软件启动连续采集
     */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
// 获取光照传感器读取结果（不含打印）
void Light_ReadData(void)
{
    uint16_t light = 0;
    // 等待转换完成 (在DMA模式下其实可以直接读addbuff，但这里保留原有的轮询逻辑如果需要，或者改为读addbuff)
    // 原逻辑是轮询10次取平均，这里改为直接返回DMA缓冲区的平均值或者保留原逻辑
    // 原逻辑使用了ADC_SoftwareStartConvCmd但配置是Continuous，所以EOC会一直触发。
    // 但是原逻辑中 while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)); 是针对非DMA的。
    // 配置了DMA后，数据会自动搬运到addbuff。
    // 既然原代码使用了ADC_GetConversionValue，那其实是读的DR寄存器。

    // 为了保持逻辑简单且非阻塞（RTOS下），我们直接读取addbuff中的值（DMA自动更新）
    // addbuff长度20，其中偶数下标是CH5(Light)，奇数下标是CH11(YanWu)
    // 这里简单取addbuff[0]作为光照值，或者取平均。

    // 既然原函数是 Light_Read() 且用了轮询，我将保留轮询逻辑但去掉打印，并返回数值。
    // 注意：原代码的轮询在DMA开启下可能有点多余，但为了稳妥起见，我还是按原逻辑修改，只是去掉打印。

    for (uint8_t i = 0; i < 10; i++)
    {
        while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
            ;
        light += ADC_GetConversionValue(ADC1);
    }
    light /= 10;
    light_val = light;
}

// 调试打印函数
void Light_Debug(void)
{
    printf("light:%d   %.1f\r\n", light_val, light_val / 4096.0 * 3.3);
}

// void LIGHT_GetADCValue()
// {
// }
// void YanWu_GetADCValue()
// {
//     uint16_t yanwu = 0;
//     for (uint8_t i = 0; i < 10; i++)
//     {
//         while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
//             ;
//         yanwu += ADC_GetConversionValue(ADC1);
//     }
//     yanwu /= 10;
//     printf("yanwu:%d   %.1f\r\n", yanwu, yanwu / 4096.0 * 3.3);

// uint16_t light = 0;
// uint16_t yanwu = 0;
// ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_55Cycles5);
// ADC_SoftwareStartConvCmd(ADC1, ENABLE);
// while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
//     ;
// light = ADC_GetConversionValue(ADC1);

// printf("light:%d   \r\n", light);
// ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);
// ADC_SoftwareStartConvCmd(ADC1, ENABLE);

// while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
//     ;
// yanwu = ADC_GetConversionValue(ADC1);

// printf("yanwu:%d  \r\n", yanwu);
// }
