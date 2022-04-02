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
	return true;
}

#endif
