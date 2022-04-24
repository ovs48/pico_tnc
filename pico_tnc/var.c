#include <string.h>
#include "tnc.h"

static char buffer[128];

const char *
getvar(const char *name)
{
	return gps_getvar(name);
}

const char *
substitute_var(const char *start, char *dest, int maxlen)
{
	const char *ptr=start;
	// debug_printf("substitute %s\r\n",start);
	for (;;) {
		char c=*ptr;
		if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_') {
			ptr++;
		} else {
			int len=ptr-start;
			// debug_printf("len=%d<%d\r\n",len,maxlen);
			if (len < maxlen) {
				const char *repl;
				strncpy(dest, start, len);
				dest[len]='\0';
				// debug_printf("var=%s\r\n",dest);
				repl=getvar(dest);
				// debug_printf("%s=%s\r\n",dest,repl);
				if (repl) {
					len=strlen(repl);
					if (len < maxlen) {
						strncpy(dest, repl, maxlen);
						dest[len]='\0';
						return ptr;
					}
				}
			}
			dest[0]='\0';
			return ptr;
		}
	}
}

const char *
substitute_vars(const char *str)
{
	int p=0,max=sizeof(buffer)-1;
	while (*str && p < max) {
		if (*str == '$') {
			str=substitute_var(str+1, buffer+p, max-p-1);
			p+=strlen(buffer+p);
		} else if (*str == '%') {
			if (str[1] != '%') {
				str=substitute_var(str+1, buffer+p, max-p-1);
				p+=strlen(buffer+p);
				if (*str == '%')
					str++;
			} else {
				buffer[p++]=*str;
				str+=2;
			}
		} else
			buffer[p++]=*str++;
	}
	buffer[p]='\0';
	return buffer;
}
