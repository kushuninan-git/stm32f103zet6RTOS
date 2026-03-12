#include "esp8266.h"
#include "main.h"       // Includes FreeRTOS headers and task handle
#include "stdio.h"      // 标准输入输出（用于printf）
#include "string.h"     // 字符串处理函数
#include "stdint.h"     // 整数类型定义（如uint32_t）
#include "stdlib.h"     // 标准库（用于atoi等）
#include "delay.h"      // 延时函数
#include "MQTTPacket.h" // MQTT协议包
#include "led.h"        // LED灯控制
#include "rtc.h"        // 实时时钟
#include "flash.h"
#include "gui_guider.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "FreeRTOS.h"
#include "task.h"

extern SENSORDATA current_sensor_data;

WIFI wifi = {.name = WIFI_SSID, .word = WIFI_PASS};

/**
 * @brief 保存WiFi配置到Flash
 */
void Save_WifiInfo(void)
{
    AppConfig_t config;

    // 1. 准备数据
    config.magic = CONFIG_MAGIC;
    memcpy(&config.data, &wifi, sizeof(WIFI)); // 复制当前 wifi 全局变量到结构体

    // 2. 擦除扇区 (必须先擦除再写入)
    sFLASH_EraseSector(FLASH_SAVE_ADDR);

    // 3. 写入整个结构体
    sFLASH_WriteBuffer((uint8_t *)&config, FLASH_SAVE_ADDR, sizeof(AppConfig_t));

    printf("WiFi Config Saved to 0x%X\r\n", FLASH_SAVE_ADDR);
}

/**
 * @brief 从Flash加载WiFi配置
 */
void Load_WifiInfo(void)
{
    AppConfig_t config;

    // 1. 读取数据
    sFLASH_ReadBuffer((uint8_t *)&config, FLASH_SAVE_ADDR, sizeof(AppConfig_t));

    // 2. 校验数据是否有效
    if (config.magic == CONFIG_MAGIC)
    {
        // 数据有效，加载到全局变量
        memcpy(&wifi, &config.data, sizeof(WIFI));
        printf("WiFi Config Loaded: SSID=%s, PASS=%s\r\n", wifi.name, wifi.word);
    }
    else
    {
        // 数据无效（可能是第一次运行），保持默认值
        printf("No valid config found, using defaults.\r\n");
    }
}

/**
 * @brief 配置UART3串口，用于与ESP8266 WiFi模块通信
 *
 * 这个函数完成以下初始化工作：
 * 1. 开启GPIO和USART3的时钟
 * 2. 配置GPIO引脚（PB10=TX, PB11=RX）
 * 3. 配置ESP8266的复位引脚（PE6）
 * 4. 配置USART3串口参数（波特率115200等）
 * 5. 配置NVIC中断优先级
 * 6. 使能USART3
 */
void Uart3_Init(void)
{
    // 第一步：开启时钟
    // ESP8266使用的是USART3，挂载在APB1总线上
    // GPIOB用于USART3的TX/RX引脚，GPIOE用于ESP8266的复位引脚
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // 开启GPIOB时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); // 开启USART3时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);  // 开启GPIOE时钟（复位引脚）

    // 第二步：定义GPIO初始化结构体并清零
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    // 配置PB10为UART3的发送引脚（TX）
    // 复用推挽输出模式：CPU通过该引脚发送数据到ESP8266
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;        // 选择PB10引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; // 设置输出速度10MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   // 复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);            // 初始化GPIOB

    // 配置PB11为UART3的接收引脚（RX）
    // 浮空输入模式：CPU通过该引脚接收ESP8266返回的数据
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;            // 选择PB11引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);                // 初始化GPIOB

    // 配置PE6作为ESP8266的复位（RST）引脚
    // 当该引脚为低电平时，ESP8266复位；高电平时正常工作
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;        // 选择PE6引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出（可输出高低电平）
    GPIO_Init(GPIOE, &GPIO_InitStructure);           // 初始化GPIOE

    // 复位ESP8266模块
    // 将RST引脚拉高，让ESP8266退出复位状态开始工作
    GPIO_WriteBit(GPIOE, GPIO_Pin_6, Bit_SET); // RST引脚拉高（取消复位）
    Delay_nms(1000);                           // 延时1秒，等待ESP8266稳定上电

    // 第三步：配置USART3串口参数
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;                                    // 波特率115200，ESP8266默认波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                     // 8位数据位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                          // 1位停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                             // 无奇偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                 // 同时开启接收和发送模式
    USART_Init(USART3, &USART_InitStructure);                                       // 初始化USART3

    // 第四步：配置NVIC中断优先级
    // 当USART3收到数据或空闲时，会触发中断通知CPU处理
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;         // 选择USART3中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;        // 子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // 使能该中断
    NVIC_Init(&NVIC_InitStructure);                           // 初始化NVIC

    // 第五步：使能USART3相关中断
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // 使能接收缓冲区非空中断（收到数据时触发）
    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE); // 使能空闲中断（一帧数据接收完成时触发）

    // 第六步：使能USART3外设
    // 至此，USART3配置完成，可以开始收发数据
    USART_Cmd(USART3, ENABLE);
}
/**
 * @brief 发送一个字节数据到UART3串口
 *
 * @param txdata 要发送的字节数据
 *
 * 该函数会将字节数据发送到USART3的发送缓冲区（DR寄存器），
 * 并等待发送完成（TXE标志位为SET）。
 */
