/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "stm32f10x.h"
/* ÎÒÃÇ¿ÉÒÔÌí¼ÓµÄºê¶¨Òå */
#define configUSE_TIME_SLICING 1                  // Ê¹ÄÜÊ±¼äÆ¬µ÷¶È(Ä¬ÈÏÊ½Ê¹ÄÜµÄ)
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1 // Ó²¼þ¼ÆËãÇ°µ¼ÁãÖ¸Áî£¬Èç¹ûËù
                                                  // Ê¹ÓÃµÄ£¬ MCU Ã»ÓÐÕâÐ©Ó²¼þÖ¸ÁîµÄ»°´ËºêÓ¦¸ÃÉèÖÃÎª 0
#define configUSE_TICKLESS_IDLE 0 // ÖÃ 1£ºÊ¹ÄÜµÍ¹¦ºÄ tickless Ä£Ê½£»ÖÃ 0£º
                                  // ±£³ÖÏµÍ³½ÚÅÄ£¨tick£©ÖÐ¶ÏÒ»Ö±ÔËÐÐ

#define configUSE_QUEUE_SETS 1          // ÆôÓÃ¶ÓÁÐ
#define configUSE_TASK_NOTIFICATIONS 1  // ¿ªÆôÈÎÎñÍ¨Öª¹¦ÄÜ£¬Ä¬ÈÏ¿ªÆô
#define configUSE_MUTEXES 1             // Ê¹ÓÃ»¥³âÐÅºÅÁ¿
#define configUSE_RECURSIVE_MUTEXES 1   // Ê¹ÓÃµÝ¹é»¥³âÐÅºÅÁ¿
#define configUSE_COUNTING_SEMAPHORES 1 // Îª 1 Ê±Ê¹ÓÃ¼ÆÊýÐÅºÅÁ¿
#define configQUEUE_REGISTRY_SIZE 10    // ÉèÖÃ¿ÉÒÔ×¢²áµÄÐÅºÅÁ¿ºÍÏûÏ¢¶ÓÁÐ¸öÊý
#define configUSE_APPLICATION_TASK_TAG 0

#define configSUPPORT_DYNAMIC_ALLOCATION 1 // Ö§³Ö¶¯Ì¬ÄÚ´æÉêÇë
#define configUSE_MALLOC_FAILED_HOOK 0     // Ê¹ÓÃÄÚ´æÉêÇëÊ§°Ü¹³×Óº¯Êý
#define configCHECK_FOR_STACK_OVERFLOW 2   // 大于 0 时启用堆栈溢出检测功能，
                                           // 如果使用此功能用户必须提供一个栈溢出钩子函数
                                         // 如果使用的话此值可以为 1 或者 2，因
                                         // 为有两种栈溢出检测方法

#define configGENERATE_RUN_TIME_STATS 1 // ÆôÓÃÔËÐÐÊ±¼äÍ³¼Æ¹¦ÄÜ
#define configUSE_STATS_FORMATTING_FUNCTIONS 1

extern volatile uint32_t CPU_RunTime;
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() (CPU_RunTime = 0ul)
#define portGET_RUN_TIME_COUNTER_VALUE() CPU_RunTime

#define configUSE_TIMERS 1                                          // ÆôÓÃÈí¼þ¶¨Ê±Æ÷
#define configTIMER_TASK_PRIORITY (configMAX_PRIORITIES - 1)        // Èí¼þ¶¨Ê±Æ÷ÓÅÏÈ¼¶
#define configTIMER_QUEUE_LENGTH 10                                 // Èí¼þ¶¨Ê±Æ÷¶ÓÁÐ³¤¶È
#define configTIMER_TASK_STACK_DEPTH (configMINIMAL_STACK_SIZE * 2) // Èí¼þ¶¨Ê±
                                                                    // Æ÷ÈÎÎñ¶ÑÕ»´óÐ¡

// ¿ÉÑ¡º¯ÊýÅäÖÃÑ¡Ïî
#define INCLUDE_xTaskGetSchedulerState 1
#define INCLUDE_eTaskGetState 1
#define INCLUDE_xTimerPendFunctionCall 1

// ÖÐ¶Ï·þÎñº¯Êý Ò²¿ÉÒÔÐÞ¸ÄÆðÊ¼ÎÄ¼þ
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

