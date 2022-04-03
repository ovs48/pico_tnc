#include <string.h>
#include "pico/stdlib.h"
#include "tnc.h"
#include "tty.h"

#ifdef ENABLE_KEYPAD

static char rows[]={GPIO_ROW1,GPIO_ROW2,GPIO_ROW3,GPIO_ROW4};
static char cols[]={GPIO_COL1,GPIO_COL2,GPIO_COL3,GPIO_COL4};
static int active_row=-1;
static int active_col=-1;
static char debounce_counter,current_key,last_key;

static char *sym_table[]=
{
	"1.,?!@&`%-:*#",
	"2ABC",
	"3DEF",
	" ",	//Next
	"4GHI",
	"5JKL",
	"6MNO",
	"\b",	//Backspace
	"7PQRS",
	"8TUV",
	"9WXYZ",
	"c",
	"*",
	"0 ",
	"#",	//Modifier
	"d",
};

/*static int* key_table[][4]={
	{key_1, key_2 ,key_3, key_A},	
	{key_4,key_5,key_6, key_B},	
	{key_7,key_8,key_9, key_C},	
	{key_star ,key_0, key_hash, key_D},	
};*/

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


char keypad_getchar(int key)
{
	static int curr_key=0;
	static int counter=0;
	if(curr_key==key)
	{
		keypad_process('\b');
		keypad_process(' ');
		if(key==3 || key ==7)
		{
			if (key==3)
			{
				return(' ');
			}
			else
			{
				keypad_process('\b');
				keypad_process(' ');
				keypad_process('\b');
				return('\0');
			}
		}
		else 
		{
			if(counter>=strlen(sym_table[key]))
				counter = 0;
			else
				counter++;
			keypad_process('\b');
			return(sym_table[key][counter]);
		}
	}
	else
	{
		if(key==3 || key == 7)
		{
			if(key==3)
			{
				return(' ');
			}
			else
			{
				keypad_process('\b');
				keypad_process(' ');
				keypad_process('\b');
				return('\0');
			}
		}
		else
		{
			curr_key=key;
			counter=0;
			//keypad_process(' ');
			return(sym_table[key][counter]);
		}
	}
}

static void
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
				current_key = keypad_getchar(key);
				
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
		current_key='\0';
		active_col = -1;
		active_row = -1;
		enable_rows();
	}
}

static void
keypad_process(char c)
{
#if 0
	tty_write_char(&tty[0],c);
#else
	display_write(&c, 1);
	display_update();
#endif
}

void
keypad_poll(void)
{
	keypad_get();
	if (current_key == last_key) {
		if (debounce_counter < 10) {
			debounce_counter++;
			if (debounce_counter == 10 && current_key != '\0') {
				keypad_process(current_key);	
			}
		}
	} else {
		last_key=current_key;
		if (debounce_counter > 0)
			debounce_counter--;
	}
}

#endif
