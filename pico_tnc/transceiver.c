#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "tnc.h"
#include "serial.h"
#include "tty.h"
#include "usb_output.h"
#include "receive.h"
#include "send.h"

#ifdef USE_EXTERNAL_TRANSCEIVER
int trx = 1;
#else
int trx = 0;
#endif

#ifdef ENABLE_TRANSCEIVER


uint32_t cmd_start;
uint32_t transceiver_timeout=10;
const char *expected_result;
int result_pos;

enum {
	TRANSCEIVER_DEBUG_RX=1,
	TRANSCEIVER_DEBUG_TX=2,
	TRANSCEIVER_DEBUG_TIMEOUT=4,
	TRANSCEIVER_DEBUG_INIT=8,
	TRANSCEIVER_DEBUG_POWER=16,
	TRANSCEIVER_DEBUG_POWER_ENABLE=32
} transceiver_debug;

enum {
	TRANSCEIVER_INIT_STATE_BOOT,
	TRANSCEIVER_INIT_STATE_DMOCONNECT,
	TRANSCEIVER_INIT_STATE_DMOSETGROUP,
	TRANSCEIVER_INIT_STATE_DMOSETVOLUME,
	TRANSCEIVER_INIT_STATE_SETFILTER,
	TRANSCEIVER_INIT_STATE_IDLE,
} transceiver_init_state;

static void
transceiver_command(const char *cmd, const char *result, uint32_t timeout)
{
	serial_write(cmd,strlen(cmd));
	if (transceiver_debug & TRANSCEIVER_DEBUG_TX)
		debug_printf("%s",cmd);
	cmd_start=tnc_time();
	expected_result=result;
	transceiver_timeout=timeout;
}

static void
transceiver_command_timeout(void)
{
	if (transceiver_debug & TRANSCEIVER_DEBUG_TIMEOUT)
		debug_printf("timeout %d\r\n",transceiver_init_state);
	switch (transceiver_init_state) {
	case TRANSCEIVER_INIT_STATE_BOOT:
		transceiver_init_state=TRANSCEIVER_INIT_STATE_DMOCONNECT;
		transceiver_command("AT+DMOCONNECT\r\n","+DMOCONNECT:0\r\n",50);
	default:
		transceiver_timeout=0;
	}
}

static void
transceiver_command_complete(void)
{
	// debug_printf("complete\r\n");
	switch (transceiver_init_state) {
	case TRANSCEIVER_INIT_STATE_DMOCONNECT:
		transceiver_init_state=TRANSCEIVER_INIT_STATE_DMOSETGROUP;
		//#ifdef USE_EXTERNAL_TRANSCEIVER
		if(trx!=0)
		{			
			transceiver_command("AT+DMOSETGROUP=1,144.8000,144.8000,0000,8,0000\r\n","+DMOSETGROUP:0\r\n",10);
		}
		//#else
		else
		{
			transceiver_command("AT+DMOSETGROUP=1,144.8000,144.8000,0000,0,0000\r\n","+DMOSETGROUP:0\r\n",10);
		}
		//#endif
		break;
	case TRANSCEIVER_INIT_STATE_DMOSETGROUP:
		transceiver_init_state=TRANSCEIVER_INIT_STATE_DMOSETVOLUME;
		transceiver_command("AT+DMOSETVOLUME=3\r\n","+DMOSETVOLUME:0\r\n",10);
		break;
	case TRANSCEIVER_INIT_STATE_DMOSETVOLUME:
		transceiver_init_state=TRANSCEIVER_INIT_STATE_SETFILTER;
		transceiver_command("AT+SETFILTER=0,1,1\r\n","+DMOSETFILTER:0\r\n",10);
		break;
	case TRANSCEIVER_INIT_STATE_SETFILTER:
		transceiver_init_state=TRANSCEIVER_INIT_STATE_IDLE;
                transceiver_timeout=0;
		break;
	}
}


void
transceiver_init(void)
{
#if 1
	gpio_init(GPIO_PD);
	gpio_set_dir(GPIO_PD, true); // output
	gpio_put(GPIO_PD, 1);
	gpio_init(GPIO_PTT1);
	gpio_set_dir(GPIO_PTT1, true); // output
	gpio_put(GPIO_PTT1, 1);
	gpio_init(GPIO_LED);
	gpio_set_dir(GPIO_LED, false); // input
#if 0
	gpio_put(GPIO_LED, 0);
#endif
#endif

#ifdef USE_EXTERNAL_TRANSCEIVER
	//#define GPIO_PTT1 GPIO_PTT0	//Pinzuweisung; DRA PTT ist jetzt auch ext TRX PTT
	//gpio_put(GPIO_PD, 0);
#else
	//foo
#endif
}

