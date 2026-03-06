#include "led.h"

// Led初始化
void Led_Init(void)
{

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure = {0};
  GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN | LED2_GPIO_PIN | LED3_GPIO_PIN | LED4_GPIO_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
  GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
  GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
  GPIO_SetBits(LED4_GPIO_PORT, LED4_GPIO_PIN);
}
