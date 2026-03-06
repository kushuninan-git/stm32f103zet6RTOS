#include "key.h"
#include "delay.h" // 引用sys_tick_cnt变量
#include "tim.h" // 引用sys_ms变量

/**
 * @brief 按键定义数组
 * @details 定义每个按键的硬件连接和参数
 *
 * 结构体成员顺序:
 * Port, Pin, ActiveLevel, ShortID, LongID, State, PressTime
 */
static Key_t keys[] = {
    // KEY1: PA0, 高电平有效(按下为1), 短按返回1, 长按返回2
    {GPIOA, GPIO_Pin_0, 1, KEY1_PRESS, KEY1_LONG, KEY_STATE_IDLE, 0},

    // KEY2: PC4, 低电平有效(按下为0), 短按返回3, 长按返回4
    {GPIOC, GPIO_Pin_4, 0, KEY2_PRESS, KEY2_LONG, KEY_STATE_IDLE, 0},

    // KEY3: PC5, 低电平有效(按下为0), 短按返回5, 长按返回6
    {GPIOC, GPIO_Pin_5, 0, KEY3_PRESS, KEY3_LONG, KEY_STATE_IDLE, 0},

    // KEY4: PC6, 低电平有效(按下为0), 短按返回7, 长按返回8
    {GPIOC, GPIO_Pin_6, 0, KEY4_PRESS, KEY4_LONG, KEY_STATE_IDLE, 0}};

// 计算按键数量
#define KEY_COUNT (sizeof(keys) / sizeof(keys[0]))

/**
 * @brief 按键初始化函数
 * @details 配置GPIO为输入模式
 */
void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

    // 配置KEY1 (PA0) - 下拉输入 (默认低电平，按下变高)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // 下拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置KEY2/3/4 (PC4/5/6) - 上拉输入 (默认高电平，按下变低)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
 * @brief 读取按键物理状态
 * @param key 指向按键结构体的指针
 * @return 1:按下, 0:松开
 */
static uint8_t KEY_ReadPin(Key_t *key)
{
    // 读取引脚电平
    uint8_t level = GPIO_ReadInputDataBit(key->port, key->pin);

    // 判断是否等于有效电平
    if (level == key->active)
    {
        return 1; // 按下
    }
    else
    {
        return 0; // 松开
    }
}

/**
 * @brief 按键扫描函数 (非阻塞状态机)
 * @return 按键ID (0表示无按键)
 * @note 需要在主循环中不断调用，建议每10ms调用一次或直接在while(1)中调用
 */
uint8_t KEY_Scan(void)
{
    uint8_t i;
    uint8_t key_return = KEY_NONE;

    for (i = 0; i < KEY_COUNT; i++)
    {
        Key_t *key = &keys[i];
        uint8_t is_pressed = KEY_ReadPin(key); // 读取物理状态

        // 状态机处理
        switch (key->state)
        {
        // 状态1：空闲状态，等待按键按下
        case KEY_STATE_IDLE:
            if (is_pressed)
            {
                key->state = KEY_STATE_DEBOUNCE; // 进入消抖状态
                key->press_time = sys_ms;  // 记录按下时间(使用系统ms计数)
            }
            break;

        // 状态2：消抖状态
        case KEY_STATE_DEBOUNCE:
            if (is_pressed)
            {
                // 检查是否达到消抖时间 (利用无符号数溢出特性，直接相减即可)
                if ((uint32_t)(sys_ms - key->press_time) >= KEY_FILTER_TIME)
                {
                    key->state = KEY_STATE_PRESSED; // 确认按下
                }
            }
            else
            {
                key->state = KEY_STATE_IDLE; // 抖动，误触，返回空闲
            }
            break;

        // 状态3：确认按下状态，检测短按松开或长按
        case KEY_STATE_PRESSED:
            if (is_pressed)
            {
                // 按键一直按着，检查是否达到长按时间
                if ((uint32_t)(sys_ms - key->press_time) >= KEY_LONG_TIME)
                {
                    key_return = key->id_long;         // 返回长按键值
                    key->state = KEY_STATE_LONG_PRESS; // 进入长按保持状态
                }
            }
            else
            {
                // 按键松开了，且未达到长按时间 -> 判定为短按
                key_return = key->id_short;  // 返回短按键值
                key->state = KEY_STATE_IDLE; // 返回空闲
            }
            break;

        // 状态4：长按保持状态，等待松开
        case KEY_STATE_LONG_PRESS:
            if (!is_pressed)
            {
                key->state = KEY_STATE_IDLE; // 松开后返回空闲
            }
            break;

        default:
            key->state = KEY_STATE_IDLE;
            break;
        }

        // 如果检测到按键事件，立即返回（每次扫描只处理一个事件）
        if (key_return != KEY_NONE)
        {
            return key_return;
        }
    }

    return KEY_NONE;
}
