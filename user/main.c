/* 标准库头文件 */
#include "stdio.h"
#include "string.h"
#include "time.h"

/* STM32标准外设库 */
#include "stm32f10x.h"
#include "stm32f10x_it.h"

/* FreeRTOS相关 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"
#include "timers.h"

/* 用户自定义头文件 */
#include "main.h"
#include "delay.h"

/* 硬件驱动 */
#include "key.h"
#include "led.h"
#include "lcd.h"
#include "usart1.h"
#include "esp8266.h"
#include "dht11.h"
#include "light.h"
#include "kqm.h"
#include "su_03t.h"
#include "rtc.h"
#include "Myi2c_bh1750.h"
#include "flash.h"
#include "bsp_lcd.h"
#include "jx_uart_send.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "tim.h"

/* 注释掉的头文件 */
// #include "bh1750.h"
// #include "stm32_eval_spi_flash.h"
// #include "zi.h"

// lvgl文件
#include "gui_guider.h"
#include "lvgl.h"
#include "lv_port_disp.h"

// 句柄
TaskHandle_t Led1TaskHandle = NULL;
TaskHandle_t Led2TaskHandle = NULL;
TaskHandle_t Led3TaskHandle = NULL;
TaskHandle_t Led4TaskHandle = NULL;
TaskHandle_t KeyTaskHandle = NULL;
TaskHandle_t RTC_TaskHandle = NULL;
TaskHandle_t Dht11TaskHandle = NULL;
TaskHandle_t KqmTaskHandle = NULL;
TaskHandle_t LightTaskHandle = NULL;
TaskHandle_t Bh1750TaskHandle = NULL;
TaskHandle_t LCDTaskHandle = NULL;
TaskHandle_t WifiTaskHandle = NULL;
TaskHandle_t LVGLTaskHandle = NULL;
// 消息队列
QueueHandle_t xQueue1 = NULL;
// 信号量
SemaphoreHandle_t xSemaphore1 = NULL;
// 事件组
EventGroupHandle_t myevent_group = NULL;
// 软件定时器
TimerHandle_t xTimer1 = NULL;
TimerHandle_t wifitimer = NULL;
TimerHandle_t aptimer = NULL;

SENSORDATA current_sensor_data = {0};

