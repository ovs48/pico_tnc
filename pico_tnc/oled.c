#include "pico/stdlib.h"
#include "tnc.h"

#ifdef ENABLE_OLED

void
oled_init(void)
{
}

bool
cmd_oled(tty_t *ttyp, uint8_t *buf, int len)
{
	return true;
}

#endif