/*Ô´ÎÄ¼þ×Ô´øºê¶¨Òå ¿ÉÒÔÐÞ¸Ä*/
#define configUSE_PREEMPTION 1                       // ÖÃ 1£º RTOS Ê¹ÓÃÇÀÕ¼Ê½µ÷¶ÈÆ÷£»ÖÃ 0£ºRTOS Ê¹ÓÃÐ­×÷Ê½µ÷¶ÈÆ÷£¨Ê±¼äÆ¬£©
#define configUSE_IDLE_HOOK 0                        // ÖÃ 1£ºÊ¹ÓÃ¿ÕÏÐ¹³×Ó£¨Idle Hook ÀàËÆÓÚ»Øµ÷º¯Êý£©£»ÖÃ 0£ººöÂÔ¿ÕÏÐ¹³×Ó
#define configUSE_TICK_HOOK 0                        // ÖÃ 1£ºÊ¹ÓÃÊ±¼äÆ¬¹³×Ó£¨Tick Hook£©£»ÖÃ 0£ººöÂÔÊ±¼äÆ¬¹³×Ó
#define configCPU_CLOCK_HZ ((unsigned long)72000000) // Ð´Èë¼ÊµÄ
                                                     // CPU ÄÚºËÊ±ÖÓÆµÂÊ£¬Ò²¾ÍÊÇ CPU Ö¸ÁîÖ´ÐÐÆµÂÊ

#define configTICK_RATE_HZ ((TickType_t)1000)          // RTOSÏµÍ³½ÚÅÄÖÐ¶ÏµÄÆµÂÊ¡£¼´Ò»ÃëÖÐ¶ÏµÄ´ÎÊý£¬Ã¿´ÎÖÐ¶Ï RTOS ¶¼»á½øÐÐÈÎÎñµ÷¶È
#define configMAX_PRIORITIES (5)                       // ¿ÉÊ¹ÓÃµÄ×î´óÓÅÏÈ¼¶
#define configMINIMAL_STACK_SIZE ((unsigned short)128) // ¿ÕÏÐÈÎÎñÊ¹ÓÃµÄ¶ÑÕ»´óÐ¡
#define configTOTAL_HEAP_SIZE ((size_t)(17 * 1024))    // ÏµÍ³ËùÓÐ×ÜµÄ¶Ñ´óÐ¡

#define configMAX_TASK_NAME_LEN (16) // ÈÎÎñÃû×Ö×Ö·û´®³¤¶È
#define configUSE_TRACE_FACILITY 1   // ÆôÓÃ¿ÉÊÓ»¯¸ú×Ùµ÷ÊÔ
#define configUSE_16_BIT_TICKS 0     // ÏµÍ³½ÚÅÄ¼ÆÊýÆ÷±äÁ¿Êý¾ÝÀàÐÍ£¬ 1 ±íÊ¾Îª
                                 // 16 Î»ÎÞ·ûºÅÕûÐÎ£¬ 0 ±íÊ¾Îª 32 Î»ÎÞ·ûºÅÕûÐÎ
#define configIDLE_SHOULD_YIELD 1 // ¿ÕÏÐÈÎÎñ·ÅÆú CPU Ê¹ÓÃÈ¨¸øÆäËûÍ¬ÓÅÏÈ¼¶µÄÓÃ»§ÈÎÎñ

/*Ð­³Ì*/
#define configUSE_CO_ROUTINES 0             // ÆôÓÃÐ­³Ì£¬ÆôÓÃÐ­³ÌÒÔºó±ØÐëÌí¼ÓÎÄ¼þ croutine.c
#define configMAX_CO_ROUTINE_PRIORITIES (2) // Ð­³ÌµÄÓÐÐ§ÓÅÏÈ¼¶ÊýÄ¿

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet 1
#define INCLUDE_uxTaskPriorityGet 1
#define INCLUDE_vTaskDelete 1
#define INCLUDE_vTaskCleanUpResources 1
#define INCLUDE_vTaskSuspend 1
#define INCLUDE_vTaskDelayUntil 1
#define INCLUDE_vTaskDelay 1

/* This is the raw value as per the Cortex-M3 NVIC. Values can be 255
(lowest) to 0 (1?) (highest). */
#define configKERNEL_INTERRUPT_PRIORITY 255
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 191 /* equivalent to 0xb0, or priority \
11. */

/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15. This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting. Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY 15 // ÖÐ¶Ï×îµÍÓÅÏÈ
#endif                                             /* FREERTOS_CONFIG_H */
