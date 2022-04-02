#include <string.h>
#include "pico/stdlib.h"
#include "tnc.h"
#include "serial.h"
#include "tty.h"

#ifdef ENABLE_TRANSCEIVER

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
	char *cmd=NULL;
	if (len == 1 && buf[0] == '1') {
		cmd="AT+DMOCONNECT\r\n";
	}
	if (len == 1 && buf[0] == '2') {
		cmd="AT+DMOSETGROUP=0,145.5250,145.5250,0000,0,0000\r\n";
	}
	if (len == 1 && buf[0] == '3') {
		gpio_put(GPIO_PTT1, 0);
	}
	if (len == 1 && buf[0] == '4') {
		gpio_put(GPIO_PTT1, 1);
	}
	tty_write_str(ttyp, "Transceiver test\r\n");
	tty_write(ttyp, buf, len);
	if (cmd) {
		serial_write(cmd,strlen(cmd));
		tty_write(ttyp, cmd, strlen(cmd));
	}
	return true;
}

#endif