static int init;

void
transceiver_input(void)
{
	while (uart_is_readable(uart0)) {
		int ch = uart_getc(uart0);
		if (transceiver_debug & TRANSCEIVER_DEBUG_RX)
			debug_printf("%c",ch);
		if (ch == expected_result[result_pos++]) {
			if (!expected_result[result_pos])
				transceiver_command_complete();
		} else {
			result_pos=0;
			if (ch == expected_result[result_pos])
				result_pos++;
		}
    	}
	if (transceiver_timeout && tnc_time() - cmd_start >= transceiver_timeout) {
		transceiver_command_timeout();
	}
}


bool switch_state(int st, tty_t* ttyp)
{
	static int switches=0;
	if(switches>=3)
	{
		tty_write_str(ttyp, "Limit Reached");
		return false;
	}
	char buffer[50];
	/*sprintf(buffer, "TRX is now %d\r\nState (st) is now %d\r\n", trx, st);
	tty_write_str(ttyp, buffer);*/

    if(st==0 && trx==1)  //Intern auf Extern
    {
        //gpio_put(GPIO_PD, 0);
        trx--;
        transceiver_command("AT+DMOSETGROUP=1,144.8000,144.8000,0000,8,0000\r\n","+DMOSETGROUP:0\r\n",10);
		/*sprintf(buffer, "TRX is now %d\r\nState (st) is now %d\r\n", trx, st);
		tty_write_str(ttyp, buffer);*/
		switches++;
    }
    else if (st==1 && trx==0)    //Extern auf intern
    {
        //gpio_put(GPIO_PD, 1);
        trx++;
        transceiver_command("AT+DMOSETGROUP=1,144.8000,144.8000,0000,0,0000\r\n","+DMOSETGROUP:0\r\n",10);
		/*sprintf(buffer, "TRX is now %d\r\nState (st) is now %d\r\n", trx, st);
		tty_write_str(ttyp, buffer);*/
		switches++;
    }
	else
		return false;

	//transceiver_init();
	return true;
}


bool
cmd_transceiver(tty_t *ttyp, uint8_t *buf, int len)
{
	int st = -1;
	if (buf && len > 6 && !strncasecmp(buf, "debug ",6)) {
		transceiver_debug=atoi(buf+6);
		if (transceiver_debug & TRANSCEIVER_DEBUG_INIT) {
			transceiver_timeout=10;
			transceiver_init_state=TRANSCEIVER_INIT_STATE_BOOT;
		}
		gpio_set_dir(GPIO_LED, !!(transceiver_debug & TRANSCEIVER_DEBUG_POWER_ENABLE)); 
		gpio_put(GPIO_LED, !!(transceiver_debug & TRANSCEIVER_DEBUG_POWER));
		return true;
	}
	else if (buf && len > 6 && !strncasecmp(buf, "external",8))
	{
		st=1;
	}
	else if (buf && len > 6 && !strncasecmp(buf, "internal",8))
	{
		st=0;
	}
	if(st>=0)
	{
		if(!switch_state(st, ttyp))
		{
			return false;
		}
		else
		{
			if(trx!=st)
			{
				tty_write_str(ttyp, "TRX VAR not set properly");
				return false;
			}
		}
		send_init();
		receive_init();
		//return true;
	}
	/*tty_write_str(ttyp, "Transceiver test ");
	tty_write(ttyp, buf, len);*/
	if(trx==0)
        tty_write_str(ttyp, "INT TRX");
    else
        tty_write_str(ttyp, "EXT TRX");
	tty_write_str(ttyp, "\r\n");
	if (len == 1 && buf[0] == '1') {
		transceiver_command("AT+DMOCONNECT\r\n","",50);
	}
	if (len == 1 && buf[0] == '2') {
		transceiver_command("AT+DMOSETGROUP=0,144.8000,144.8000,0000,0,0000\r\n","",10);
	}
	if (len == 1 && buf[0] == '3') {
		gpio_put(GPIO_PTT1, 0);
	}
	if (len == 1 && buf[0] == '4') {
		gpio_put(GPIO_PTT1, 1);
	}
	if (len == 1 && buf[0] == '5') {
		transceiver_command("AT+DMOSETVOLUME=8\r\n","",10);
	}
	if (len == 1 && buf[0] == '6') {
		init=0;
	}
	return true;
}

#endif