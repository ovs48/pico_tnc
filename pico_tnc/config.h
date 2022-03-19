#define OVS48 1
#ifdef OVS48
#define GPIO_UART0_TX 0
#define GPIO_UART0_RX 1
#define GPIO_PWM1 2
#define GPIO_SQ 3
#define GPIO_PTT1 4
#define GPIO_PD 5
#define GPIO_PTT0 6
#define GPIO_PWM0 7
#define GPIO_UART1_TX 8
#define GPIO_UART1_RX 9
#define GPIO_LED 10
#define GPIO_ENCODER_SW 11
#define GPIO_I2C0_SDA 12
#define GPIO_I2C0_SCL 13
#define GPIO_ENCODER_DT 14
#define GPIO_ENCODER_CLK 15
#define GPIO_ROW4 16
#define GPIO_ROW3 17
#define GPIO_ROW2 18
#define GPIO_ROW1 19
#define GPIO_COL4 20
#define GPIO_COL3 21
#define GPIO_COL2 22
#define GPIO_ADC0 26
#define GPIO_ADC1 27
#define GPIO_COL1 28

#define GPIO_CDT0 GPIO_LED
#define GPIO_CDT1 GPIO_LED
#define GPIO_CDT2 GPIO_LED

#define GPIO_PWM2 GPIO_PWM1
#define GPIO_PTT2 GPIO_PTT1

#define ENABLE_KEYPAD 1
#define ENABLE_OLED 1
#define ENABLE_ENCODER 1
#define ENABLE_TRANSCEIVER 1

#else
#define GPIO_UART0_TX 0
#define GPIO_UART0_RX 1
#define GPIO_PWM2 6
#define GPIO_PTT2 7

#define GPIO_UART1_TX 8
#define GPIO_PWM1 8

#define GPIO_UART1_RX 9
#define GPIO_PTT1 9
#define GPIO_PWM0 10
#define GPIO_PTT0 11

#define GPIO_CDT0 20
#define GPIO_CDT1 21
#define GPIO_CDT2 22
#endif
