#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "tnc.h"
#include "tty.h"
#include "usb_output.h"

void
debug_printf(const char *fmt, ...)
{
	char buffer[512];
	va_list argp;
	va_start(argp, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, argp);
	usb_write(buffer, strlen(buffer));
	va_end(argp);
}
