#ifndef JX_UART_SEND_H_
#define JX_UART_SEND_H_

#ifdef __cplusplus
extern "C" {
#endif

// 串口发送消息最大长度
#define UART_SEND_MAX      32
#define UART_MSG_HEAD_LEN  2
#define UART_MSG_FOOT_LEN  2

// 串口发送消息号
#define U_MSG_co2_2      1
#define U_MSG_ch2o_2      2
#define U_MSG_voc_2      3
#define U_MSG_hum_2      4
#define U_MSG_tem_2      5

// 串口消息参数类型
typedef union {
  double d_double;
  int d_int;
  unsigned char d_ucs[8];
  char d_char;
  unsigned char d_uchar;
  unsigned long d_long;
  short d_short;
  float d_float;
}uart_param_t;



// action: co2_2
void _uart_co2_2(double co2);

// action: ch2o_2
void _uart_ch2o_2(double ch2o);

// action: voc_2
void _uart_voc_2(double voc);

// action: hum_2
void _uart_hum_2(double hum);

// action: tem_2
void _uart_tem_2(double tem);

#ifdef __cplusplus
}
#endif

#endif /*JX_UART_SEND_H_*/

