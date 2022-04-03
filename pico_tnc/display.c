#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "tnc.h"
#include "tty.h"
#include "pico-ssd1306/ssd1306.h"

#ifdef ENABLE_DISPLAY

extern const uint8_t font_8x5[];

static struct display_context {
	enum {
		WRAP_FORWARD=1,
		WRAP_BACKWARD=2,
	} wrap;
	const uint8_t *font;
	uint32_t scale;

	enum {
		CURSOR_STATE_OFF,
		CURSOR_STATE_ON,
		CURSOR_STATE_DRAWN,
	} cursor_state;
	uint32_t cursor_x, cursor_y,cw,ch;
	struct {
		uint32_t x,y,w,h;
	} window;
	ssd1306_t disp;
} dc;

/* TODO: Move this into a ssd1306 fork and do a pull request */

static void ssd1306_clear_pixel(ssd1306_t *p, uint32_t x, uint32_t y) {
    if(x>=p->width || y>=p->height) return;

    p->buffer[x+p->width*(y>>3)]&=~(0x1<<(y&0x07)); // y>>3==y/8 && y&0x7==y%8
}

void ssd1306_clear_square(ssd1306_t *p, uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
    for(uint32_t i=0; i<width; ++i)
        for(uint32_t j=0; j<height; ++j)
            ssd1306_clear_pixel(p, x+i, y+j);
}


static void
display_home(struct display_context *dc)
{
	dc->cursor_x=dc->window.x;
	dc->cursor_y=dc->window.y;
}

static void
display_clear(struct display_context *dc)
{
	ssd1306_clear(&dc->disp);
	display_home(dc);
}

static void
display_update_do(struct display_context *dc)
{
	ssd1306_show(&dc->disp);
}

void
display_update()
{
	display_update_do(&dc);
}

static void
display_init_font(struct display_context *dc, const uint8_t *font, uint32_t scale)
{
	dc->font=font;
	dc->scale=scale;
	dc->cw=6*scale;
	dc->ch=9*scale;
}

void
display_init(void)
{
	i2c_init(I2C_OLED, 400 * 1000); /* 400kHz */
	gpio_set_function(GPIO_OLED_SDA, GPIO_FUNC_I2C);
	gpio_set_function(GPIO_OLED_SCL, GPIO_FUNC_I2C);
	gpio_pull_up(GPIO_OLED_SDA);
	gpio_pull_up(GPIO_OLED_SCL);
	dc.disp.external_vcc=false;
	dc.wrap=WRAP_FORWARD;
	dc.cursor_state=CURSOR_STATE_ON;
	dc.window.w=128;
	dc.window.h=64;
	display_init_font(&dc, font_8x5, 1);
	ssd1306_init(&dc.disp, dc.window.w, dc.window.h, 0x3C, I2C_OLED);
	ssd1306_clear(&dc.disp);
	char *str="init\r\n";
	display_write(str, strlen(str));
#if 0
	ssd1306_draw_string(&disp, 10, 10, 1, "Init");
	ssd1306_show(&disp);
#endif
}

static void
display_space(struct display_context *dc)
{
	ssd1306_clear_square(&dc->disp, dc->cursor_x, dc->cursor_y, dc->cw, dc->ch);
}

static void
display_cursor_clear(struct display_context *dc)
{
	if (dc->cursor_state == CURSOR_STATE_DRAWN) {
		display_space(dc);
		dc->cursor_state == CURSOR_STATE_ON;
	}
}

static void
display_write_do(struct display_context *dc, uint8_t const *data, int len)
{
	display_cursor_clear(dc);
	while (len >= (dc->cursor_state == CURSOR_STATE_OFF ? 1:0)) {
		uint8_t c = len?*data++:0;
		len--;
		if (c >= 32 || c == '\0') {
			bool done=true;
			do {
				if ((dc->wrap & WRAP_FORWARD) && dc->cursor_x+dc->cw > dc->window.w) {
					dc->cursor_x=dc->window.x;
					dc->cursor_y+=dc->ch;
					done=false;
				}  else {
					if (dc->cursor_x < dc->window.w && dc->cursor_y < dc->window.h) {
						if (c == 32) {
							display_space(dc);
						} else if (c != 0) {
							ssd1306_draw_char_with_font(&dc->disp, dc->cursor_x, dc->cursor_y, dc->scale, dc->font, c);
						}  else {
							ssd1306_draw_char_with_font(&dc->disp, dc->cursor_x, dc->cursor_y, dc->scale, dc->font, '_');
							dc->cursor_state == CURSOR_STATE_DRAWN;	
						}
						if (c != 0)
							dc->cursor_x+=dc->cw;
					}
				}
			} while(!done);
		} else if (c == '\b') {
			if (dc->cursor_x >= dc->window.x+dc->cw)
				dc->cursor_x-=dc->cw;
		} else if (c == '\r') {
			dc->cursor_x=dc->window.x;
		} else if (c == '\n') {
			display_update_do(dc);
			dc->cursor_y+=dc->ch;
		}
	}
}

void
display_write(uint8_t const *data, int len)
{
	display_write_do(&dc, data, len);
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
	display_clear(&dc);
	display_write_do(&dc, "OVS48",5);
	display_update_do(&dc);
#endif
	return true;
}

#endif
