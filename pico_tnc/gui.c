#include <string.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "tnc.h"
#include "tty.h"

#ifdef ENABLE_GUI

static char buffer[64];
static char msg_buffer[64*10];

struct menu_entry {
	struct menu_entry *(*func)(struct menu_entry *, tty_t *, char *);
	void *args;
};

struct menus {
	char *name;
	struct menu_entry *entries;
};

static int menu_i,cursor;
struct menu_entry *selection[11];
static void menu_display(char *name, tty_t *ttyp);

static void
menu_display_entries(struct menu_entry *e, tty_t *ttyp)
{
	menu_i=1;
	memset(selection, 0, sizeof(selection));
	tty_write(ttyp, "\eE",2);
	while(e->func) {
		e->func(e, ttyp, NULL);
		e++;
	}
	selection[10]=e;
}

static void
menu_idx(struct menu_entry *e, tty_t *ttyp)
{
	snprintf(buffer,sizeof(buffer), "%d ",menu_i);
	if (menu_i >= 1 && menu_i <= 10) {
		selection[menu_i-1]=e;
	}
	tty_write(ttyp, buffer, strlen(buffer));
	menu_i++;
}

static struct menu_entry *
menu_title(struct menu_entry *e, tty_t *ttyp, char *mode)
{
	char *txt=e->args;
	tty_write(ttyp, txt, strlen(txt));
	tty_write(ttyp, "\r\n", 2);
}

static struct menu_entry *
menu_gui(struct menu_entry *e, tty_t *ttyp, char *mode)
{
	char *txt=e->args;
	if (!mode) {
		menu_idx(e, ttyp);
		tty_write(ttyp, txt, strlen(txt));
		tty_write(ttyp, "\r\n", 2);
	} else {
		menu_display(txt, ttyp);
	}
}

static struct menu_entry *
menu_parameter(struct menu_entry *e, tty_t *ttyp, char *mode)
{
	menu_idx(e, ttyp);
	if (!mode) 
		snprintf(buffer,sizeof(buffer),"%s query",(char *)e->args);
	else
		snprintf(buffer,sizeof(buffer),"%s help",(char *)e->args);
	cmd_do(ttyp, buffer, strlen(buffer), 1);
		
	return NULL;
}

static struct menu_entry *
menu_back(struct menu_entry *e, tty_t *ttyp, char *mode)
{
	if (e->args) {
		if (!mode) {
			tty_write(ttyp,"# Back\r\n",8);
			selection[10]=e;
		} else
			menu_display_entries(e, ttyp);
	} else if (!mode) {
			tty_write(ttyp,"# Exit\r\n",8);
			selection[10]=e;
		}
}

static struct menu_entry menu_root[] = {
	{menu_title, "Menu"},
	{menu_gui, "Transmit"},
	{menu_gui, "Settings"},
	{menu_parameter, "Reset"},
	{menu_back, NULL},
	{NULL, NULL},
};

static struct menu_entry menu_settings[] = {
	{menu_title, "Settings"},
	{menu_gui, "APRS"},
	{menu_gui, "Beacon"},
	{menu_gui, "Receive"},
	{menu_gui, "Transmit"},
	{menu_back, menu_root},
	{NULL, NULL},
};

static struct menu_entry menu_aprs[] = {
	{menu_title, "APRS"},
	{menu_parameter, "Mycall"},
	{menu_parameter, "Myalias"},
	{menu_back, menu_settings},
	{NULL, NULL},
};

static struct menu_entry menu_beacon[] = {
	{menu_title, "Beacon"},
	{menu_parameter, "GPS"},
	{menu_parameter, "Btext"},
	{menu_parameter, "Beacon"},
	{menu_back, menu_settings},
	{NULL, NULL},
};

static struct menu_entry menu_receive[] = {
	{menu_title, "Receive"},
	{menu_parameter, "Monitor"},
	{menu_parameter, "Digipeat"},
	{menu_back, menu_settings},
	{NULL, NULL},
};

