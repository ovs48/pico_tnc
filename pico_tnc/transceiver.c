#include <string.h>
#include "pico/stdlib.h"
#include "tnc.h"
#include "serial.h"
#include "tty.h"
#include "usb_output.h"

#ifdef ENABLE_TRANSCEIVER

uint32_t cmd_start;
uint32_t transceiver_timeout=10;
const char *expected_result;
int result_pos;

enum {
	TRANSCEIVER_INIT_STATE_BOOT,
	TRANSCEIVER_INIT_STATE_DMOCONNECT,
	TRANSCEIVER_INIT_STATE_DMOSETGROUP,
	TRANSCEIVER_INIT_STATE_DMOSETVOLUME,
	TRANSCEIVER_INIT_STATE_SETFILTER,
	TRANSCEIVER_INIT_STATE_IDLE,
} transceiver_init_state;

static void
transceiver_command(const char *cmd, const char *result, uint32_t timeout)
{
	serial_write(cmd,strlen(cmd));
	usb_write(cmd, strlen(cmd));
	cmd_start=tnc_time();
	expected_result=result;
	transceiver_timeout=timeout;
}

static void
transceiver_command_timeout(void)
{
	// debug_printf("timeout\r\n");
	switch (transceiver_init_state) {
	case TRANSCEIVER_INIT_STATE_BOOT:
		transceiver_init_state=TRANSCEIVER_INIT_STATE_DMOCONNECT;
		transceiver_command("AT+DMOCONNECT\r\n","+DMOCONNECT:0\r\n",50);
	default:
		transceiver_timeout=0;
	}
}

static void
transceiver_command_complete(void)
{
	// debug_printf("complete\r\n");
	switch (transceiver_init_state) {
	case TRANSCEIVER_INIT_STATE_DMOCONNECT:
		transceiver_init_state=TRANSCEIVER_INIT_STATE_DMOSETGROUP;
		transceiver_command("AT+DMOSETGROUP=1,144.8000,144.8000,0000,0,0000\r\n","+DMOSETGROUP:0\r\n",10);
		break;
	case TRANSCEIVER_INIT_STATE_DMOSETGROUP:
		transceiver_init_state=TRANSCEIVER_INIT_STATE_DMOSETVOLUME;
		transceiver_command("AT+DMOSETVOLUME=3\r\n","+DMOSETVOLUME:0\r\n",10);
		break;
	case TRANSCEIVER_INIT_STATE_DMOSETVOLUME:
		transceiver_init_state=TRANSCEIVER_INIT_STATE_SETFILTER;
		transceiver_command("AT+SETFILTER=0,1,1\r\n","+DMOSETFILTER:0\r\n",10);
		break;
	case TRANSCEIVER_INIT_STATE_SETFILTER:
		transceiver_init_state=TRANSCEIVER_INIT_STATE_IDLE;
                transceiver_timeout=0;
		break;
	}
}


void
transceiver_init(void)
{
#if 1
	gpio_init(GPIO_PD);
	gpio_set_dir(GPIO_PD, true); // output
	gpio_put(GPIO_PD, 1);
	gpio_init(GPIO_PTT1);
	gpio_set_dir(GPIO_PTT1, true); // output
	gpio_put(GPIO_PTT1, 1);
#endif
}

static int init;

void
transceiver_input(void)
{
	while (uart_is_readable(uart0)) {
		int ch = uart_getc(uart0);
		// usb_write_char(ch);
		if (ch == expected_result[result_pos++]) {
			if (!expected_result[result_pos])
				transceiver_command_complete();
		} else {
			result_pos=0;
			if (ch == expected_result[result_pos])
				result_pos++;
		}
    	}
	if (transceiver_timeout && tnc_time() - cmd_start >= transceiver_timeout) {
		transceiver_command_timeout();
	}
}


bool
cmd_transceiver(tty_t *ttyp, uint8_t *buf, int len)
{
	tty_write_str(ttyp, "Transceiver test ");
	tty_write(ttyp, buf, len);
	tty_write_str(ttyp, "\r\n");
	if (len == 1 && buf[0] == '1') {
		transceiver_command("AT+DMOCONNECT\r\n","",50);
	}
	if (len == 1 && buf[0] == '2') {
		transceiver_command("AT+DMOSETGROUP=0,144.8000,144.8000,0000,0,0000\r\n","",10);
	}
	if (len == 1 && buf[0] == '3') {
		gpio_put(GPIO_PTT1, 0);
	}
	if (len == 1 && buf[0] == '4') {
		gpio_put(GPIO_PTT1, 1);
	}
	if (len == 1 && buf[0] == '5') {
		transceiver_command("AT+DMOSETVOLUME=8\r\n","",10);
	}
	if (len == 1 && buf[0] == '6') {
		init=0;
	}
	return true;
}

#endif
