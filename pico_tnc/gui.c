#include "pico/stdlib.h"
#include "tnc.h"

#ifdef ENABLE_GUI

struct menu_entry {
	struct menu_entry *(*func)(struct menu_entry *);
	void *args;
};

struct menu_entry *
menu_parameter(struct menu_entry *entry)
{
	return NULL;
}

struct menu_entry root_menu[] = {
	{menu_parameter, "Mycall,Text"},
	{menu_parameter, "Btext,Text"},
	{menu_parameter, "Beacon,Num,1-60"},
};

void
gui_init(void)
{
}

bool
cmd_gui(tty_t *ttyp, uint8_t *buf, int len)
{
        return true;
}

#endif