void Uart3_Tx(uint8_t txdata)
{
    USART_SendData(USART3, txdata);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
        ;
}
/**
 * @brief 发送一个字节数组到UART3串口
 *
 * @param buff 要发送的字节数组
 * @param len 数组长度
 *
 * 该函数会遍历字节数组中的每个元素，调用Uart3_Tx函数发送每个字节。
 */
void Uart3_TxBuff(uint8_t buff[], uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
        Uart3_Tx(buff[i]);
}
/**
 * @brief 发送一个字符串到UART3串口
 *
 * @param buff 要发送的字符串
 *
 * 该函数会遍历字符串中的每个字符，调用Uart3_Tx函数发送每个字符。
 * 发送完成后，会在字符串末尾添加一个换行符（'\n'）。
 */
void Uart3_TxStr(uint8_t buff[])
{
    while (*buff != '\0')
    {
        Uart3_Tx(*buff);
        buff++;
    }
}
/**
 * @brief ESP8266结构体，用于存储接收数据和状态
 *
 * 该结构体包含以下成员：
 * - state：接收状态标志，用于判断是否接收完成一帧数据
 * - over：接收完成标志，用于判断是否接收完成一帧数据
 * - rxnum：接收数据计数器，用于记录当前接收数据的索引
 * - rxbuff：接收数据缓冲区，用于存储接收到的数据
 */
ESP esp = {.state = 2};
/**
 * @brief USART3中断服务函数，用于处理接收数据和空闲中断
 *
 * 当USART3收到数据时，会触发接收缓冲区非空中断（USART_IT_RXNE），
 * 该中断会将接收到的数据存储到接收缓冲区（esp.rxbuff）中。
 *
 * 当一帧数据接收完成时，会触发空闲中断（USART_IT_IDLE），
 * 该中断会将接收状态标志（esp.over）设置为1。
 */
void USART3_IRQHandler(void)
{
    uint8_t data = 0;

    if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
    {
        data = USART3->DR;
        USART1->DR = data;

        esp.rxbuff[esp.rxnum++] = data;
        esp.rxnum %= 512;

        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }

    if (USART_GetITStatus(USART3, USART_IT_IDLE) == SET)
    {
        esp.over = 1;
        data = USART3->SR;
        data = USART3->DR;

        // 通知云数据解析任务处理数据
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        if (YunDataHandleTaskHandle != NULL)
        {
            vTaskNotifyGiveFromISR(YunDataHandleTaskHandle, &xHigherPriorityTaskWoken);
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }
    }
}

/**
 * @brief 清除ESP8266接收缓冲区
 * @details 将接收缓冲区全部清零，重置接收字节计数器和接收完成标志
 *          在每次发送命令前调用，确保接收数据是从头开始
 */
void ESP_ClearRxBuffer(void)
{
    memset(esp.rxbuff, 0, sizeof(esp.rxbuff)); // 将512字节缓冲区全部置0
    esp.rxnum = 0;                             // 重置接收计数器，从头开始接收
    esp.over = 0;                              // 清除接收完成标志
}

/**
 * @brief 发送AT命令到ESP8266（简化版）
 * @details 发送命令后自动添加回车换行符（\r\n），是ESP8266 AT命令的标准格式
 * @param cmd 要发送的命令字符串
 */
void ESP_SendCommand(char *cmd)
{
    Uart3_TxStr(cmd);    // 发送命令字符串
    Uart3_TxStr("\r\n"); // 发送回车换行符，结束命令
}