#define BIT_0 (1 << 0) // 状态
#define BIT_4 (1 << 4)
#define BIT_6 (1 << 6)
// LED任务
void LED1Task(void *pvParameters)
{
    Led_Init(); // 初始化LED
    for (;;)
    {
        // 获取事件组当前所有状态位的快照
        // BIT_0: 布防标志 (KEY1置位, KEY2清除)
        // BIT_4|BIT_6: 传感器报警标志
        EventBits_t uxBits = xEventGroupWaitBits(myevent_group,
                                                 BIT_0 | BIT_4 | BIT_6,
                                                 pdFALSE, // 退出时不清除任何位
                                                 pdFALSE, // 等待任意位 (这里主要为了读取状态)
                                                 100);    // 轮询周期100ms

        // 核心逻辑：先检查是否处于布防状态 (BIT_0)
        if ((uxBits & BIT_0) != 0)
        {
            // 已布防：根据传感器状态决定亮灭
            // 必须 BIT_4 和 BIT_6 同时满足才报警
            if ((uxBits & (BIT_4 | BIT_6)) == (BIT_4 | BIT_6))
            {
                LED1_ON(); // 报警：环境恶劣且已布防
                // printf("ALARM ON! Bits: %X\n", uxBits);
            }
            else
            {
                LED1_OFF(); // 正常：环境良好
                // printf("Normal. Bits: %X\n", uxBits);
            }
        }
        else
        {
            // 未布防 (BIT_0被KEY2清除)：强制灭灯
            LED1_OFF();
            // printf("Disarmed. Bits: %X\n", uxBits);
        }
    }
}
// LED2任务
void LED2Task(void *pvParameters)
{
    for (;;)
    {
        LED2_ON();
        vTaskDelay(200);
        LED2_OFF();
        LED3_ON();
        vTaskDelay(200);
        LED3_OFF();
        LED4_ON();
        vTaskDelay(200);
        LED4_OFF();
        vTaskDelay(200);
    }
}
// LED3任务
void LED3Task(void *pvParameters)
{
}
// LED4任务
void LED4Task(void *pvParameters)
{
}
// key任务
void KeyTask(void *pvParameters)
{
    KEY_Init(); // 初始化按键
    uint8_t Keyflag = 0;
    // 切换lvgl界面计数
    uint8_t lvgl_count = 1;
    uint8_t wrong_count = 1;
    for (;;)
    {
        vTaskDelay(10);
        Keyflag = KEY_Scan();
        if (Keyflag == KEY1_PRESS)
        {
            // 布防：设置BIT_0
            wrong_count++;
            if (wrong_count > 2)
            {
                wrong_count = 1;
            }
            if (wrong_count == 1)
            {
                xEventGroupSetBits(myevent_group, BIT_0);
                printf("KEY1_PRESS: 布防\n");
            }
            else if (wrong_count == 2)
            {
                // 撤防：清除BIT_0
                xEventGroupClearBits(myevent_group, BIT_0);
                printf("KEY2_PRESS: 撤防\n");
            }
        }
        else if (Keyflag == KEY2_PRESS)
        {
        }
        if (Keyflag == KEY3_PRESS)
        {
            lvgl_count++;
            if (lvgl_count > 2)
            {
                lvgl_count = 1;
            }
            if (lvgl_count == 1)
            {
                lv_scr_load(guider_ui.screen_open);
            }
            else if (lvgl_count == 2)
            {
                lv_scr_load(guider_ui.screen_data);
            }
        }
        else if (Keyflag == KEY4_PRESS)
        {
            esp.state = 1;
            lv_scr_load(guider_ui.screen_wifi);
            printf("ap模式 \r\n");
        }
        vTaskDelay(10);
    }
}
// RTC时间更新任务
void RTC_Task(void *pvParameters)
{
    RTC_Init(); // 初始化RTC
    SENSORDATA sensor_data = {0};
    for (;;)
    {
        RTC_GetTime(); // 更新RTC时间
        sensor_data.rtcdata[0] = info->tm_year + 1900;
        sensor_data.rtcdata[1] = info->tm_mon + 1;
        sensor_data.rtcdata[2] = info->tm_mday;
        sensor_data.rtcdata[3] = info->tm_hour;
        sensor_data.rtcdata[4] = info->tm_min;
        sensor_data.rtcdata[5] = info->tm_sec;
        sensor_data.rtcdata[6] = info->tm_wday;
        sensor_data.id = SENSOR_ID_RTC;
        xQueueSend(xQueue1, &sensor_data, portMAX_DELAY);
        vTaskDelay(1000);
    }
}
// DHt11数据读取任务
void Dht11Task(void *pvParameters)
{
    Dht11_Init(); // 初始化DHT11温湿度传感器
    SENSORDATA sensor_data = {0};
    for (;;)
    {
        // 进入临界区
        // taskENTER_CRITICAL();
        Dht11_ReadData(); // 读取DHT11温湿度传感器数据
        // 退出临界区
        // taskEXIT_CRITICAL();
        sensor_data.dht11data[0] = dht.tem;
        sensor_data.dht11data[1] = dht.hum;
        sensor_data.id = SENSOR_ID_DHT11;
        // 发送到消息队列,一直等待直到发送成功
        xQueueSend(xQueue1, &sensor_data, portMAX_DELAY);
        // 节拍2秒读取一次
        vTaskDelay(2000);
    }
}
// kqm数据读取任务
void KqmTask(void *pvParameters)
{
    Kqm_Init(); // 初始化空气质量传感器
    SENSORDATA sensor_data = {0};
    for (;;)
    {
        // 等待信号量,如果没有信号量,则等待
        xSemaphoreTake(xSemaphore1, portMAX_DELAY);
        Kqm_ReadData(); // 读取空气质量传感器数据
        sensor_data.kqmdata[0] = kqm.co2;
        sensor_data.kqmdata[1] = kqm.ch2o;
        sensor_data.kqmdata[2] = kqm.voc;
        sensor_data.id = SENSOR_ID_KQM;
        // 发送到消息队列,一直等待直到发送成功
        xQueueSend(xQueue1, &sensor_data, portMAX_DELAY);
        // printf("kqmdata: %.2f, %.2f, %.2f\n", sensor_data.kqmdata[0], sensor_data.kqmdata[1], sensor_data.kqmdata[2]);
        if (sensor_data.kqmdata[0] > 380)
        {
            /* code */
            // 当CO2浓度大于380时,设置事件组BIT_6
            xEventGroupSetBits(myevent_group, BIT_6);
        }
        else
        {
            // 当CO2浓度小于等于380时,清除事件组BIT_6
            xEventGroupClearBits(myevent_group, BIT_6);
        }
        if (sensor_data.kqmdata[2] > 0.5)
        {
            /* code */
            // 当VOC浓度大于380时,设置事件组BIT_6
            xEventGroupSetBits(myevent_group, BIT_4);
        }
        else
        {
            // 当VOC浓度小于等于380时,清除事件组BIT_6
            xEventGroupClearBits(myevent_group, BIT_4);
        }

        // 消息队列等待100ms
        // xQueueReceive(xQueue1, &sensor_data, 100);
        // Kqm_Debug(); // 打印空气质量数据
        // vTaskDelay(2000); // 500ms读取一次
    }
}
// 光照数据读取
void LightTask(void *pvParameters)
{
    Light_Init(); // 初始化光照传感器
    SENSORDATA sensor_data = {0};
    for (;;)
    {
        Light_ReadData(); // 读取光照传感器数据
        sensor_data.lightdata[0] = addbuff[0];
        sensor_data.lightdata[1] = addbuff[1];
        sensor_data.id = SENSOR_ID_LIGHT;
        // 发送到消息队列,一直等待直到发送成功
        xQueueSend(xQueue1, &sensor_data, portMAX_DELAY);
        // 消息队列等待100ms
        // xQueueReceive(xQueue1, &sensor_data, 100);
        // Light_Debug(); // 打印光照数据
        vTaskDelay(2000); // 2秒读取一次
    }
}
// bh1750数据读取任务
void Bh1750Task(void *pvParameters)
{
    // BH1750_I2C1_Init();    // 硬件初始化BH1750光照传感器
    BH1750_SoftI2C_Init(); // 软件初始化BH1750的I2C接口
    BH1750_Sensor_Init();  // 初始化BH1750传感器
    SENSORDATA sensor_data = {0};
    uint8_t debug_counter = 0;
    for (;;)
    {
        // 进入临界区
        taskENTER_CRITICAL();
        sensor_data.bh1750data = BH1750_Sensor_ReadLight(); // 读取BH1750传感器数据（已增强）
        // 退出临界区
        taskEXIT_CRITICAL();
        sensor_data.id = SENSOR_ID_BH1750;
        // 发送到消息队列,一直等待直到发送成功
        xQueueSend(xQueue1, &sensor_data, portMAX_DELAY);

        // // 每5次读取打印一次增强调试信息
        // if (++debug_counter >= 5)
        // {
        //     BH1750_Debug(); // 打印增强的BH1750数据
        //     debug_counter = 0;
        // }

        vTaskDelay(1000); // 1秒读取一次（更频繁）
    }
}

