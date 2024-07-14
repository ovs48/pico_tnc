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
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"

#include "tnc.h"
#include "cmd.h"
#include "usb_output.h"
#include "tnc.h"
#include "usb_input.h"
#include "serial.h"
#include "unproto.h"
#include "tty.h"

#define GPS_LEN 127
#define GPS_INTERVAL (3 * 60 * 100) // 3 min.
#define GPS_PORT 0

// usb echo flag
//uint8_t usb_echo = 1; // on

static uint8_t gps_buf[GPS_LEN + 1];
static int gps_idx = 0;
enum gps_debug gps_debug;

#define DOLLAR '$'
#define BS '\b'
#define CR '\r'
#define LF '\n'
#define DEL '\x7f'
#define BELL '\a'

static char lat[10]="0000.00N";
static char lon[10]="00000.00E";
// static char lat[10]="5033.43N";
// static char lon[10]="01257.46E";
static char compressedPos[13] = "";

//in - der String aus dem die Zeichenkette gelesen werden soll.
//cmp - der String der die zu lesende Zeichenkette enthält.
int cmp_str(const char * in, const char * cmp)
{
  int len = strlen(cmp);
  int notequal=0;
  for(int i=0; i<len && notequal == 0; i++)
  {
    if(in[i] != cmp[i]) notequal = 1;
  }
  return notequal;
}

void gps_info(char *str)
{
  static int status=1;
  int chk=0;
  int ost=status;
  int i;
  for(i=0; i<strlen(str); i++)
  {
    if(str[i]>=48 && str[i]<=57)
    {
      //display_write(&str[i], 1);
      //tty_write(&display_tty, &str[i], 1);
      chk+=(str[i]-48);
    }
  }
  char dsp[10]="";
  if(chk!=0 && status==0)
  {
    status++;
    strcpy(dsp, "GPS OK\r\n");
    //tty_write(&display_tty, "GPS OK", strlen("GPS OK"));
  }
  if(chk==0 && status==1)
  {
    status--;
    strcpy(dsp, "GPS NC\r\n");
    //tty_write(&display_tty, "GPS NC", strlen("GPS NC"));
  }
  if(ost!=status)
  {
    display_write(dsp, strlen(dsp));
	  display_update();
  }
}

static void gps_compress();
const char *gps_getvar(const char *name)
{
	if (!cmp_str(name,"LAT"))
  {
    //gps_info(lat);
		return lat;
  }
	if (!cmp_str(name,"LON"))
  {
    //gps_info(lon);
		return lon;
  }
  if (!cmp_str(name,"COMP"))
  {
    //gps_info(lon);
    gps_compress();
    debug_printf("Compressed Data: %s", compressedPos);
		return compressedPos;
  }
}

float getDecimal(bool is_lon)
{
  unsigned short coeff = 1;

  float raw_data = 0.0;
  if(is_lon)
  {
      raw_data = atof(lon);
      if(lon[strlen(lon)-1] == 'W') coeff = -1;
      debug_printf("%c", lon[strlen(lon)-1]);
  } 
  else 
  {
    raw_data = atof(lat);
    if(lat[strlen(lat)-1] == 'S') coeff = -1;
      debug_printf("%c", lat[strlen(lat)-1]);
  }

  int raw_deg = (int)floor(raw_data / 100);
  int raw_min = (int)floor(raw_data - (raw_deg * 100));
  float raw_secs = (float)(raw_data - floor(raw_data)) * 60.0;

  float decimaldata = raw_deg + ((float)raw_min / 60.0) + (raw_secs / 3600.0);
  decimaldata = decimaldata * (float)coeff;

  debug_printf("Decimal Degs: %f, Raw: %f, Degs: %d, Mins: %d, Secs: %f\r\n", decimaldata, raw_data, raw_deg, raw_min, raw_secs);
  return decimaldata;
}

void compressX(char* x)
{
  float decLon = getDecimal(true);
  int baseVal = 190463 * (180.0 + decLon);
  int temp = 0;

  char values[4];
  
  for(int i = 0; i<4; i++)
  {
    //Value divided by 91^(3-i)
    float power = pow(91, 3-i);
    temp = floor((float)baseVal / power);
    baseVal -= temp * power;
    values[i] = temp + 33;
    debug_printf("[X] Compressed value %d (divided by %f) with remainder %d to %c\r\n", temp, power, baseVal, values[i]);
  }

  debug_printf("Raw Lon: %f, Compressed Data: %s\r\n", decLon, values);

  strncpy(x, values, 4);
}