/**
 * @brief 发送AT命令并等待期望的响应
 * @details 这是一个阻塞式函数，发送命令后会一直等待响应或超时
 * @param cmd 要发送的命令
 * @param ack 期望在响应中看到的关键词（如"OK"、"ERROR"等）
 * @param outtime 超时时间，单位为毫秒
 * @return 1:成功（收到期望的ack） 2:失败（收到ERROR） 0:超时（什么都没收到）
 */
uint8_t ESP_SendCmdAndAck(uint8_t *cmd, uint8_t *ack, uint16_t outtime)
{
    ESP_ClearRxBuffer(); // 清除接收缓冲区，准备接收新数据

    Uart3_TxStr(cmd); // 发送AT命令

    // 等待响应或超时
    while (outtime--)
    {

        vTaskDelay(1); // 延时1毫秒，释放CPU
        Delay_nms(1);  // 延时1毫秒
        if (esp.over == 1)
        {
            // 在接收缓冲区中查找期望的关键词
            if (strstr((char *)esp.rxbuff, (char *)ack) != NULL)
                return 1; // 找到期望响应，成功

            // 检查是否返回错误
            if (strstr((char *)esp.rxbuff, (char *)"ERROR") != NULL)
                return 2; // 收到ERROR，失败
        }
    }
    return 0; // 超时，未收到任何有效响应
}

/**
 * @brief 状态机变量，记录当前执行到哪一步
 * @details 这是一个静态变量，每次调用UpDataYun时保持上次的值
 */
uint8_t num = 1;

/**
 * @brief ESP8266初始化和云端连接主函数（状态机）
 * @details 这是一个分阶段执行的状态机函数，每次调用只执行一个步骤
 *          完整流程包括11个步骤：
 *          1.测试模块 2.复位模块 3.设置WiFi模式 4.连接WiFi 5.设置单连接
 *          6.建立TCP连接 7.开启透传模式 8.准备发送 9.发送业务数据
 *          10.处理响应 11.周期性上报
 *
 * 工作流程：
 * - 第一次调用(num=1)：测试ESP8266是否在线
 * - 第二次调用(num=2)：复位ESP8266模块
 * - 第三次调用(num=3)：设置为STA模式，连接WiFi
 * - 连接到WiFi后，先连接拼多多服务器获取时间（state=3）
 * - 获取时间成功后，关闭连接，转而连接MQTT服务器（state=4）
 * - 连接MQTT成功后，进入周期性上报数据状态（state=4, num=11）
 */
