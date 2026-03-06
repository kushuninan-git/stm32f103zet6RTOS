#include "jx_uart_send.h"
#include "stdio.h"
#include "su_03t.h" // 引入UART5_Tx_Buff声明

// 串口通信消息头
const unsigned char g_uart_send_head[] = {
    0xaa, 0x55};

// 串口通信消息尾
const unsigned char g_uart_send_foot[] = {
    0x55, 0xaa};
// 十六位整数转32位整数
void _int16_to_int32(uart_param_t *param)
{
  if (sizeof(int) >= 4)
    return;
  unsigned long value = param->d_long;
  unsigned long sign = (value >> 15) & 1;
  unsigned long v = value;
  if (sign)
    v = 0xFFFF0000 | value;
  uart_param_t p;
  p.d_long = v;
  param->d_ucs[0] = p.d_ucs[0];
  param->d_ucs[1] = p.d_ucs[1];
  param->d_ucs[2] = p.d_ucs[2];
  param->d_ucs[3] = p.d_ucs[3];
}

// 浮点数转双精度
void _float_to_double(uart_param_t *param)
{
  if (sizeof(int) >= 4)
    return;
  unsigned long value = param->d_long;
  unsigned long sign = value >> 31;
  unsigned long M = value & 0x007FFFFF;
  unsigned long e = ((value >> 23) & 0xFF) - 127 + 1023;
  uart_param_t p0, p1;
  p1.d_long = ((sign & 1) << 31) | ((e & 0x7FF) << 20) | (M >> 3);
  p0.d_long = (M & 0x7) << 29;
  param->d_ucs[0] = p0.d_ucs[0];
  param->d_ucs[1] = p0.d_ucs[1];
  param->d_ucs[2] = p0.d_ucs[2];
  param->d_ucs[3] = p0.d_ucs[3];
  param->d_ucs[4] = p1.d_ucs[0];
  param->d_ucs[5] = p1.d_ucs[1];
  param->d_ucs[6] = p1.d_ucs[2];
  param->d_ucs[7] = p1.d_ucs[3];
}
// 串口发送函数实现
void _uart_send_impl(unsigned char *buff, int len)
{
  // TODO: 调用项目实际的串口发送函数
  Su03t_SendArray(buff, len);

  /*
  int i = 0;
  unsigned char c;
  for (i = 0; i < len; i++) {
    c = buff[i];
    printf("%02X ", c);
  }
  printf("\n");
  */
}

// action: co2_2
void _uart_co2_2(double co2)
{
  uart_param_t param;
  int i = 0;
  unsigned char buff[UART_SEND_MAX] = {0};
  for (i = 0; i < UART_MSG_HEAD_LEN; i++)
  {
    buff[i + 0] = g_uart_send_head[i];
  }
  buff[2] = U_MSG_co2_2;
  param.d_double = co2;
  _float_to_double(&param);
  buff[3] = param.d_ucs[0];
  buff[4] = param.d_ucs[1];
  buff[5] = param.d_ucs[2];
  buff[6] = param.d_ucs[3];
  buff[7] = param.d_ucs[4];
  buff[8] = param.d_ucs[5];
  buff[9] = param.d_ucs[6];
  buff[10] = param.d_ucs[7];
  for (i = 0; i < UART_MSG_FOOT_LEN; i++)
  {
    buff[i + 11] = g_uart_send_foot[i];
  }
  _uart_send_impl(buff, 13);
}

// action: ch2o_2
void _uart_ch2o_2(double ch2o)
{
  uart_param_t param;
  int i = 0;
  unsigned char buff[UART_SEND_MAX] = {0};
  for (i = 0; i < UART_MSG_HEAD_LEN; i++)
  {
    buff[i + 0] = g_uart_send_head[i];
  }
  buff[2] = U_MSG_ch2o_2;
  param.d_double = ch2o;
  _float_to_double(&param);
  buff[3] = param.d_ucs[0];
  buff[4] = param.d_ucs[1];
  buff[5] = param.d_ucs[2];
  buff[6] = param.d_ucs[3];
  buff[7] = param.d_ucs[4];
  buff[8] = param.d_ucs[5];
  buff[9] = param.d_ucs[6];
  buff[10] = param.d_ucs[7];
  for (i = 0; i < UART_MSG_FOOT_LEN; i++)
  {
    buff[i + 11] = g_uart_send_foot[i];
  }
  _uart_send_impl(buff, 13);
}

// action: voc_2
void _uart_voc_2(double voc)
{
  uart_param_t param;
  int i = 0;
  unsigned char buff[UART_SEND_MAX] = {0};
  for (i = 0; i < UART_MSG_HEAD_LEN; i++)
  {
    buff[i + 0] = g_uart_send_head[i];
  }
  buff[2] = U_MSG_voc_2;
  param.d_double = voc;
  _float_to_double(&param);
  buff[3] = param.d_ucs[0];
  buff[4] = param.d_ucs[1];
  buff[5] = param.d_ucs[2];
  buff[6] = param.d_ucs[3];
  buff[7] = param.d_ucs[4];
  buff[8] = param.d_ucs[5];
  buff[9] = param.d_ucs[6];
  buff[10] = param.d_ucs[7];
  for (i = 0; i < UART_MSG_FOOT_LEN; i++)
  {
    buff[i + 11] = g_uart_send_foot[i];
  }
  _uart_send_impl(buff, 13);
}

// action: hum_2
void _uart_hum_2(double hum)
{
  uart_param_t param;
  int i = 0;
  unsigned char buff[UART_SEND_MAX] = {0};
  for (i = 0; i < UART_MSG_HEAD_LEN; i++)
  {
    buff[i + 0] = g_uart_send_head[i];
  }
  buff[2] = U_MSG_hum_2;
  param.d_double = hum;
  _float_to_double(&param);
  buff[3] = param.d_ucs[0];
  buff[4] = param.d_ucs[1];
  buff[5] = param.d_ucs[2];
  buff[6] = param.d_ucs[3];
  buff[7] = param.d_ucs[4];
  buff[8] = param.d_ucs[5];
  buff[9] = param.d_ucs[6];
  buff[10] = param.d_ucs[7];
  for (i = 0; i < UART_MSG_FOOT_LEN; i++)
  {
    buff[i + 11] = g_uart_send_foot[i];
  }
  _uart_send_impl(buff, 13);
}

// action: tem_2
void _uart_tem_2(double tem)
{
  uart_param_t param;
  int i = 0;
  unsigned char buff[UART_SEND_MAX] = {0};
  for (i = 0; i < UART_MSG_HEAD_LEN; i++)
  {
    buff[i + 0] = g_uart_send_head[i];
  }
  buff[2] = U_MSG_tem_2;
  param.d_double = tem;
  _float_to_double(&param);
  buff[3] = param.d_ucs[0];
  buff[4] = param.d_ucs[1];
  buff[5] = param.d_ucs[2];
  buff[6] = param.d_ucs[3];
  buff[7] = param.d_ucs[4];
  buff[8] = param.d_ucs[5];
  buff[9] = param.d_ucs[6];
  buff[10] = param.d_ucs[7];
  for (i = 0; i < UART_MSG_FOOT_LEN; i++)
  {
    buff[i + 11] = g_uart_send_foot[i];
  }
  _uart_send_impl(buff, 13);
}