// LCD任务
void LCDTask(void *pvParameters)
{
    // LCD_Init();    // 初始化LCD
    // LCD_UI_Init(); // 初始化LCD UI界面
    u8 lcdbuff[50] = {0};
    SENSORDATA sensor_data = {0};
    for (;;)
    {
        // 从消息队列接收数据,一直等待直到接收成功
        xQueueReceive(xQueue1, &sensor_data, portMAX_DELAY);

        // 更新全局传感器数据，供MQTT任务使用（实现"复制函数"）
        switch (sensor_data.id)
        {
        case SENSOR_ID_DHT11:
            current_sensor_data.dht11data[0] = sensor_data.dht11data[0];
            current_sensor_data.dht11data[1] = sensor_data.dht11data[1];
            break;
        case SENSOR_ID_KQM:
            current_sensor_data.kqmdata[0] = sensor_data.kqmdata[0];
            current_sensor_data.kqmdata[1] = sensor_data.kqmdata[1];
            current_sensor_data.kqmdata[2] = sensor_data.kqmdata[2];
            break;
        case SENSOR_ID_LIGHT:
            current_sensor_data.lightdata[0] = sensor_data.lightdata[0];
            current_sensor_data.lightdata[1] = sensor_data.lightdata[1];
            break;
        case SENSOR_ID_BH1750:
            current_sensor_data.bh1750data = sensor_data.bh1750data;
            break;
        case SENSOR_ID_RTC:
            memcpy(current_sensor_data.rtcdata, sensor_data.rtcdata, sizeof(sensor_data.rtcdata));
            break;
        }

        // 根据传感器ID更新LCD数据
        switch (sensor_data.id)
        {
        case SENSOR_ID_DHT11:
            // printf("tem: %.1f hum: %.1f\n", sensor_data.dht11data[0], sensor_data.dht11data[1]);
            memset(lcdbuff, 0, sizeof(lcdbuff));
            sprintf((char *)lcdbuff, "温度:% .1f℃ ", sensor_data.dht11data[0]);
            lv_label_set_text(guider_ui.screen_data_label_tem, (char *)lcdbuff);

            sprintf((char *)lcdbuff, "湿度:% .1f%%", sensor_data.dht11data[1]);
            lv_label_set_text(guider_ui.screen_data_label_hum, (char *)lcdbuff);
            break;
        case SENSOR_ID_KQM:
            // printf("KQM: co2: %.2f ch2o: %.2f voc: %.2f\n", sensor_data.kqmdata[0], sensor_data.kqmdata[1], sensor_data.kqmdata[2]);
            memset(lcdbuff, 0, sizeof(lcdbuff));
            sprintf((char *)lcdbuff, "CO2  :% .2fppm", sensor_data.kqmdata[0]);
            lv_label_set_text(guider_ui.screen_data_label_CO2, (char *)lcdbuff);

            sprintf((char *)lcdbuff, "CH2O :% .2fppm", sensor_data.kqmdata[1]);
            lv_label_set_text(guider_ui.screen_data_label_CH2O, (char *)lcdbuff);

            sprintf((char *)lcdbuff, "VOC  :% .2fppm", sensor_data.kqmdata[2]);
            lv_label_set_text(guider_ui.screen_data_label_VOC, (char *)lcdbuff);
            break;
        case SENSOR_ID_LIGHT:
            // printf("light: %04d, yanwu: %04d\n", sensor_data.lightdata[0], sensor_data.lightdata[1]);
            memset(lcdbuff, 0, sizeof(lcdbuff));
            sprintf((char *)lcdbuff, "light:%02d lm", sensor_data.lightdata[0]);
            lv_label_set_text(guider_ui.screen_data_label_lm, (char *)lcdbuff);

            sprintf((char *)lcdbuff, "yanwu:%02d ppm", sensor_data.lightdata[1]);
            lv_label_set_text(guider_ui.screen_data_label_yanwu, (char *)lcdbuff);
            break;
        case SENSOR_ID_BH1750:
            // printf("BH1750: %.1f lx \n", sensor_data.bh1750data);
            memset(lcdbuff, 0, sizeof(lcdbuff));
            sprintf((char *)lcdbuff, "BH:%.1f lx", sensor_data.bh1750data);
            lv_label_set_text(guider_ui.screen_data_label_lx, (char *)lcdbuff);
            break;
        case SENSOR_ID_RTC:
            // printf("RTC: %04d, %02d, %02d, %02d, %02d, %02d, %d\n", sensor_data.rtcdata[0], sensor_data.rtcdata[1], sensor_data.rtcdata[2], sensor_data.rtcdata[3], sensor_data.rtcdata[4], sensor_data.rtcdata[5], sensor_data.rtcdata[6]);
            memset(lcdbuff, 0, sizeof(lcdbuff));
            sprintf((char *)lcdbuff, "%04d-%02d-%02d %02d:%02d", sensor_data.rtcdata[0], sensor_data.rtcdata[1], sensor_data.rtcdata[2], sensor_data.rtcdata[3], sensor_data.rtcdata[4]);
            lv_label_set_text(guider_ui.screen_data_label_time, (char *)lcdbuff);
            break;
        default:
            break;
        }
    }
}
// wifi任务
void WifiTask(void *pvParameters)
{

    Uart3_Init();
    for (;;)
    {
        vTaskDelay(100);
        if (esp.state == 1)
        {
            ESP_AP();
        }
        else if (esp.state >= 2 && esp.state < 5)
        {
            UpDataYun();
        }
        else if (esp.state == 5)
        {
            YunDataHandle();
        }
    }
}
// lvgl任务
void LVGLTask(void *pvParameters)
{
    while (1)
    {
        lv_task_handler();
        vTaskDelay(100);
    }
}
// 定时器回调函数
// void vTimerCallback(TimerHandle_t xTimer)
//{
//    // 获取定时器ID
//    uint32_t ulCount = (uint32_t)pvTimerGetTimerID(xTimer);
//    // printf("Timer ID: %d\n", ulCount);
//}
// WIFI回调函数
void wifibackFunction(TimerHandle_t xTimer)
{
    static uint8_t num = 0;
    uint32_t ulCount = (uint32_t)pvTimerGetTimerID(xTimer);
    if (ulCount == 2)
    {
        if (esp.state == 0)
        {
            num++;
            if (num >= 10)
            {
                esp.state = 2;
                printf("选择sta模式\r\n");
            }
        }
    }
    if (esp.state == 5)
    {
        MQTT_REQ();
    }
    static uint32_t report_cnt = 0;
}
// 任务状态显示定时器回调函数
uint8_t CPU_RunInfo[400] = {0};
void vCallbackFunction(TimerHandle_t xTimer1)
{
    memset(CPU_RunInfo, 0, 400);
    vTaskList((char *)&CPU_RunInfo);
    printf("---------------------------------------------\r\n");
    printf("任务名 任务状态 优先级 剩余栈 任务序号\r\n");
    printf("%s", CPU_RunInfo);
    printf("---------------------------------------------\r\n");
}
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 中断分组2（2位抢占+2位子优先级）
    USART1_Init();                                  // 初始化串口1
    TIM_Init();                                     // 初始化定时器
    sFLASH_Init();
    lv_init();           // 初始化lvgl
    lv_port_disp_init(); // 初始化lvgl显示接口

    setup_scr_screen_open(&guider_ui);  // 初始化打开界面
    setup_scr_screen_data(&guider_ui);  // 初始化数据界面
    setup_scr_screen_wifi(&guider_ui);  // 初始化wifi界面
    lv_scr_load(guider_ui.screen_open); // 加载欢迎界面
    // 创建队列，用于存储传感器数据，队列长度为5，每个元素大小为SENSORDATA结构体大小
    xQueue1 = xQueueCreate(5, sizeof(SENSORDATA));
    // 创建信号量
    xSemaphore1 = xSemaphoreCreateBinary();

    // 创建事件组
    myevent_group = xEventGroupCreate();
    // 创建软件定时器
    // xTimer1 = xTimerCreate(
    //     "Timer1",           // 定时器名称
    //     1000,               // 定时器周期，单位为系统时钟周期数（1000ms）
    //     pdTRUE,             // 自动重装载模式
    //     (void *)0,          // 传递给定时器回调函数的参数
    //     vCallbackFunction); // 定时器回调函数
    wifitimer = xTimerCreate("wifiTimer", 50000, pdTRUE, (void *)1, wifibackFunction);
    aptimer = xTimerCreate("apTimer", 1000, pdTRUE, (void *)2, wifibackFunction);
    // 启动定时器,第二个参数为阻塞时间,这里设置为0,表示不阻塞
    // xTimerStart(xTimer1, 0);
    xTimerStart(wifitimer, 0);
    xTimerStart(aptimer, 0);
    // 4. 创建任务
    BaseType_t xReturned; // 任务创建返回值变量

    // 创建LED1任务
    xReturned = xTaskCreate(
        LED1Task,         // 任务主体函数名称
        "led1",           // 任务的别名
        64,               // 任务站空间  128*4
        NULL,             // 任务主体函数传参
        1,                // 任务优先级
        &Led1TaskHandle); // 任务句柄
    // 创建LED2任务
    xReturned = xTaskCreate(LED2Task, "led2", 64, NULL, 1, &Led2TaskHandle);
    // 创建LED3任务
    // xReturned = xTaskCreate(LED3Task, "led3", 64, NULL, 1, &Led3TaskHandle);
    // 创建LED4任务
    // xReturned = xTaskCreate(LED4Task, "led4", 64, NULL, 1, &Led4TaskHandle);
    // 创建key任务
    xReturned = xTaskCreate(KeyTask, "key", 64, NULL, 3, &KeyTaskHandle);
    // 创建DHT11任务
    xReturned = xTaskCreate(Dht11Task, "dht11", 80, NULL, 2, &Dht11TaskHandle);
    // 创建kqm任务
    xReturned = xTaskCreate(KqmTask, "kqm", 80, NULL, 2, &KqmTaskHandle);
    // 创建光照任务
    xReturned = xTaskCreate(LightTask, "light", 80, NULL, 2, &LightTaskHandle);
    // 创建BH1750任务
    xReturned = xTaskCreate(Bh1750Task, "bh1750", 80, NULL, 2, &Bh1750TaskHandle);
    // 创建RTC时间更新任务
    xReturned = xTaskCreate(RTC_Task, "rtc", 128, NULL, 2, &RTC_TaskHandle);
    // 创建LCD任务
    xReturned = xTaskCreate(LCDTask, "lcd", 256, NULL, 4, &LCDTaskHandle);
    // 创建wifi任务
    xReturned = xTaskCreate(WifiTask, "wifi", 320, NULL, 4, &WifiTaskHandle);
    // 创建lvgl任务
    xReturned = xTaskCreate(LVGLTask, "lvgl", 512, NULL, 4, &LVGLTaskHandle);
    // 启动rtos, 开始调度任务.
    vTaskStartScheduler();
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    printf("Stack Overflow in task: %s\r\n", pcTaskName);
    while (1)
    {
    }
}
// 定义任务时间戳变量 (基于sys_tick_cnt的上次执行时间)
// static uint32_t esp_time = 0;
// static uint32_t reqtime = 0;
// static uint32_t yundatatime = 0;
// static uint32_t rtc_time = 0;
// static uint32_t kqm_time = 0;
// static uint32_t light_time = 0;
// static uint32_t su_03t_time = 0;
// static uint32_t dht11_time = 0;
// static uint32_t bh1750_time = 0;
// static uint32_t printf_time = 0;
// static uint32_t lcd_time = 0;
// static uint32_t gif_time = 0;
// int main(void)
// {
// NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 中断分组2（2位抢占+2位子优先级）
// SystemCoreClockUpdate();                        // 更新系统时钟
// SysTick_Config(SystemCoreClock / 1000);         // SysTick=1ms中断