void UpDataYun(void)
{
    uint8_t data = 0;

    // 使用switch语句实现状态机，每次只执行一个步骤
    switch (num)
    {
    case 1:
        // 步骤1：发送AT指令测试模块是否在线
        // 发送"AT"命令，如果模块正常会返回"OK"
        data = ESP_SendCmdAndAck((uint8_t *)CMD_AT, (uint8_t *)"OK", 100);
        if (data == 1)
            num++; // 成功后进入下一步
        break;

    case 2:
        // 步骤2：复位ESP8266模块
        // 发送"AT+RST"命令让模块软复位
        data = ESP_SendCmdAndAck((uint8_t *)CMD_RST, (uint8_t *)"OK", 1000);
        Delay_nms(1000); // 等待复位完成
        if (data == 1)
            num++;
        break;

    case 3:
        // 步骤3：设置WiFi模式为STA（Station）模式
        // STA模式：作为WiFi客户端，连接到其他热点
        data = ESP_SendCmdAndAck((uint8_t *)CMD_CWMODE_STA, (uint8_t *)"OK", 1000);
        if (data == 1)
        {
            esp.state = 3; // 设置内部状态为3，表示下一步要连接时间服务器
            num++;
        }
        break;

    case 4:
        // 步骤4：连接到指定的WiFi热点
        // 使用全局变量wifi.name和wifi.word（从Flash读取或配网获取）
        {

            uint8_t buff[100] = {0};
            // 使用全局变量中的WiFi信息
            Load_WifiInfo();
            sprintf((char *)buff, "AT+CWJAP=\"%s\",\"%s\"\r\n", wifi.name, wifi.word);

            printf("Connecting to WiFi: %s\r\n", wifi.name);

            data = ESP_SendCmdAndAck(buff, (uint8_t *)"OK", 10000);
            if (data == 1)
                num++;
        }

        break;

    case 5:
        // 步骤5：设置为单连接模式（CIPMUX=0）
        // 如果当前有连接存在（link is builded），直接设置会报错
        // 所以先尝试关闭所有连接
        ESP_SendCmdAndAck((uint8_t *)"AT+CIPCLOSE\r\n", (uint8_t *)"OK", 500);
        Delay_nms(500);

        // 单连接模式下一次只能建立一个TCP连接
        data = ESP_SendCmdAndAck((uint8_t *)CMD_CIPMUX_SINGLE, (uint8_t *)"OK", 1000);
        if (data == 1)
            num++;
        break;

    case 6:
        // 步骤6：建立TCP连接到服务器
        uint8_t buff[128] = {0};

        if (esp.state == 3)
        {
            // state=3：连接时间服务器（拼多多API）获取网络时间
            // 拼接TCP连接命令：AT+CIPSTART="TCP","api.pinduoduo.com",80
            sprintf((char *)buff, CMD_CIPSTART_PIN, PIN_SERVER_IP, PIN_SERVER_PORT);
        }
        else if (esp.state == 4)
        {
            // state=4：连接MQTT服务器（OneNET）
            // 拼接TCP连接命令：AT+CIPSTART="TCP","mqtts.heclouds.com",1883
            sprintf((char *)buff, CMD_CIPSTART_MQTT, MQTT_SERVER_IP, MQTT_SERVER_PORT);
        }

        data = ESP_SendCmdAndAck(buff, (uint8_t *)"OK", 10000);
        if (data == 1)
            num++;
        break;

    case 7:
        // 步骤7：开启透传模式（CIPMODE=1）
        // 透传模式下，串口收到的数据会直接透传到TCP连接，反之亦然
        data = ESP_SendCmdAndAck((uint8_t *)CMD_CIPMODE, (uint8_t *)"OK", 10000);
        if (data == 1)
            num++;
        break;

    case 8:
        // 步骤8：准备发送数据（CIPSEND）
        // 在透传模式下使用此命令进入发送状态
        data = ESP_SendCmdAndAck((uint8_t *)CMD_CIPSEND, (uint8_t *)">", 10000);
        if (data == 1)
            num++;
        break;

    case 9:
        // 步骤9：发送具体业务数据
        if (esp.state == 3)
        {
            // state=3：发送HTTP GET请求获取服务器时间
            ESP_ClearRxBuffer();
            Uart3_TxStr(PIN_SERVER_GET); // 发送HTTP请求
            num++;                       // 进入下一步等待解析响应
        }
        else if (esp.state == 4)
        {
            // state=4：发送MQTT CONNECT连接报文
            data = MQTT_Connect();
            if (data == 1)
                num++;
        }
        break;

    case 10:
        // 步骤10：处理服务器响应数据
        if (esp.state == 3)
        {
            // 解析时间服务器返回的HTTP响应
            data = ESP_ParsePinduoduoData();
            if (data == 1)
            {
                // 解析成功，现在需要关闭时间服务器连接，转而连接MQTT

                // 发送"+++"退出透传模式（注意：发送后要延时1秒）
                Uart3_TxStr("+++");
                Delay_nms(1000);
                Uart3_TxStr("AT+CIPSTATUS\r\n");
                data = ESP_SendCmdAndAck("AT+CIPSTATUS\r\n", "STATUS:4", 1000);

                // 关闭TCP连接
                // data = ESP_SendCmdAndAck(CMD_CIPCLOSE_RST, "OK", 10000);

                if (data == 1)
                {
                    esp.state = 4; // 切换到MQTT连接状态
                    num = 6;       // 跳回步骤6，重新建立TCP连接（这次连接MQTT）
                    Delay_nms(500);
                }
                else
                {
                    data = ESP_SendCmdAndAck(CMD_CIPCLOSE_RST, "OK", 10000);
                    if (data == 1)
                    {
                        esp.state = 4;
                        num = 6;
                        Delay_nms(500);
                    }
                }
            }
        }
        else if (esp.state == 4)
        {
            // state=4：订阅MQTT主题，接收平台下发的命令
            MQTT_Subscribe();
            num++;
        }
        break;

    case 11:
    {
        // 步骤11：周期性任务循环
        // 此步骤会反复执行，不断上报数据到云端
        static int data1 = 0; // Modified: Start from 0
        data1++;

        // 每隔约100次循环（约10秒，假设主循环100ms一次）
        // 或者当状态异常时，重新发布一次数据
        if (data1 >= 100 || esp.state == 0)
        {
            data1 = 0; // Reset counter
            // esp.state = 1; // FIX: Do NOT reset state to 1, stay connected!
            MQTT_Publish(); // 发布传感器数据到MQTT服务器
            printf("成功发布传感器数据到MQTT服务器\r\n");
            esp.state = 5;
            // MQTT_PublishText();
        }
    }
    break;
    }
}

