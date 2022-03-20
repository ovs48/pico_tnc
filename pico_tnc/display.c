#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "tnc.h"
#include "tty.h"
#include "pico-ssd1306/ssd1306.h"

#ifdef ENABLE_DISPLAY

extern const uint8_t font_8x5[];

static ssd1306_t disp;
static char cursor_x, cursor_y;
static bool wrap = true;
const uint8_t *font = font_8x5;

void
display_home(void)
{
	cursor_x=0;
	cursor_y=0;
}

void
display_clear(void)
{
	ssd1306_clear(&disp);
	display_home();
}

void
display_update(void)
{
	ssd1306_show(&disp);
}

void
display_init(void)
{
	i2c_init(I2C_OLED, 400 * 1000); /* 400kHz */
	gpio_set_function(GPIO_OLED_SDA, GPIO_FUNC_I2C);
	gpio_set_function(GPIO_OLED_SCL, GPIO_FUNC_I2C);
	gpio_pull_up(GPIO_OLED_SDA);
	gpio_pull_up(GPIO_OLED_SCL);
	disp.external_vcc=false;
	ssd1306_init(&disp, 128, 64, 0x3C, I2C_OLED);
	ssd1306_clear(&disp);
	char *str="init\r\n";
	display_write(str, strlen(str));
#if 0
	ssd1306_draw_string(&disp, 10, 10, 1, "Init");
	ssd1306_show(&disp);
#endif
}

void
display_write(uint8_t const *data, int len)
{
	uint32_t scale=1;
	
	
	while (len > 0) {
		uint8_t c=*data++;
		len--;
		if (c >= 32) {
			bool done=true;
			do {
				uint32_t x=cursor_x*6;
				uint32_t y=cursor_y*9;
				if (wrap && x+font[0] >= disp.width) {
					cursor_x=0;
					cursor_y++;
					done=false;
				}  else {
					if (x < disp.width && y < disp.height) {
						ssd1306_draw_char_with_font(&disp, x, y, scale, font, c);
						cursor_x++;
					}
				}
			} while(!done);
		} else if (c == '\r') {
			cursor_x=0;
		} else if (c == '\n') {
			display_update();
			cursor_y++;
		}
	}
}

bool reserved_addr(uint8_t addr) {
	return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}


bool
cmd_display(tty_t *ttyp, uint8_t *buf, int len)
{
#if 0
	tty_write_str(ttyp,"   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\r\n");

	for (int addr = 0; addr < (1 << 7); ++addr) {
		if (addr % 16 == 0) {
			tty_write_byte(ttyp, addr);
			tty_write_str(ttyp, " ");
		}

		int ret;
		uint8_t rxdata;
		if (reserved_addr(addr))
			ret = PICO_ERROR_GENERIC;
		else
			ret = i2c_read_blocking(i2c_default, addr, &rxdata, 1, false);

		tty_write_str(ttyp, ret < 0 ? "." : "@");
		tty_write_str(ttyp, addr % 16 == 15 ? "\r\n" : "  ");
	}
	tty_write_str(ttyp, "Done.\r\n");
#else
	ssd1306_clear(&disp);
	ssd1306_draw_string(&disp, 10, 10, 2, "OVS48");
	ssd1306_show(&disp);
#endif
	return true;
}

#endif