// 1. 核心外设初始化
// USART1_Init();  // 初始化串口1
// Uart3_Config(); // 初始化串口3 (连接ESP8266)
// TIM_Init();         // 初始化定时器

// 2. 基础硬件初始化
// KEY_Init();   // 初始化按键
// Led_Config(); // 初始化LED

// RGB_Init();   // 初始化RGB LED
// RELAY_Init(); // 初始化继电器
// Bep_Init();   // 初始化蜂鸣器

// 3. 传感器初始化
// Dht11_Init();   // 初始化DHT11温湿度传感器
// Kqm_Init();     // 初始化空气质量传感器
// Light_Config(); // 初始化光照传感器
// Su03t_Init();          // 初始化语音模块
// BH1750_I2C1_Init();    // 硬件初始化BH1750光照传感器
// BH1750_SoftI2C_Init(); // 软件初始化BH1750的I2C接口（二选一）
// BH1750_Sensor_Init();  // 初始化BH1750传感器

//  4. 其他功能初
//  Pwm_Init();     // 初始化PWM
// RTC_Init(); // 初始化RTC
//  Flash_Config(); // 初始化W25Q64芯片
//  LCD_Init();       // 初始化LCD
//  LCD_Clear(BLACK); // 清屏
//  LCD_SetCursor(0, 0); // 设置光标位置
//  LCD_ShowString(0, 0, 128, 16, 16, "Hello World!");
//  LCD_ShowPhoto(0, 16, 240, 240, (uint8_t *)gImage_haimian); // 显示图片
//  LCD_ShowPhoto(3, 3, 20, 19, (uint8_t *)gImage_Icon_wendu);    // 温度
//  LCD_ShowPhoto(3, 26, 20, 19, (uint8_t *)gImage_Icon_shidu);   // 湿度
//  LCD_ShowPhoto(3, 49, 20, 19, (uint8_t *)gImage_Icon_light);   // 光照
//  LCD_ShowPhoto(3, 72, 20, 19, (uint8_t *)gImage_Icon_yanwu);   // 烟感
//  LCD_ShowPhoto(3, 95, 20, 19, (uint8_t *)gImage_Icon_co2);     // CO2
//  LCD_ShowPhoto(3, 118, 20, 19, (uint8_t *)gImage_Icon_ch2o);   // CH2O
//  LCD_ShowPhoto(3, 141, 20, 20, (uint8_t *)gImage_Icon_voc);    // VOC
//  LCD_ShowPhoto(3, 164, 20, 20, (uint8_t *)gImage_Icon_bh1750); // bh1750
//  LCD_ShowChinese(3, 250, 16, (uint8_t *)FONT_ZI_HAO);          // 好
//  LCD_ShowChinese(19, 250, 16, (uint8_t *)FONT_ZI_NI);          // 你

