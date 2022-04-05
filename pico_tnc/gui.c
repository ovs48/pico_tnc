#include <string.h>
#include "pico/stdlib.h"
#include "tnc.h"
#include "tty.h"

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
	{menu_parameter, "menu settings"},
	{NULL, NULL},
};

/* Transmit */
/* Settings */

/* APRS MYCALL, MYALIAS */
/* Beacon GPS, BTEXT, BEACON */
/* RX MONITOR, DIGIPEAT */
/* TX TXDELAY, UNPROTO */

struct menu_entry settings_menu[] = {
	{menu_parameter, "TXDELAY"},
	{menu_parameter, "GPS"},
	{menu_parameter, "MONITOR"},
	{menu_parameter, "DIGIPEAT"},
	{menu_parameter, "BEACON"},
	{menu_parameter, "UNPROTO"},
	{menu_parameter, "MYCALL"},
	{menu_parameter, "MYALIAS"},
	{menu_parameter, "BTEXT"},
	{NULL, NULL},
};

void
gui_init(void)
{
}

bool
cmd_gui(tty_t *ttyp, uint8_t *buf, int len)
{
	struct menu_entry *e=root_menu;
	if (len == 8 && !strncmp(buf, "settings",len)) {
		e=settings_menu;
	}
	int i=1;
	while(e->func) {
		char buffer[64];
		snprintf(buffer,sizeof(buffer), "%d ",i++);
		tty_write(ttyp, buffer, strlen(buffer));
		snprintf(buffer,sizeof(buffer),"%s query",(char *)e->args);
		cmd_do(ttyp, buffer, strlen(buffer), 1);
		e++;
	}
        return true;
}

#endif
