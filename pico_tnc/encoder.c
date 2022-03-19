#include "pico/stdlib.h"
#include "tnc.h"

#ifdef ENABLE_ENCODER

void
encoder_init(void)
{
}

bool
cmd_encoder(tty_t *ttyp, uint8_t *buf, int len)
{
        return true;
}

#endif