void compressY(char* y)
{
  float decLat = getDecimal(false);
  int baseVal = 380926 * (90 - decLat);
  int temp = 0;

  char values[4];
  
  for(int i = 0; i<4; i++)
  {
    //Value divided by 91^(3-i)
    float power = pow(91, 3-i);
    temp = floor((float)baseVal / power);

    //Set the base Val to remainder
    baseVal -= temp * power;

    //Add 33 so the resulting byte gets in the valid range
    values[i] = temp + 33;
    debug_printf("[Y] Compressed value %d (divided by %f) with remainder %d to %c\r\n", temp, power, baseVal, values[i]);
  }

  debug_printf("Raw Lat: %f, Compressed Data: %s\r\n", decLat, values);

  strncpy(y, values, 4);
}

static void gps_compress()
{
  //Compresses GPS data as a whole payload
  /*
    Compressed data format (with bytelength):
    TYYYYXXXXSIIC
    Where
    T -> Symbol Table ID (aka Overlay), 1 byte
    Y -> Compressed lat, 4 bytes
    X -> Compressed lon, 4 bytes
    S -> Symbol Code (for example > with T='/' for car), 1 byte

    The following bytes are not used for now. So after your symbol code you have to insert a SPACE followed by two filler characters.
    I -> Additional compressed info, like speed, course OR range OR alt. 2 bytes
    C -> Compression type, 1 byte

    Overall length: 13 bytes max

    For example, =/XXXXYYYY> --Lukas S48
    Corresponds to a compressed APRS packet with
      - Symbol Table "/" to link to regular symbols
      - Compressed Position XXXXYYYY
      - Symbol Code ">", which in combination with table "/" corresponds to a car
      - SPACE Dash Dash as seperator and filler
      - Lukas S48 as comment
  */
  //Compression type byte
  char origin = (char)0b00100110;
  char y[5] = "0000";
  char x[5] = "0000";
  char cs[2] = "00";

  //Add GPS Info to compression type
  switch(param.gps)
  {
    case GPGGA:
      origin += 0b10000;
      break;

    case GPRMC:
      origin += 0b11000;
      break;

    case GPGLL:
      origin += 0b01000;
      break;
  }

  compressX(x);
  compressY(y);

  sprintf(compressedPos, "%s%s", y, x);
  debug_printf("compressedPos should look like \"%s%s\"\r\n", y, x);
  // strncpy(compressedPos, "/", 1);
  // strncpy(compressedPos+1, y, 4);
  // strncpy(compressedPos+5, x, 4);
  // strncpy(compressedPos+9, ">", 1);
  // strncpy(compressedPos+10, cs, 2);
  // strncpy(compressedPos+12, origin, 1);
}

static void gps_send(uint8_t *buf, int len)
{
    static uint32_t gps_timer = 0;
    if (gps_debug & GPS_DEBUG_RAW)
	debug_printf("%s\r\n",buf);
    if (!strncmp("$GPRMC", buf, 6)) {
	char *str[32],*b=buf,*s;
	int idx=0;
	char *saveptr;
	if (gps_debug & GPS_DEBUG_GPRMC)
		debug_printf("%s\r\n",buf);
	while (idx < 31 && (s=strtok_r(b,",",&saveptr)) != NULL) {
		b=NULL;
		str[idx++]=s;
		str[idx]=NULL;
	}
	if (idx > 6 && !strcmp(str[2],"A")) {
		strcpy(str[3]+7,str[4]);
		strncpy(lat, str[3], 8);
		strcpy(str[5]+8,str[6]);
		strncpy(lon, str[5], 9);
		if (gps_debug & GPS_DEBUG_LATLON) {
			debug_printf("LAT=%s LON=%s\r\n",lat, lon);
		}
	}
    }
    if (tnc_time() - gps_timer < GPS_INTERVAL) return;

    if ((param.gps == GPGGA && !strncmp("$GPGAA", buf, 6))
        || (param.gps == GPGLL && !strncmp("$GPGLL", buf, 6))
        || (param.gps == GPRMC && !strncmp("$GPRMC", buf, 6))) {

	if (gps_debug & GPS_DEBUG_REPORT)
	    debug_printf("%s\r\n",buf);
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
        gps_info(lat);
    }
}
