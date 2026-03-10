#ifndef __BH1750_DRIVER_H
#define __BH1750_DRIVER_H

#include "stm32f10x.h"
#include "delay.h"

// BH1750 设备地址
#define BH1750_ADDR_GND 0x23 // ADDR 接 GND
#define BH1750_ADDR_VCC 0x5C // ADDR 接 VCC

// BH1750 工作模式
#define BH1750_MODE_CONTINUOUS_HIGH 0x10 // 连续高分辨率模式（1lx，120ms）
#define BH1750_MODE_CONTINUOUS_LOW 0x13  // 连续低分辨率模式（4lx，16ms）
#define BH1750_MODE_ONE_TIME_HIGH 0x20   // 一次高分辨率模式
#define BH1750_MODE_ONE_TIME_LOW 0x23    // 一次低分辨率模式
#define BH1750_MODE_POWER_DOWN 0x00      // 断电模式

// I2C 应答定义（使用通用 I2C 定义）
#define BH1750_I2C_ACK SOFT_I2C_ACK
#define BH1750_I2C_NACK SOFT_I2C_NACK

// 错误码定义
#define BH1750_OK 0               // 操作成功
#define BH1750_ERR_ADDR_ACK 1     // 地址应答失败
#define BH1750_ERR_CMD_ACK 2      // 命令应答失败
#define BH1750_ERR_DATA_INVALID 3 // 数据无效
#define BH1750_ERR_COMM 4         // 通信错误
#define BH1750_ERR_TIMEOUT 5      // 超时错误

// 滤波模式
#define BH1750_FILTER_NONE 0       // 无滤波
#define BH1750_FILTER_MOVING_AVE 1 // 移动平均滤波
#define BH1750_FILTER_MEDIAN 2     // 中值滤波

// 功耗模式
#define BH1750_POWER_NORMAL 0 // 正常模式
#define BH1750_POWER_LOW 1    // 低功耗模式

// 传感器状态
#define BH1750_STATUS_OFFLINE 0 // 离线
#define BH1750_STATUS_ONLINE 1  // 在线
#define BH1750_STATUS_ERROR 2   // 错误

// 阈值状态
#define BH1750_THRESHOLD_NORMAL 0 // 在阈值范围内
#define BH1750_THRESHOLD_LOW 1    // 低于阈值
#define BH1750_THRESHOLD_HIGH 2   // 高于阈值

// 数据记录配置
#define BH1750_MAX_RECORD_SIZE 32 // 最大记录数量

// 传感器配置结构体
typedef struct
{
    uint8_t device_addr;       // 设备地址
    uint8_t filter_mode;       // 滤波模式
    uint8_t filter_depth;      // 滤波深度
    uint8_t power_mode;        // 功耗模式
    float calibration_factor;  // 校准系数
    float min_threshold;       // 最小阈值
    float max_threshold;       // 最大阈值
    uint16_t measure_interval; // 测量间隔（ms）
} BH1750_ConfigTypeDef;

// 传感器状态结构体
typedef struct
{
    uint8_t status;             // 传感器状态
    float current_lux;          // 当前光照强度
    float min_lux;              // 最小光照强度
    float max_lux;              // 最大光照强度
    float avg_lux;              // 平均光照强度
    uint8_t error_count;        // 错误计数
    uint32_t last_measure_time; // 上次测量时间
} BH1750_StatusTypeDef;

// 数据记录结构体
typedef struct
{
    float lux_values[BH1750_MAX_RECORD_SIZE]; // 光照强度记录
    uint8_t record_count;                     // 记录数量
    uint8_t current_index;                    // 当前索引
} BH1750_RecordTypeDef;

// 传感器句柄结构体
typedef struct
{
    SoftI2C_HandleTypeDef *hi2c; // 通用 I2C 句柄指针
    BH1750_ConfigTypeDef config; // 配置参数
    BH1750_StatusTypeDef status; // 状态信息
    BH1750_RecordTypeDef record; // 数据记录
    float filter_buffer[16];     // 滤波缓冲区
} BH1750_HandleTypeDef;

// 核心传感器操作函数
typedef uint8_t (*BH1750_InitFunc)(BH1750_HandleTypeDef *hdev);
typedef float (*BH1750_ReadFunc)(BH1750_HandleTypeDef *hdev);
typedef uint8_t (*BH1750_CheckFunc)(BH1750_HandleTypeDef *hdev);

// 初始化和配置函数
uint8_t BH1750_Init(BH1750_HandleTypeDef *hdev, SoftI2C_HandleTypeDef *hi2c, uint8_t addr);
uint8_t BH1750_Init_WithConfig(BH1750_HandleTypeDef *hdev, SoftI2C_HandleTypeDef *hi2c, BH1750_ConfigTypeDef *config);
void BH1750_DeInit(BH1750_HandleTypeDef *hdev);

// 数据校准功能
void BH1750_SetCalibrationFactor(BH1750_HandleTypeDef *hdev, float factor);
float BH1750_GetCalibrationFactor(BH1750_HandleTypeDef *hdev);

// 阈值检测功能
void BH1750_SetThreshold(BH1750_HandleTypeDef *hdev, float min_lux, float max_lux);
uint8_t BH1750_CheckThreshold(BH1750_HandleTypeDef *hdev);

// 数据滤波功能
void BH1750_SetFilterMode(BH1750_HandleTypeDef *hdev, uint8_t mode);
void BH1750_SetFilterDepth(BH1750_HandleTypeDef *hdev, uint8_t depth);
float BH1750_ApplyFilter(BH1750_HandleTypeDef *hdev, float raw_lux);

// 功耗管理功能
void BH1750_SetPowerMode(BH1750_HandleTypeDef *hdev, uint8_t mode);
void BH1750_SetMeasureInterval(BH1750_HandleTypeDef *hdev, uint16_t interval);
void BH1750_EnterSleep(BH1750_HandleTypeDef *hdev);
void BH1750_WakeUp(BH1750_HandleTypeDef *hdev);

// 多传感器支持
uint8_t BH1750_Init_Multi(BH1750_HandleTypeDef *hdev, SoftI2C_HandleTypeDef *hi2c, uint8_t addr);

// 数据记录功能
void BH1750_StartRecording(BH1750_HandleTypeDef *hdev);
void BH1750_StopRecording(BH1750_HandleTypeDef *hdev);
void BH1750_AddRecord(BH1750_HandleTypeDef *hdev, float lux);
float BH1750_GetMaxLux(BH1750_HandleTypeDef *hdev);
float BH1750_GetMinLux(BH1750_HandleTypeDef *hdev);
float BH1750_GetAvgLux(BH1750_HandleTypeDef *hdev);
void BH1750_ClearRecords(BH1750_HandleTypeDef *hdev);

// 错误处理功能
uint8_t BH1750_GetStatus(BH1750_HandleTypeDef *hdev);
uint8_t BH1750_GetErrorCount(BH1750_HandleTypeDef *hdev);
void BH1750_ResetErrorCount(BH1750_HandleTypeDef *hdev);

// 传感器操作函数
float BH1750_ReadLight(BH1750_HandleTypeDef *hdev);
uint8_t BH1750_CheckSensor(BH1750_HandleTypeDef *hdev);
void BH1750_SetPowerDown(BH1750_HandleTypeDef *hdev);
void BH1750_SetOneTimeMode(BH1750_HandleTypeDef *hdev);
void BH1750_SetContinuousMode(BH1750_HandleTypeDef *hdev);

// 自检功能
uint8_t BH1750_SelfTest(BH1750_HandleTypeDef *hdev);

#endif /* __BH1750_DRIVER_H */