/**
 * @brief 建立MQTT连接（CONNECT报文）
 * @details 使用MQTT库构造并发送CONNECT连接报文，完成与MQTT服务器的认证
 *          CONNECT报文包含：客户端ID、用户名、密码、KeepAlive时间等
 * @return 1:连接成功 0:连接失败
 */
uint8_t MQTT_Connect(void)
{
    ESP_ClearRxBuffer(); // 清除接收缓冲区，准备接收MQTT响应

    static uint8_t txbuff[512] = {0};                                 // 发送缓冲区（使用static避免栈溢出）
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer; // 连接参数结构体

    // 配置MQTT连接参数
    data.clientID.cstring = MQTT_DEVICE_NAME; // 设备名称作为客户端ID
    data.keepAliveInterval = 60;              // 心跳间隔60秒
    data.cleansession = 1;                    // 清除会话（clean session）
    data.username.cstring = MQTT_PRODUCT_ID;  // 用户名（产品ID）
    data.password.cstring = MQTT_PASSWORD;    // 密码（Token）

    // 使用MQTT库将连接参数序列化为二进制数据
    int len = MQTTSerialize_connect(txbuff, 512, &data);
    if (len <= 0)
        printf("connect包序列化失败\r\n"); // 序列化失败提示

    // 发送CONNECT报文到MQTT服务器
    Uart3_TxBuff(txbuff, len);

    Delay_nms(1000); // 等待服务器响应

    // 打印收到的响应（前4个字节用于调试）
    printf("MQTT Connect Resp: %02X %02X %02X %02X\r\n",
           esp.rxbuff[0], esp.rxbuff[1], esp.rxbuff[2], esp.rxbuff[3]);

    // 解析MQTT CONNACK响应
    // MQTT连接响应格式：0x20 0x02 0x00(return_code)
    if (esp.rxbuff[0] == 0x20 && esp.rxbuff[1] == 0x02)
    {
        if (esp.rxbuff[3] == 0x00) // 0x00表示连接成功
        {
            printf("connect连接成功\r\n");
            return 1;
        }
        else
        {
            printf("连接失败, 返回码: %02X\r\n", esp.rxbuff[3]); // 打印错误码
        }
    }
    return 0; // 连接失败
}

/**
 * @brief 发布MQTT消息（PUBLISH报文初始化）
 * @details 将传感器数据封装成JSON格式，通过MQTT PUBLISH报文发送到云端（OneNET平台）
 *          上报的参数包括：LED状态、温度、湿度、CO2、甲醛、光照等
 */
#include "stdint.h" // 确保int32_t等类型可用

