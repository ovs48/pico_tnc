#include "pico/stdlib.h"
#include "tnc.h"

#ifdef ENABLE_TRANSCEIVER

void
transceiver_init(void)
{
}

bool
cmd_transceiver(tty_t *ttyp, uint8_t *buf, int len)
{
	return true;
}

#endif
