#include <string.h>
#include "pico/stdlib.h"
#include "tnc.h"
#include "serial.h"
#include "tty.h"
#include "usb_output.h"

#ifdef ENABLE_TRANSCEIVER

static void
transceiver_command(const char *cmd)
{
	serial_write(cmd,strlen(cmd));
	usb_write(cmd, strlen(cmd));
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
	if (init < 4000) {
		init++;
		if (init == 400) {
			transceiver_command("AT+DMOCONNECT\r\n");
		}
		if (init == 2900) {
			transceiver_command("AT+DMOSETGROUP=1,144.8000,144.8000,0000,0,0000\r\n");
		}
		if (init == 3400) {
			transceiver_command("AT+DMOSETVOLUME=1\r\n");
		}
		if (init == 3900) {
			transceiver_command("AT+SETFILTER=0,1,1\r\n");
		}
	}
	while (uart_is_readable(uart0)) {
		int ch = uart_getc(uart0);
		usb_write_char(ch);
    	}
}


bool
cmd_transceiver(tty_t *ttyp, uint8_t *buf, int len)
{
	tty_write_str(ttyp, "Transceiver test ");
	tty_write(ttyp, buf, len);
	tty_write_str(ttyp, "\r\n");
	if (len == 1 && buf[0] == '1') {
		transceiver_command("AT+DMOCONNECT\r\n");
	}
	if (len == 1 && buf[0] == '2') {
		transceiver_command("AT+DMOSETGROUP=0,144.8000,144.8000,0000,0,0000\r\n");
	}
	if (len == 1 && buf[0] == '3') {
		gpio_put(GPIO_PTT1, 0);
	}
	if (len == 1 && buf[0] == '4') {
		gpio_put(GPIO_PTT1, 1);
	}
	if (len == 1 && buf[0] == '5') {
		transceiver_command("AT+DMOSETVOLUME=8\r\n");
	}
	if (len == 1 && buf[0] == '6') {
		init=0;
	}
	return true;
}

#endif
