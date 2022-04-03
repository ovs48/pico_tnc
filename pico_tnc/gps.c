/*
Copyright (c) 2021, Kazuhisa Yokota, JN1DFF
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, 
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.
* Neither the name of the <organization> nor the names of its contributors 
  may be used to endorse or promote products derived from this software 
  without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#include "tnc.h"
#include "cmd.h"
#include "usb_output.h"
#include "tnc.h"
#include "usb_input.h"
#include "serial.h"
#include "unproto.h"

#define GPS_LEN 127
#define GPS_INTERVAL (3 * 60 * 100) // 3 min.
#define GPS_PORT 0

// usb echo flag
//uint8_t usb_echo = 1; // on

static uint8_t gps_buf[GPS_LEN + 1];
static int gps_idx = 0;

#define DOLLAR '$'
#define BS '\b'
#define CR '\r'
#define LF '\n'
#define DEL '\x7f'
#define BELL '\a'

static void gps_send(uint8_t *buf, int len)
{
    static uint32_t gps_timer = 0;
#if 0
    if (!strncmp("$GPRMC", buf, 6)) {
	char *str[32],*b=buf,*s;
	int idx=0;
	char *saveptr;
	while (idx < 31 && (s=strtok_r(b,",",&saveptr)) != NULL) {
		b=NULL;
		str[idx++]=s;
		str[idx]=NULL;
	}
	static int count=0;
	if (count++ > 10) {
		count=0;
		if (idx > 6 && !strcmp(str[2],"A")) {
			strcpy(str[3]+7,str[4]);
			strcpy(str[5]+8,str[6]);
			debug_printf("%s %s\r\n",str[3],str[5]);
		}
	}
    }
#endif
    if (tnc_time() - gps_timer < GPS_INTERVAL) return;

    if ((param.gps == GPGGA && !strncmp("$GPGAA", buf, 6))
        || (param.gps == GPGLL && !strncmp("$GPGLL", buf, 6))
        || (param.gps == GPRMC && !strncmp("$GPRMC", buf, 6))) {

        send_unproto(&tnc[GPS_PORT], buf, len);
        gps_timer = tnc_time();
    }
}

void gps_input(int ch)
{
#if 0
    static int count;
    if (count++ > 500) 
	    usb_write_char(ch);
#endif
    if (ch == DOLLAR) gps_idx = 0;

    if (gps_idx < GPS_LEN) gps_buf[gps_idx++] = ch;

    if (ch == LF) {
	gps_buf[gps_idx-1]='\0';
        gps_send(gps_buf, gps_idx);
        gps_idx = 0;
    }
}