// printf("init ok\r\n");

// // 读取0x123456地址开始 of the 10 bytes
// u8 buff2[10] = {0};
// Flash_ReadData(buff2, 10, 0x123456);
// printf("buff:");

// // 打印buff2数组的内容
// for (u8 i = 0; i < 10; i++)
// {
//     printf("%02X ", buff2[i]);
// }
// printf("\r\n");

// // 擦除0x123456地址开始 of the 4KB sector
// Flash_Delete(0x123456);
// printf("Delete 0x123456 OK\r\n");
// Delay_nms(1000);
// // 再次读取0x123456地址开始 of the 10 bytes
// u8 buff[10] = {0};
// Flash_ReadData(buff, 10, 0x123456);
// printf("buff:");

// // 打印buff数组的内容
// for (u8 i = 0; i < 10; i++)
// {
//     printf("%02X ", buff[i]);
// }
// printf("\r\n");

// // 写数据到0x123456地址开始 of the 10 bytes
// u8 buff1[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
// Flash_WriteData(buff1, 10, 0x123456);
// // 读取0x123456地址开始 of the 10 bytes
// Flash_ReadData(buff, 10, 0x123456);
// printf("buff:");
// for (u8 i = 0; i < 10; i++)
// {
//     printf("%02X ", buff[i]);
// }
// printf("\r\n");

