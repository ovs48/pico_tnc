#include <string.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "tnc.h"
#include "tty.h"

#ifdef ENABLE_KEYPAD

static char rows[]={GPIO_ROW1,GPIO_ROW2,GPIO_ROW3,GPIO_ROW4};
static char cols[]={GPIO_COL1,GPIO_COL2,GPIO_COL3,GPIO_COL4};
static int active_row=-1;
static int active_col=-1;
static char debounce_counter;
static char last_char;
static char *current_key,*last_key,*last_table;
static enum keypad_mode keypad_mode;
static uint32_t key_start_time;

static char *sym_table[]=
{
	"1.,?!@&`%-:*#",
	"2ABC",
	"3DEF",
	"~",	//Backspace
	"4GHI",
	"5JKL",
	"6MNO",
	" ",	//Space
	"7PQRS",
	"8TUV",
	"9WXYZ",
	"-",	//Dash
	"*",
	"0 ",
	"#",	//Modifier
	"\r",	// Return
};

static void keypad_process(char c);

static void
disable_rows(void)
{
	int i;
	for (i = 0 ; i < sizeof(rows)/sizeof(rows[0]); i++) {
		gpio_put(rows[i], 0);
	}
}

static void
enable_row(int i)
{
	gpio_put(rows[i], 1);
}

static void
enable_rows(void)
{
	int i;
	for (i = 0 ; i < sizeof(rows)/sizeof(rows[0]); i++) {
		enable_row(i);
	}
}

void
keypad_init(void)
{
	int i=0;
	for (i = 0 ; i < sizeof(rows)/sizeof(rows[0]); i++) {
		char gpio=rows[i];
		gpio_init(gpio);
		gpio_set_dir(gpio, true);
	}
	enable_rows();
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

static int
first_bit(char val)
{
	int ret=-1;
	while (val) {
		ret++;
		val>>=1;
	}
	return ret;
}

static int
get_col(void)
{
	return  first_bit(get_cols());
}

bool
cmd_keypad(tty_t *ttyp, uint8_t *buf, int len)
{
	int i;
	tty_write_str(ttyp, "All active cols: ");
	tty_write_byte(ttyp, get_cols());
	tty_write_str(ttyp, "\r\n");
	disable_rows();
	for (i = 0 ; i < sizeof(rows)/sizeof(rows[0]); i++) {
		enable_row(i);
		tty_write_str(ttyp, "Active col in row ");
		tty_write_byte(ttyp, i);
		tty_write_str(ttyp,":");
		tty_write_byte(ttyp, get_cols());
		tty_write_str(ttyp, "\r\n");
		disable_rows();
	}
	enable_rows();
	keypad_process('_');
	return true;
}

void backspace()
{
char backspc[5]={' ', '\b', '\b', ' ', '\b'};
#if 0
	tty_write_char(&tty[0],c);
#endif
#if 0
	display_write(backspc, 5);
	display_update();
#endif
}

void
keypad_set_mode(enum keypad_mode mode)
{
	keypad_mode=mode;	
}

static void
keypad_process(char c)
{
#if 0
	tty_write_char(&tty[0],c);
#endif
#if 0
	display_write(&c, 1);
	display_update();
#endif
	gui_process_char(c, &display_tty);
}

static void
keypad_timeout(void)
{
	keypad_process(last_char);
	last_table=NULL;
	last_char='\0';
}

static void
keypad_process_table(char *table)
{
	static int counter=0;
	static int modifier=0;	//0 -- Standard, 1 -- Kleinbuchstaben, 2 -- Nur Zahlen
	int next=1;

	if (!table) {
		// debug_printf("-");
		return;
	}
	// debug_printf("%c",table[0]);
	if (keypad_mode == KEYPAD_MODE_SIMPLE) {
		keypad_process(table[0]);
		return;
	}
	if(table[0] == '#') {
		if(modifier==2) 
			modifier=0;
		else
			modifier++;
		table=last_table;
		next=0;
	}
	if(last_table!=table)
	{
		last_table=table;
		if (last_char)
			keypad_process(last_char);
		counter=0;
	}
	if(modifier==2 || strlen(table) == 1) {
		keypad_process(table[0]);
		last_char='\0';
		counter=0;
	} else {
		last_char=table[counter];
		if(modifier==1)
			last_char=tolower(last_char);
		keypad_process(last_char);
		keypad_process('\b');
		key_start_time=tnc_time();
		counter += next;
		if(counter>=strlen(table))
			counter = 0;
	}
}


static char *
keypad_get(void)
{
	int key;
	int i;
	if (active_col == -1) {
		active_col = get_col();
#if 0
		if (active_col != -1)
			tty_write_char(&tty[0],'a');
#endif
			
	}
	if (active_col != -1 && active_row == -1) {
#if 0
		tty_write_char(&tty[0],'b');
#endif
		disable_rows();
		for (i = 0 ; i < sizeof(rows)/sizeof(rows[0]); i++) {
			enable_row(i);
			busy_wait_us_32(100);
			active_col = get_col();
			if (active_col != -1) {
				active_row = i;

				key=(active_col*4) + active_row;
				current_key=sym_table[key];
				
#if 0
				tty_write_char(&tty[0],'c');
				tty_write_byte(&tty[0],active_col);
				tty_write_byte(&tty[0],active_row);
				tty_write_char(&tty[0],current_key);
#endif
				break;
			}
		}
	}
	if (active_col != -1 && active_row != -1 && active_col != get_col()) {
#if 0
		tty_write_char(&tty[0],'d');
#endif
		current_key=NULL;
		active_col = -1;
		active_row = -1;
		enable_rows();
	}
	return current_key;
}

void
keypad_poll(void)
{
	keypad_get();
	if (current_key == last_key) {
		if (debounce_counter < 10) {
			debounce_counter++;
			if (debounce_counter == 10) 
				keypad_process_table(current_key);
		}
	} else {
		last_key=current_key;
		if (debounce_counter > 0)
			debounce_counter--;
	}
	if (last_char && tnc_time() - key_start_time >= 50)
		keypad_timeout();
}

#endif
