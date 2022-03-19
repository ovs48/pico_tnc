#include "pico/stdlib.h"
#include "tnc.h"
#include "tty.h"

#ifdef ENABLE_KEYPAD

static char rows[]={GPIO_ROW1,GPIO_ROW2,GPIO_ROW3,GPIO_ROW4};
static char cols[]={GPIO_COL1,GPIO_COL2,GPIO_COL3,GPIO_COL4};


void
keypad_init(void)
{
	int i=0;
	for (i = 0 ; i < sizeof(rows)/sizeof(rows[0]); i++) {
		char gpio=rows[i];
		gpio_init(gpio);
		gpio_set_dir(gpio, true);
		gpio_put(gpio, 1);
	}
	for (i = 0 ; i < sizeof(cols)/sizeof(cols[0]); i++) {
		char gpio=cols[i];
		gpio_init(gpio);
		gpio_pull_down(gpio);

	}
}

static int
get_cols(void)
{
	int i,ret=0;
	for (i = 0 ; i < sizeof(cols)/sizeof(cols[0]); i++) {
		if (gpio_get(cols[i]))
			ret|=(1<<i);
	}
	return ret;
}

bool
cmd_keypad(tty_t *ttyp, uint8_t *buf, int len)
{
	int i;
	tty_write_str(ttyp, "All active cols: ");
	tty_write_byte(ttyp, get_cols());
	tty_write_str(ttyp, "\r\n");
	for (i = 0 ; i < sizeof(rows)/sizeof(rows[0]); i++) {
		gpio_put(rows[i], 0);
	}
	for (i = 0 ; i < sizeof(rows)/sizeof(rows[0]); i++) {
		gpio_put(rows[i], 1);
		tty_write_str(ttyp, "Active col in row ");
		tty_write_byte(ttyp, i);
		tty_write_str(ttyp,":");
		tty_write_byte(ttyp, get_cols());
		tty_write_str(ttyp, "\r\n");
		gpio_put(rows[i], 0);
	}
	for (i = 0 ; i < sizeof(rows)/sizeof(rows[0]); i++) {
		gpio_put(rows[i], 1);
	}
	return true;
}

#endif