// for (u16 i = 0; i < 100; i++)
// {
//     keyflage=KEY_Scan();
//     if(keyflage==KEY1_PRESS){
//         esp.state=1;
//     }
// }

// while (1)
// {

// if ((uint32_t)(sys_tick_cnt - esp_time) >= 100)
// {
//     esp_time = sys_tick_cnt;
//     if (esp.state == 1)
//     {
//         ESP_AP();
//     }
//     else if (esp.state >= 2)
//     {
//         UpDataYun();
//     }
// }
// if ((uint32_t)(sys_tick_cnt - reqtime) >= 50000)
// {
//     reqtime = sys_tick_cnt;
//     if (esp.state == 5)
//         MQTT_REQ();
// }
// // 云数据解析
// if ((uint32_t)(sys_tick_cnt - yundatatime) >= 500)
// {
//     yundatatime = sys_tick_cnt;
//     if (esp.state == 5)
//     {
//         YunDataHandle();
//     }
// }

// 1. 高频任务（500ms）
// RTC时间更新

// if ((uint32_t)(sys_tick_cnt - rtc_time) >= 1000)
// {
//     rtc_time = sys_tick_cnt;
//     RTC_GetTime();
// }

// 空气质量传感器数据更新
// if ((uint32_t)(sys_tick_cnt - kqm_time) >= 500)
// {
//     kqm_time = sys_tick_cnt;
//     Kqm_Data_Analyze();
// }
// 光照传感器数据更新
// if ((uint32_t)(sys_tick_cnt - light_time) >= 500)
// {
//     light_time = sys_tick_cnt;
//     for (u16 i = 0; i < 20; i++)
//     {
//         if (i % 2 == 0)
//         {
//             /* code */
//             printf("light %d\n", addbuff[i]);
//         }
//         else if (i % 2 == 1)
//         {
//             /* code */
//             printf("yanwu %d\n", addbuff[i]);
//         }
//     }
// }