void MQTT_Publish(void)
{
    ESP_ClearRxBuffer();
    static uint8_t txbuff[512] = {0};
    MQTTString topicString = MQTTString_initializer;
    static char payload[512];

    // 1. 数值强转+范围校验（重点修复lig）
    int led_status = 0; // 0:关 1:开
    double temp_val = (double)current_sensor_data.dht11data[0];
    int32_t hum_val = (int32_t)current_sensor_data.dht11data[1];
    float ch20_val = current_sensor_data.kqmdata[1];
    double co2_val = (double)current_sensor_data.kqmdata[0];
    co2_val = (co2_val < 0) ? 0 : (co2_val > 5000) ? 5000
                                                   : co2_val;
    // 关键：lig保留一位小数（匹配步长0.1，使用float类型）
    float lig_val = current_sensor_data.bh1750data;
    // 既然要修改物模型，这里就不需要强制取整了，直接使用原始浮点值
    // lig_val = roundf(lig_val);

    float voc_val = current_sensor_data.kqmdata[2];
    

    // Debug: 打印lig_val的值确认非0
    // printf("Debug: lig_val = %.4f\r\n", lig_val);

    // 2. 格式化JSON
    // 为了防止sprintf参数对齐问题导致浮点数解析错误，建议显式强转为double
    sprintf(payload, "{\"id\":\"123\",\"version\":\"1.0\",\"params\":{"
                     "\"led\":{\"value\":%d},"
                     "\"temp\":{\"value\":%.1f},"
                     "\"hum\":{\"value\":%d},"
                     "\"ch20\":{\"value\":%.2f},"
                     "\"co2\":{\"value\":%.1f},"
                     "\"lig\":{\"value\":%.1f}," // 浮点数格式，步长0.1
                     "\"temp_max\":{\"value\":26.0},"
                     "\"voc\":{\"value\":%.2f}"
                     "}}",
            1,
            temp_val,
            hum_val,
            (double)ch20_val, // ch20也强转double
            co2_val,
            (double)lig_val,  // 显式强转为double，确保参数传递正确
            (double)voc_val); // voc也强转double

    // Debug: 打印完整的Payload检查
    // printf("Payload: %s\r\n", payload);

    // 3. 序列化+发送（原有逻辑不变）
    topicString.cstring = MQTT_TOPIC_POST;
    int len = MQTTSerialize_publish(txbuff, 512, 0, 1, 0, 0x1111,
                                    topicString, (unsigned char *)payload,
                                    strlen((char *)payload));
    if (len <= 0)
        printf("publish包序列化失败\r\n");

    // 发送PUBLISH报文
    Uart3_TxBuff(txbuff, len);

    vTaskDelay(pdMS_TO_TICKS(1000)); // 释放CPU，让其他任务运行

    // 打印响应（用于调试）
    // printf("MQTT Publish Resp: %02X %02X %02X %02X\r\n",
    //        esp.rxbuff[0], esp.rxbuff[1], esp.rxbuff[2], esp.rxbuff[3]);

    // 解析PUBACK响应（QoS=1时服务器会返回PUBACK）
    // 格式：0x40 0x02 0x00(packet_id)
    if (esp.rxbuff[0] == 0x40 && esp.rxbuff[1] == 0x02)
    {
        if (esp.rxbuff[3] == 0x11) // packet_id = 0x1111 的低字节
            printf("publish成功\r\n");
    }
}
///**
// * @brief 发布MQTT消息（PUBLISH报文初始化）
// * @details 将传感器数据封装成JSON格式，通过MQTT PUBLISH报文发送到云端（OneNET平台）
// *          上报的参数包括：LED状态、温度、湿度、CO2、甲醛、光照等
// */
// void MQTT_PublishText(void)
//{
//    ESP_ClearRxBuffer(); // 清除接收缓冲区

//    static uint8_t txbuff[512] = {0};                // 发送缓冲区（static避免栈溢出）
//    MQTTString topicString = MQTTString_initializer; // 主题字符串

//    // 定义要上传的JSON数据（传感器数据）
//    uint8_t payload[] = "{\"id\":\"123\",\"version\":\"1.0\",\"params\":\
//{\"led\":{\"value\":1},\
//\"temp\":{\"value\":23.6},\
//\"hum\":{\"value\":24},\
//\"ch20\":{\"value\":0.06},\
//\"co2\":{\"value\":760.0 },\
//\"lig\":{\"value\":320.0},\
//\"temp_max\":{\"value\":26.0},\
//\"voc\":{\"value\":0.05}\
//}}";

//    // 设置发布主题为属性上报主题
//    topicString.cstring = MQTT_TOPIC_POST;

//    // 序列化PUBLISH报文
//    // 参数：缓冲区、QoS=1、retain=0、packetid=0x1111
//    int len = MQTTSerialize_publish(txbuff, 512, 0, 1, 0, 0x1111,
//                                    topicString, (unsigned char *)payload,
//                                    strlen((char *)payload));
//    if (len <= 0)
//        printf("publish包序列化失败\r\n");

//    // 发送PUBLISH报文
//    Uart3_TxBuff(txbuff, len);

//    Delay_nms(1000); // 等待服务器响应

//    // 打印响应（用于调试）
//    printf("MQTT Publish Resp: %02X %02X %02X %02X\r\n",
//           esp.rxbuff[0], esp.rxbuff[1], esp.rxbuff[2], esp.rxbuff[3]);

//    // 解析PUBACK响应（QoS=1时服务器会返回PUBACK）
//    // 格式：0x40 0x02 0x00(packet_id)
//    if (esp.rxbuff[0] == 0x40 && esp.rxbuff[1] == 0x02)
//    {
//        if (esp.rxbuff[3] == 0x11) // packet_id = 0x1111 的低字节
//            printf("publish成功\r\n");
//    }
//}
/**
 * @brief 订阅MQTT主题（SUBSCRIBE报文）
 * @details 订阅平台下发命令的主题，这样云端可以向设备发送控制指令
 *          例如：控制LED开关、设置参数等
 */