static struct menu_entry menu_transmit[] = {
	{menu_title, "Transmit"},
	{menu_parameter, "TxDelay"},
	{menu_parameter, "Unproto"},
	{menu_back, menu_settings},
	{NULL, NULL},
};


static struct menu_entry *menus[] = {
	menu_settings,
	menu_aprs,
	menu_beacon,
	menu_receive,
	menu_transmit,
	menu_root
};

static void
menu_display(char *name, tty_t *ttyp)
{
	struct menu_entry **e=menus;
	debug_printf("Menu %s\r\n",name);
	for (;;) {
		char *title=(*e)->args;
		if (!strcasecmp("Menu",title) || !strcasecmp(name, title)) {
			menu_display_entries(*e, ttyp);
			break;
		}
		e++;
	}
	
}

void
gui_init(void)
{
	keypad_set_mode(KEYPAD_MODE_SIMPLE);
}

void
gui_process_char(char c, tty_t *ttyp)
{
	static int in_menu;
	struct menu_entry *e;
#if 0
	debug_printf("Got %d (%c)\r\n", c, c);
#endif
	if (c == '*') {
		in_menu=1;
		keypad_set_mode(KEYPAD_MODE_SIMPLE);
		tty_write(ttyp, "\ef", 2);
		cursor=0;
		menu_display(NULL, ttyp);
		return;
	}
	if (in_menu && c >= '0' && c <= '9') {
		e=selection[c == '0'?9:c-'1'];
		debug_printf("Selection %p\r\n", e);
		if (e)
			e->func(e, ttyp, "enter");
		return;
	}
	if (in_menu && c == '#' && selection[10]) {
		e=selection[10];
		e->func(e, ttyp, "enter");
		return;
	}
	if (!cursor) {
		tty_write(ttyp, "\ee", 2);
		cursor=1;
	}
	tty_write(ttyp, &c, 1);
	display_update();
}

void
gui_display_packet(tnc_t *tp)
{
	uint8_t *data = tp->pdata.data;
	if (param.mon == MON_ALL || (param.mon == MON_ME && ax25_callcmp(&param.mycall, &data[0]))) {
		tty_write(&display_tty, "\ef\ew", 2);
		cursor=0;
		display_packet_do(&display_tty, tp, &tp->pdata, DISPLAY_FLAGS_SRC|DISPLAY_FLAGS_DATA);
		tty_write(&display_tty, "\ev", 2);
	}
}

static void
set_addr(const char *addr, char *dest)
{
	int i;
	for (i = 0 ; i < 7 ; i++) {
		char c=addr[i];
		if (i == 6) {
			if (c >= 'a')
				c-='a'-10;
			else
				c-='0';
		}
		dest[i]=c<<1;
	}
}

bool
cmd_gui(tty_t *ttyp, uint8_t *buf, int len)
{
	static int idx;
	char ** test_packets[]={
		(char *[]){"AZZS480","DH1DF 3","WIDE1 1","WIDE2 2",NULL,":Test1234"},
		(char *[]){"DH1DF 3","DH1DF f","WIDE1 1","WIDE2 2",NULL,":It works"},
		(char *[]){"APDW170","DO4DR a","DB0KUHa","WIDE2 1",NULL,":BLN1WXZ  :DWD WARNUNG vor WINDBOEEN in Z bis 10.04. 18:00"},
		NULL,
	};
	tnc_t tp;
	char *ds=tp.pdata.data;
	char *d=ds;
	char **ptr=test_packets[idx++];
	if (!test_packets[idx])
		idx=0;
	while (*ptr) {
		set_addr(*ptr, d);
		d+=7;
		ptr++;
	}
	ptr++;	
	d[-1]|=1;
	d[0]=0x03;
	d[1]=0xf0;
	d+=2;
	strcpy(d, *ptr);
	d+=strlen(d);
	tp.pdata.data_cnt=d-ds+2;
	display_packet_do(&tty[TTY_USB], &tp, &tp.pdata, DISPLAY_FLAGS_ALL);
	gui_display_packet(&tp);
        return true;
}

#endif
