/**
 * @file    esp8266.h
 * @brief   ESP8266 WiFi模块驱动头文件
 * @details 本文件定义了ESP8266 WiFi模块与STM32通信的所有功能接口、数据结构和宏定义
 *          支持WiFi连接、TCP/UDP通信、MQTT物联网协议连接等功能
 * @author  STM32 Developer
 * @date    2024
 * @version 1.0
 */

#ifndef _ESP8266_H_
#define _ESP8266_H_

/**
 * @brief 引入标准库头文件
 */
#include "stm32f10x.h" // STM32标准外设库

/**
 * @brief Flash存储配置
 */
#define FLASH_SAVE_ADDR 0x7FF000 ///< W25Q64最后一个4KB扇区地址
#define CONFIG_MAGIC 0x55AA      ///< 配置有效性标志

typedef struct
{
    uint8_t name[50];
    uint8_t word[50];
} WIFI;

/**
 * @brief 应用配置存储结构体
 */
typedef struct
{
    uint16_t magic; ///< 校验标志
    WIFI data;      ///< WiFi配置数据
} AppConfig_t;

void Save_WifiInfo(void);
void Load_WifiInfo(void);

typedef struct
{
    uint8_t rxbuff[512]; ///< 接收数据缓冲区，最大存储512字节
    uint16_t rxnum;      ///< 当前已接收数据的字节数
    uint8_t over;        ///< 接收完成标志：1表示一帧数据接收完成
    uint8_t state;       ///< ESP8266当前工作状态（用于状态机）
} ESP;

extern WIFI wifi;
/**
 * @brief 全局ESP8266结构体实例
 * @details 在中断服务函数中会被更新，用于存储接收到的数据
 */
extern ESP esp;

/**
 * @brief WiFi热点配置宏
 * @note 请根据实际情况修改WiFi名称和密码
 */
#define WIFI_SSID "X"        ///< WiFi热点名称（SSID）
#define WIFI_PASS "12345678" ///< WiFi热点密码

/**
 * @brief MQTT服务器配置宏
 * @details 用于连接OneNET物联网平台
 */
#define MQTT_SERVER_IP "mqtts.heclouds.com" ///< MQTT服务器IP地址（OneNET）
#define MQTT_SERVER_PORT "1883"             ///< MQTT服务器端口号（1883为非加密端口）

/**
 * @brief MQTT设备认证信息宏
 * @details 用于OneNET平台设备身份验证
 */
#define MQTT_PRODUCT_ID "4NZk701N39" ///< 产品ID（OneNET平台分配）
#define MQTT_DEVICE_NAME "Deno"      ///< 设备名称
/**
 * @brief 设备密码（包含Token信息）
 * @note 实际使用时请替换为正确的密码
 */
#define MQTT_PASSWORD "version=2018-10-31&res=products%2F4NZk701N39%2Fdevices%2FDeno&et=2087951246&method=md5&sign=F6QD7btNCSlly6T5yb1gCA%3D%3D"

/**
 * @brief MQTT主题宏定义
 * @details 用于发布数据和订阅消息
 */
#define MQTT_TOPIC_POST "$sys/" MQTT_PRODUCT_ID "/" MQTT_DEVICE_NAME "/thing/property/post" ///< 发布消息主题（上报属性）
#define MQTT_TOPIC_SET "$sys/" MQTT_PRODUCT_ID "/" MQTT_DEVICE_NAME "/thing/property/set"   ///< 订阅主题（接收平台下发命令）

/**
 * @brief ESP8266 AT指令宏定义
 * @details 这些是ESP8266模块的标准AT命令，用于配置和控制WiFi模块
 */
#define CMD_AT "AT\r\n"      ///< 测试ESP8266是否在线
#define CMD_RST "AT+RST\r\n" ///< 软件复位ESP8266模块

#define CMD_CWMODE_STA "AT+CWMODE=1\r\n" ///< 设置为STA（Station）模式，作为WiFi客户端
#define CMD_CWMODE_AP "AT+CWMODE=2\r\n"  ///< 设置为AP（Access Point）模式，作为WiFi热点
/**
 * @brief 设置AP模式参数
 * @param %s 为WiFi密码，%s为端口号
 */
#define CMD_CWSAP "AT+CWSAP=\"XXXX\",\"1234567890\",5,3\r\n"

// #define CMD_CWJAP "AT+CWJAP=\"" WIFI_SSID "\",\"" WIFI_PASS "\"\r\n" ///< 连接到指定WiFi热点