void MQTT_Subscribe(void)
{
    ESP_ClearRxBuffer(); // 清除接收缓冲区

    static uint8_t txbuff[512] = {0};                // 发送缓冲区（static避免栈溢出）
    MQTTString topicString = MQTTString_initializer; // 主题字符串
    int qos = 1;                                     // QoS等级：至少一次投递

    // 设置要订阅的主题（接收云端下发命令）
    topicString.cstring = MQTT_TOPIC_SET;

    // 序列化SUBSCRIBE报文
    // 参数：缓冲区、packetid=0x2222、主题数=1
    int len = MQTTSerialize_subscribe(txbuff, 512, 0, 0x2222, 1,
                                      &topicString, &qos);
    if (len <= 0)
        printf("subscribe包序列化失败\r\n");

    // 发送SUBSCRIBE报文
    Uart3_TxBuff(txbuff, len);

    Delay_nms(1000); // 等待服务器响应

    // 打印响应（用于调试）
    printf("MQTT Subscribe Resp: %02X %02X %02X %02X\r\n",
           esp.rxbuff[0], esp.rxbuff[1], esp.rxbuff[2], esp.rxbuff[3]);

    // 解析SUBACK响应
    // 格式：0x90 0x03 0x00(packet_id低字节) 0x00(packet_id高字节) 0x??(返回码)
    if (esp.rxbuff[0] == 0x90 && esp.rxbuff[1] == 0x03)
    {
        if (esp.rxbuff[2] == 0x22 && esp.rxbuff[3] == 0x22) // packet_id = 0x2222
        {
            if (esp.rxbuff[4] != 0x80) // 0x80表示订阅失败
                printf("subscribe成功\r\n");
            else
                printf("subscribe失败, 返回码: %02X\r\n", esp.rxbuff[4]);
        }
    }
}
/**
 * @brief 发送MQTT心跳包（PINGREQ报文）
 * @details 用于保持与MQTT服务器的连接活跃
 *          MQTT协议要求客户端定期发送心跳，如果超过KeepAlive时间未发心跳，服务器会断开连接
 */
void MQTT_REQ(void)
{
    ESP_ClearRxBuffer();

    uint8_t buff[2] = {0xC0, 0x00};
    Uart3_TxBuff(buff, 2);

    vTaskDelay(pdMS_TO_TICKS(500));

    if (esp.rxbuff[0] == 0xD0 && esp.rxbuff[1] == 0x00)
    {
        printf("mqtt 心跳成功\r\n");
    }
    else if (esp.over == 1)
    {
        printf("mqtt 心跳响应异常: %02X %02X\r\n", esp.rxbuff[0], esp.rxbuff[1]);
    }

    ESP_ClearRxBuffer();
}
/**
 * @brief 处理云端下发的数据
 * @details 解析从MQTT主题收到的命令消息，执行相应操作
 *          例如：收到{"led":1}则开灯，收到{"led":0}则关灯
 */
void YunDataHandle(void)
{
    if (esp.over == 1)
    {
        if (esp.rxbuff[0] == 0xD0 && esp.rxbuff[1] == 0x00)
        {
            ESP_ClearRxBuffer();
            return;
        }

        if (esp.rxnum < 10)
        {
            ESP_ClearRxBuffer();
            return;
        }

        printf("RX Buff: %s\r\n", esp.rxbuff);
        char *p = strstr((char *)&esp.rxbuff[10], "led");
        if (p != NULL)
        {
            if (*(p + 5) == '1')
                LED1_ON();
            else
                LED1_OFF();
        }

        ESP_ClearRxBuffer();
    }
}
/**
 * @brief 解析拼多多服务器返回的时间数据
 * @details 解析HTTP响应中的JSON数据，提取server_time字段的时间戳
 *          并将其转换为Unix时间戳，更新STM32的RTC时钟
 * @return 1:解析成功并更新时间 0:解析失败
 */
