/**
 * Copyright (c) 2021 JN1DFF
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once
#include "pico/util/queue.h"

#include "filter.h"
#include "ax25.h"
//#include "cmd.h"

// number of ports
#define PORT_N 1    // number of ports, 1..3

#define BAUD_RATE 1200
#define SAMPLING_N 11
//#define DELAY_N 3
//#define SAMPLING_RATE ((1000000*DELAY_N+DELAY_US/2)/DELAY_US)
#define SAMPLING_RATE (BAUD_RATE * SAMPLING_N)
#define DELAY_US 446 // 446us
#define DELAYED_N ((SAMPLING_RATE * DELAY_US + 500000) / 1000000)

#define ADC_SAMPLING_RATE (SAMPLING_RATE * PORT_N)

#define DATA_LEN 1024                   // packet receive buffer size

#define FIR_LPF_N 27
#define FIR_BPF_N 25

#define ADC_BIT 8       // adc bits 8 or 12
//#define ADC_BIT 12      // adc bits 8 or 12

//#define BELL202_SYNC 1  // sync decode
#define DECODE_PLL 1    // use PLL

#define CONTROL_N 10
#define DAC_QUEUE_LEN 64
#define DAC_BLOCK_LEN (DAC_QUEUE_LEN + 1)

#define SEND_QUEUE_LEN (1024 * 16)

#define AX25_FLAG 0x7e

#define BUSY_PIN 22

#define BEACON_PORT 0

#define KISS_PACKET_LEN 1024                // kiss packet length
#define TTY_N 3                             // number of serial
#define CMD_BUF_LEN 255

#include "config.h"

enum STATE {
	FLAG,
	DATA
};

typedef struct {
  int low_i;
  int low_q;
  int high_i;
  int high_q;
} values_t;

typedef struct TTY tty_t;

struct TNC_data {
    uint16_t data_cnt;
    uint8_t data[DATA_LEN];
};

typedef struct TNC {
    uint8_t port;

    // receive

    // demodulator
    uint8_t bit;

    // decode_bit
    struct TNC_data pdata;
    uint8_t state;
    uint8_t flag;
    uint8_t data_byte;
    uint8_t data_bit_cnt;

    // decode
    uint8_t edge;
    
    // decode2
    int32_t pll_counter;
    uint8_t pval;
    uint8_t nrzi;

    // output_packet
    int pkt_cnt;

    // bell202_decode
    int delayed[DELAYED_N];
    int delay_idx;
    int cdt;
    int cdt_lvl;
    int avg;
    uint8_t cdt_pin;

    // bell202_decode2
    int sum_low_i;
    int sum_low_q;
    int sum_high_i;
    int sum_high_q;
    int low_idx;
    int high_idx;
    values_t values[SAMPLING_N];
    int values_idx;
    filter_t lpf;
    filter_t bpf;

    // send

    // kiss parameter
    uint8_t kiss_txdelay;
    uint8_t kiss_p;
    uint8_t kiss_slottime;
    uint8_t kiss_fullduplex;

    // dac queue
    queue_t dac_queue;

    // DAC, PTT pin
    uint8_t ptt_pin;
    uint8_t ptt_active;
    uint8_t pwm_pin;
    uint8_t pwm_slice;

    // DMA channels
    uint8_t ctrl_chan;
    uint8_t data_chan;
    uint32_t data_chan_mask;
    uint8_t busy;

    // Bell202 wave generator
    int next;
    int phase;
    int level;
    int cnt_one;

    // wave buffer for DMA
    uint32_t const *dma_blocks[DAC_BLOCK_LEN][CONTROL_N + 1];

    // send data queue
    queue_t send_queue;
    int send_time;
    int send_len;
    int send_state;
    int send_data;

    // field for test packet
    int test_state;
    uint8_t const *ptp;
    uint8_t const *packet;
    uint16_t packet_len;
    int wait_time;

    // calibrate
    uint8_t cal_data;
    bool do_nrzi;
    uint32_t cal_time;
    tty_t *ttyp;

} tnc_t;

extern tnc_t tnc[];
extern uint32_t __tnc_time;

void tnc_init(void);

inline uint32_t tnc_time(void)
{
    return __tnc_time;
}

// TNC command
enum MONITOR {
    MON_ALL = 0,
    MON_ME,
    MON_OFF,
};

// GPS
enum GPS_SENTENCE {
    GPGGA = 0,
    GPGLL,
    GPRMC,
};

#define UNPROTO_N 4
#define BTEXT_LEN 100


// TNC parameter
typedef struct TNC_PARAM {
    callsign_t mycall;
    callsign_t myalias;
    callsign_t unproto[UNPROTO_N];
    uint8_t btext[BTEXT_LEN + 1];
    uint8_t txdelay;
    uint8_t gps;
    uint8_t mon;
    uint8_t digi;
    uint8_t beacon;
    uint8_t trace;
    uint8_t echo;
} param_t;

extern param_t param;

// tty

enum TTY_MODE {
    TTY_TERMINAL = 0,
    TTY_GPS,
    TTY_TRANSCEIVER,
    TTY_GUI,
};

enum TTY_SERIAL {
    TTY_USB = 0,
    TTY_UART0,
    TTY_UART1,
    TTY_DISPLAY,
};

typedef struct TTY {
    uint8_t kiss_buf[KISS_PACKET_LEN];
    uint8_t cmd_buf[CMD_BUF_LEN + 1];
    int kiss_idx;
    int cmd_idx;

    uint8_t num;        // index of tty[]

    uint8_t tty_mode;   // terminal or GPS
    uint8_t tty_serial; // USB, UART0, UART1

    uint8_t kiss_mode;  // kiss mode
    uint8_t kiss_state; // kiss state
    uint32_t kiss_timeout; // kiss timer

    tnc_t *tp;          // input/output port No.
} tty_t;

extern tty_t tty[];

// send process state
enum SEND_STATE {
    SP_IDLE = 0,
    SP_WAIT_CLR_CH,
    SP_P_PERSISTENCE,
    SP_WAIT_SLOTTIME,
    SP_PTT_ON,
    SP_SEND_FLAGS,
    SP_DATA_START,
    SP_DATA,
    SP_ERROR,
    SP_CALIBRATE,
    SP_CALIBRATE_OFF,
};

void debug_printf(const char *fmt, ...);

enum DISPLAY_FLAGS {
    DISPLAY_FLAGS_SRC=1,
    DISPLAY_FLAGS_DST=2,
    DISPLAY_FLAGS_ROUTE=4,
    DISPLAY_FLAGS_METADATA=8,
    DISPLAY_FLAGS_DATA=16,
    DISPLAY_FLAGS_ALL=255
};

void display_packet_do(tty_t *ttyp, tnc_t *tp, struct TNC_data *pkt, enum DISPLAY_FLAGS flags);

#ifdef ENABLE_KEYPAD
enum keypad_mode {
	KEYPAD_MODE_TEXT_UPPERCASE,
	KEYPAD_MODE_TEXT,
	KEYPAD_MODE_AZ09MINUS,
	KEYPAD_MODE_SIMPLE,
};

extern void keypad_init(void);
void keypad_poll(void);
void keypad_set_mode(enum keypad_mode mode);
bool cmd_keypad(tty_t *ttyp, uint8_t *buf, int len);
#endif

#ifdef ENABLE_DISPLAY
extern void display_init(void);
extern void display_write(uint8_t const *data, int len);
extern void display_update(void);
extern bool cmd_display(tty_t *ttyp, uint8_t *buf, int len);
extern tty_t display_tty;
#endif

#ifdef ENABLE_ENCODER
extern void encoder_init(void);
bool cmd_encoder(tty_t *ttyp, uint8_t *buf, int len);
#endif

#ifdef ENABLE_TRANSCEIVER
extern void transceiver_init(void);
extern void transceiver_input(void);
extern bool cmd_transceiver(tty_t *ttyp, uint8_t *buf, int len);
#endif

#ifdef ENABLE_GUI
extern void gui_init(void);
extern bool cmd_gui(tty_t *ttyp, uint8_t *buf, int len);
extern void gui_process_char(char c, tty_t *ttyp);
extern void gui_display_packet(tnc_t *tp);
#endif

extern enum gps_debug {
	GPS_DEBUG_RAW=1,
	GPS_DEBUG_GPRMC=2,
	GPS_DEBUG_LATLON=4,
	GPS_DEBUG_REPORT=8,
} gps_debug;

extern enum receive_debug {
	RECEIVE_DEBUG_ADC=1,
	RECEIVE_DEBUG_CDT=2,
} receive_debug;

extern const char *gps_getvar(const char *var);
extern const char *substitute_vars(const char *str);