#define CMD_CIFSR "AT+CIFSR\r\n" ///< 查询本地IP地址

/**
 * @brief TCP/UDP连接配置宏
 */
#define CMD_CIPMUX_MULTI "AT+CIPMUX=1\r\n"  ///< 开启多连接模式（最多支持5个TCP/UDP连接）
#define CMD_CIPMUX_SINGLE "AT+CIPMUX=0\r\n" ///< 设置为单连接模式
#define CMD_CIPSERVER "AT+CIPSERVER=1\r\n"  ///< 开启TCP服务器（需要在多连接模式下）
#define CMD_CIPMODE "AT+CIPMODE=1\r\n"      ///< 开启透传模式（Transparent Transmission）
#define CMD_CIPSEND "AT+CIPSEND\r\n"        ///< 准备发送数据（透传模式下使用）
#define CMD_CIPCLOSE "+++"                  ///< 退出透传模式（注意：+++之间不能有其他字符）
#define CMD_CIPCLOSE_RST "AT+CIPCLOSE\r\n"  ///< 关闭TCP/UDP连接

/**
 * @brief 建立TCP连接到MQTT服务器
 */
#define CMD_CIPSTART_MQTT "AT+CIPSTART=\"TCP\",\"" MQTT_SERVER_IP "\"," MQTT_SERVER_PORT "\r\n"

/**
 * @brief 拼多多服务器配置（用于获取网络时间）
 */
#define PIN_SERVER_IP "api.pinduoduo.com"                                 ///< 拼多多API服务器IP
#define PIN_SERVER_PORT "80"                                              ///< HTTP服务端口
#define CMD_CIPSTATUS "AT+CIPSTATUS\r\n"                                  ///< 查询连接状态
#define PIN_SERVER_GET "GET http://api.pinduoduo.com/api/server/_stm\r\n" ///< HTTP GET请求（获取服务器时间）

/**
 * @brief 建立TCP连接到拼多多服务器
 */
#define CMD_CIPSTART_PIN "AT+CIPSTART=\"TCP\",\"%s\",%s\r\n"

/* ==================== 函数声明 ==================== */

/**
 * @brief 配置UART3串口，用于与ESP8266 WiFi模块通信
 * @details 初始化USART3外设，设置波特率115200，配置GPIO引脚和中断
 */
void Uart3_Init(void);

/**
 * @brief 清除ESP8266接收缓冲区
 * @details 将接收缓冲区清零，重置接收计数器和完成标志
 */
void ESP_ClearRxBuffer(void);

/**
 * @brief 发送AT命令并等待响应
 * @param cmd 要发送的AT命令
 * @param ack 期望收到的响应关键词
 * @param outtime 超时时间（毫秒）
 * @return 1:收到期望响应 2:收到ERROR 0:超时
 */
uint8_t ESP_SendCmdAndAck(uint8_t *cmd, uint8_t *ack, uint16_t outtime);

/**
 * @brief 发送字符串到UART3（用于发送AT命令）
 * @param buff 要发送的字符串
 */
void Uart3_TxStr(uint8_t buff[]);

/**
 * @brief 建立MQTT连接
 * @return 1:连接成功 0:连接失败
 */
uint8_t MQTT_Connect(void);

/**
 * @brief 发布MQTT消息
 * @details 将传感器数据发布到MQTT服务器（OneNET平台）
 */
void MQTT_Publish(void);

/**
 * @brief 发送MQTT心跳包
 * @details 保持与MQTT服务器的连接活跃
 */
void MQTT_REQ(void);

/**
 * @brief ESP8266初始化和云端连接主函数
 * @details 这是一个状态机函数，每次调用执行一个步骤
 *          包括：测试模块、连接WiFi、建立TCP连接、连接MQTT服务器
 */
void UpDataYun(void);

/**
 * @brief 清除ESP8266接收缓冲区（重复声明）
 */
void ESP_ClearRxBuffer(void);

/**
 * @brief 订阅MQTT主题
 * @details 订阅平台下发命令的主题
 */
void MQTT_Subscribe(void);

/**
 * @brief 处理云端下发的数据
 * @details 解析收到的MQTT消息，执行相应操作（如控制LED）
 */
void YunDataHandle(void);

/**
 * @brief 解析拼多多服务器返回的时间数据
 * @return 1:解析成功并更新时间 0:解析失败
 */
uint8_t ESP_ParsePinduoduoData(void);
void ESP_AP(void);
#endif /* _ESP8266_H_ */
