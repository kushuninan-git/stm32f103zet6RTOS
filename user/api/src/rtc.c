#include "rtc.h"
#include "time.h"

#include <stdio.h>

/**
 * @brief Safe Wait For Synchro with timeout
 */
static void RTC_WaitForSynchro_Safe(void)
{
    uint32_t timeout = 0x000FFFFF;
    RTC_ClearFlag(RTC_FLAG_RSF);
    while ((RTC_GetFlagStatus(RTC_FLAG_RSF) == RESET) && (timeout-- > 0))
    {
    }
    if (timeout == 0)
    {
        printf("RTC Error: Synchro Timeout!\r\n");
    }
}

/**
 * @brief Safe Wait For Last Task with timeout
 */
static void RTC_WaitForLastTask_Safe(void)
{
    uint32_t timeout = 0x000FFFFF;
    while ((RTC_GetFlagStatus(RTC_FLAG_RTOFF) == RESET) && (timeout-- > 0))
    {
    }
    if (timeout == 0)
    {
        printf("RTC Error: LastTask Timeout!\r\n");
    }
}

/**
 * @brief Initialize RTC
 * @note  Forces usage of LSI (Internal Low Speed Oscillator) due to unstable LSE.
 */
void RTC_Init(void)
{
    // 0. Enable PWR and BKP clocks
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    // Check if RTC is already configured (0xA2A2) or if it's stopped (Counter < 100)
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA2A2 || RTC_GetCounter() < 100)
    {
        printf("RTC Init (LSI)...\r\n");

        // 1. Reset Backup Domain
        BKP_DeInit();

        // 2. Enable LSI (Internal Low Speed Oscillator)
        RCC_LSICmd(ENABLE);

        // Wait for LSI to be ready
        uint32_t timeout = 0x0FFFFF;
        while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET && timeout--)
        {
        }

        if (timeout == 0)
        {
            printf("RTC Error: LSI Start Failed!\r\n");
            return;
        }

        // 3. Configure RTC clock source to LSI
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

        // 4. Set RTC Prescaler
        // LSI freq is approx 40kHz. Prescaler = 40000 - 1 for 1Hz counter clock.
        RTC_SetPrescaler(40000 - 1);

        // 5. Enable RTC Clock
        RCC_RTCCLKCmd(ENABLE);

        // 6. Wait for synchronization
        RTC_WaitForSynchro_Safe();
        RTC_WaitForLastTask_Safe();

        // 7. Set Initial Time (2026-02-04 02:52:53 -> 1770173573)
        RTC_SetCounter(1770173573);
        RTC_WaitForLastTask_Safe();

        // 8. Write initialized flag
        BKP_WriteBackupRegister(BKP_DR1, 0xA2A2);

        printf("RTC Init Success.\r\n");
    }
    else
    {
        // Already configured, just wait for sync
        RTC_WaitForSynchro_Safe();
        RTC_WaitForLastTask_Safe();
    }
}

/**
 * @brief Get Current Time and Print it
 */

struct tm *info;
void RTC_GetTime(void)
{
    uint32_t sec = RTC_GetCounter(); // »ñÈ¡¼ÆÊýÖµ
    info = localtime(&sec);
}

/**
 * @brief Update RTC Time
 */
void RTC_UpData_Time(uint32_t sec)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);

    RTC_SetCounter(sec);
    RTC_WaitForLastTask_Safe();
}