uint8_t ESP_ParsePinduoduoData(void)
{
    // 检查是否收到完整的一帧数据
    if (esp.over == 1)
    {
        // 打印接收到的完整数据，用于调试
        // printf("RX Buff: %s\r\n", esp.rxbuff);

        // 在HTTP响应中搜索"time"关键字（服务器返回的JSON中包含时间戳）
        // HTTP响应格式示例：{"server_time":1772528604311,...}
        char *p = strstr((char *)&esp.rxbuff[0], "time");
        if (p != NULL)
        {
            uint8_t buff[11] = {0};

            // 解析时间戳字符串
            // JSON格式："server_time":1772528604311
            //          p指向 "time" 的 't'
            // 字符位置：t(0) i(1) m(2) e(3) "(4) :(5) 1(6)
            // 所以数字起始位置是 p + 6
            char *num_start = p + 6;

            // 提取前10位（秒级时间戳，忽略毫秒）
            for (uint8_t i = 0; i < 10; i++)
                buff[i] = *(num_start + i);
            buff[10] = '\0'; // 确保字符串正确结束

            printf("Parsed time str: %s\r\n", buff);

            // 将字符串转换为整数（Unix时间戳）
            unsigned int sec = (unsigned int)atoi((char *)buff);

            // 判断时间戳是否有效（大于1600000000，即2020年）
            if (sec > 1600000000)
            {
                printf("Valid Time! Updating RTC to: %d\r\n", sec);
                // 转换为北京时间（UTC+8）：加上8小时的秒数
                RTC_UpData_Time(sec + 8 * 3600);
                lv_scr_load(guider_ui.screen_data);
                return 1; // 解析成功
            }
            else
            {
                printf("Invalid Time Value: %d\r\n", sec);
            }
        }
        else
        {
            printf("Keyword 'time' not found in response\r\n");
        }

        // 解析失败或完成，清除缓冲区
        memset(esp.rxbuff, 0, 512);
        esp.rxnum = 0;
        esp.over = 0;
    }
    return 0; // 解析失败
}
void ESP_AP(void)
{
    static uint8_t num1 = 1;
    switch (num1)
    {
    case 0: // 判断是否是透传模式

        break;
    case 1:
        Uart3_TxStr("AT\r\n");
        Delay_nms(100);
        Uart3_TxStr("AT+CWMODE=2\r\n");
        Delay_nms(100);
        Uart3_TxStr("AT+RST\r\n");
        Delay_nms(1000);
        Uart3_TxStr(CMD_CWSAP);
        Delay_nms(100);
        Uart3_TxStr("AT+CIFSR\r\n");
        Delay_nms(100); // ²éÑ¯IP
        Uart3_TxStr("AT+CIPMUX=1\r\n");
        Delay_nms(100);                    // ¿ªÆô¶àÁ´½Ó
        Uart3_TxStr("AT+CIPSERVER=1\r\n"); // 使用默认端口333
        Delay_nms(100);                    // ¿ªÆô·þÎñÆ÷
        ESP_ClearRxBuffer();
        num1++;
        break;

    case 2:
        if (esp.over == 1)
        {
            uint8_t data = 0;
            // 先清空wifi结构体，防止残留数据
            memset(&wifi, 0, sizeof(wifi));

            // ---------------------------------------------------------
            // 鲁棒性更强的解析逻辑：不依赖固定偏移
            // 1. 查找 "wifiname" 键
            char *p_key = strstr((char *)esp.rxbuff, "\"wifiname\"");
            if (p_key != NULL)
            {
                // 2. 从键开始向后找冒号
                char *p_colon = strchr(p_key, ':');
                if (p_colon != NULL)
                {
                    // 3. 从冒号开始向后找值的起始引号
                    char *p_val_start = strchr(p_colon, '"');
                    if (p_val_start != NULL)
                    {
                        p_val_start++; // 跳过起始引号，指向值的第一个字符

                        // 4. 开始拷贝，直到遇到结束引号
                        uint8_t i = 0;
                        while (*p_val_start != '"' && i < sizeof(wifi.name) - 1)
                        {
                            wifi.name[i++] = *p_val_start++;
                        }
                        wifi.name[i] = '\0';
                        data = 1;
                    }
                }
            }

            // 对 password 做同样的鲁棒性处理
            p_key = strstr((char *)esp.rxbuff, "\"password\"");
            if (p_key != NULL)
            {
                char *p_colon = strchr(p_key, ':');
                if (p_colon != NULL)
                {
                    char *p_val_start = strchr(p_colon, '"');
                    if (p_val_start != NULL)
                    {
                        p_val_start++; // 跳过起始引号
                        uint8_t i = 0;
                        while (*p_val_start != '"' && i < sizeof(wifi.word) - 1)
                        {
                            wifi.word[i++] = *p_val_start++;
                        }
                        wifi.word[i] = '\0';
                        if (data == 1)
                            data++;
                    }
                }
            }
            // ---------------------------------------------------------

            if (data == 2)
            {
                printf("Parsed: SSID=%s, PASS=%s\r\n", wifi.name, wifi.word);
                Save_WifiInfo();
                esp.state = 2;
                num = 1;
                // 配网成功，自动跳转到数据界面
                lv_scr_load(guider_ui.screen_data);
            }
            ESP_ClearRxBuffer();
        }
        break;
    }
}