// 2. 中频任务（1000ms）
// 语音模块数据更新
// if ((uint32_t)(sys_tick_cnt - su_03t_time) >= 1000)
// {
//     su_03t_time = sys_tick_cnt;
//     Su03t_Data_Analyze();
// }

// 温湿度传感器数据更新
// if ((uint32_t)(sys_tick_cnt - dht11_time) >= 1000)
// {
//     dht11_time = sys_tick_cnt;
//     Dht11_GetVal();
// }

// 3. 低频任务（2000ms）
// BH1750光照传感器数据更新
// float light;
// if ((uint32_t)(sys_tick_cnt - bh1750_time) >= 2000)
// {
//     bh1750_time = sys_tick_cnt;
//     light = BH1750_Sensor_ReadLight();
//     // printf("BH1750 Light: %.2f lx\r\n", light);
// }
// 传感器数据统一打印
//        if ((uint32_t)(sys_tick_cnt - printf_time) >= 2000)
//        {
//            printf_time = sys_tick_cnt;
//            printf("DHT11: %.2f C, %.2f %%RH\r\n", dht.tem, dht.hum);
//            printf("KQM: CO2: %.2f ppm, CH2O: %.2f ppm, VOC: %.2f ppm\r\n", kqm.co2, kqm.ch2o, kqm.voc);
//            printf("BH1750 Light: %.2f lx\r\n", light);
//            for (u16 i = 0; i < 2; i++)
//            {
//                if (i % 2 == 0)
//                {
//                    /* code */
//                    printf("light %d\n", addbuff[i]);
//                }
//                else if (i % 2 == 1)
//                {
//                    /* code */
//                    printf("yanwu %d\n", addbuff[i]);
//                }
//            }
//      }
// LCD显示更新
// if ((uint32_t)(sys_tick_cnt - lcd_time) >= 2000)
// {
//     lcd_time = sys_tick_cnt;
//     // 显示温度数值在温度图标右侧
//     char temp_str[12];
//     sprintf(temp_str, "%.2f C", dht.tem); // 与实际数据一致，保留两位小数
//     LCD_ShowString(25, 3, 128, 16, 16, (uint8_t *)temp_str);
//
//     // 显示湿度数值在湿度图标右侧
//     char hum_str[12];
//     sprintf(hum_str, "%.2f %%RH", dht.hum); // 与实际数据一致，保留两位小数
//     LCD_ShowString(25, 26, 128, 16, 16, (uint8_t *)hum_str);
//
//     // 显示光照数值在光照图标右侧
//     char light_str[12];
//     sprintf(light_str, "%d", addbuff[0]); // 直接显示ADC原始值
//     LCD_ShowString(25, 49, 128, 16, 16, (uint8_t *)light_str);
//
//     // 显示烟感数值在烟感图标右侧
//     char yanwu_str[12];
//     sprintf(yanwu_str, "%d", addbuff[1]);
//     LCD_ShowString(25, 72, 128, 16, 16, (uint8_t *)yanwu_str);
//
//     // 显示CO2数值在CO2图标右侧
//     char co2_str[12];
//     sprintf(co2_str, "%.2f ppm", kqm.co2); // 与实际数据一致，保留两位小数
//     LCD_ShowString(25, 95, 128, 16, 16, (uint8_t *)co2_str);
//
//     // 显示CH2O数值在CH2O图标右侧
//     char ch2o_str[12];
//     sprintf(ch2o_str, "%.2f ppm", kqm.ch2o); // 与实际数据一致，保留两位小数
//     LCD_ShowString(25, 118, 128, 16, 16, (uint8_t *)ch2o_str);
//
//     // 显示VOC数值在VOC图标右侧
//     char voc_str[12];
//     sprintf(voc_str, "%.2f ppm", kqm.voc); // 与实际数据一致，保留两位小数
//     LCD_ShowString(25, 141, 128, 16, 16, (uint8_t *)voc_str);
//
//     // 显示bh1750数值在bh1750图标右侧
//     char bh1750_str[12];
//     sprintf(bh1750_str, "%.2f lx", light); // 与实际数据一致，保留两位小数
//     LCD_ShowString(25, 164, 128, 16, 16, (uint8_t *)bh1750_str);
// }
// // 5. gif任务（1000ms）
// if ((uint32_t)(sys_tick_cnt - gif_time) >= 10)
// {
//     gif_time = sys_tick_cnt;
//     // 显示gif_1
//     LCD_ShowPhoto(3, 180, 50, 50, (uint8_t *)gImage_gif_1);
//     Delay_nms(100);
//     // 显示gif_2
//     LCD_ShowPhoto(3, 180, 50, 50, (uint8_t *)gImage_gif_2);
//     Delay_nms(100);
//     // 显示gif_3
//     LCD_ShowPhoto(3, 180, 50, 50, (uint8_t *)gImage_gif_3);
//     Delay_nms(100);
//     // 显示gif_4
//     LCD_ShowPhoto(3, 180, 50, 50, (uint8_t *)gImage_gif_4);
//     Delay_nms(100);
// }

