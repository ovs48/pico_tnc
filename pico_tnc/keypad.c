#include "pico/stdlib.h"
#include "tnc.h"

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
		gpio_init(cols[i]);
	}
}

#endif