// 4. 语音模块命令处理
// switch (su03t.date)
// {
// case 0x01: // 语音模块返回0x01时，打开LED1
//     LED1_ON();
//     su03t.date = 0;
//     break;
// case 0x10: // 语音模块返回0x10时，关闭LED1
//     LED1_OFF();
//     su03t.date = 0;
//     break;
// case 0x06: // 语音模块返回0x06时，开始传输二氧化碳浓度
//     _uart_co2_2(kqm.co2);
//     su03t.date = 0;
//     break;
// case 0x07: // 语音模块返回0x07时，开始传输甲醛浓度
//     _uart_ch2o_2(kqm.ch2o);
//     su03t.date = 0;
//     break;
// case 0x08: // 语音模块返回0x08时，开始传输VOC浓度
//     _uart_voc_2(kqm.voc);
//     su03t.date = 0;
//     break;
// case 0x09: // 语音模块返回0x09时，开始传输湿度
//     _uart_hum_2(dht.hum);
//     su03t.date = 0;
//     break;
// case 0x0A: // 语音模块返回0x0A时，开始传输温度
//     _uart_tem_2(dht.tem);
//     su03t.date = 0;
//     break;

//  default: // 无匹配指令时，保持当前状态
//     break;
// }
// }

//  }
// ==================== STM32低功耗模式 ====================

// /**
//  * @brief 睡眠模式 (Sleep Mode)
//  * 功耗：低
//  * 唤醒源：任意中断
//  * 特点：
//  * - 处理器停止， peripherals继续工作
//  * - 所有寄存器和内存内容保持
//  * - 唤醒时间短（微秒级）
//  * 注意事项：
//  * - 唤醒后程序从断点处继续执行
//  * - 不需要重新初始化系统
//  */
// // __WFI(); // Wait For Interrupt，进入睡眠模式
// //  __WFE(); // Wait For Event，进入睡眠模式

// /**
//  * @brief 停机模式 (Stop Mode)
//  * 功耗：极低
//  * 唤醒源：
//  * - WKUP引脚上升沿
//  * - RTC闹钟事件
//  * - USB唤醒事件
//  * - 外部中断线(EXTI Line)
//  * 特点：
//  * - 处理器和 peripherals停止
//  * - 内存内容保持
//  * - 唤醒时间中等（毫秒级）
//  * 注意事项：
//  * - 唤醒后需要重新初始化系统时钟
//  * - 必须调用SystemInit()重新配置时钟
//  */
// /* PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
//    SystemInit(); // 唤醒后必须重新初始化系统时钟
//    // 重新配置其他外设时钟 */

// /**
//  * @brief 待机模式 (Standby Mode)
//  * 功耗：最低
//  * 唤醒源：
//  * - WKUP引脚上升沿
//  * - RTC闹钟事件
//  * - IWDG复位
//  * 特点：
//  * - 几乎所有电路都停止工作
//  * - 只有备份寄存器和待机电路保持供电
//  * - 唤醒时间较长（毫秒级）
//  * - 相当于系统复位
//  * 注意事项：
//  * - 唤醒后程序从复位向量开始执行（相当于重新上电）
//  * - 所有寄存器和RAM内容丢失，除了备份寄存器
//  * - 必须清除Wake-Up标志位
//  * - 必须使能WKUP引脚唤醒功能
//  */
// /* PWR_ClearFlag(PWR_FLAG_WU); // 清除唤醒标志位
//    PWR_WakeUpPinCmd(ENABLE); // 使能WKUP引脚的唤醒功能
//    PWR_EnterSTANDBYMode(); // 进入待机模式
//    // 唤醒后程序从main函数开始执行 */

// // ==================== RTC唤醒相关配置 ====================

// /**
//  * @brief 配置RTC闹钟唤醒
//  * 用于从停机模式或待机模式唤醒系统
//  */
// void RTC_AlarmConfig(void)
// {
//     // 使能备份区域访问
//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
//     PWR_BackupAccessCmd(ENABLE);

//     // 配置RTC闹钟
//     RTC_AlarmCmd(DISABLE);               // 先禁用闹钟
//     RTC_SetAlarm(RTC_GetCounter() + 10); // 设置10秒后闹钟
//     RTC_AlarmCmd(ENABLE);                // 使能闹钟

//     // 使能RTC闹钟中断
//     RTC_ITConfig(RTC_IT_ALR, ENABLE);

//     // 配置NVIC
//     NVIC_InitTypeDef NVIC_InitStructure;
//     NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//     NVIC_Init(&NVIC_InitStructure);
// }

// /**
//  * @brief RTC中断处理函数
//  * 用于处理RTC闹钟事件
//  */
// void RTC_IRQHandler(void)
// {
//     if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
//     {
//         // 处理闹钟事件
//         RTC_ClearITPendingBit(RTC_IT_ALR); // 清除闹钟中断标志
//         RTC_WaitForLastTask();
//     }
// }
