#ifndef TERMINAL_H
#define TERMINAL_H

#include "Keyboard.h"

extern "C"
{
#include "rpi-gpio.h"
#include "rpi-hardware.h"
#include "timer.h"

	
typedef unsigned char       BYTE;
typedef unsigned int   		size_t;

static uint8_t* vga_screenmem;
static uint8_t  term_row;
static uint8_t	term_col;
static uint8_t  term_cursor_mode;
static uint8_t  term_cursor_on;

void term_init(Keyboard *keyboard);
void term_putchar(uint8_t c);
void term_puts(char* text);

void term_printf(char* text, ...);
void term_getcharat(uint8_t col, uint8_t row, uint8_t* ch);
void term_putcharat(uint8_t col, uint8_t row, uint8_t ch);
void term_rc2xy(uint8_t col, uint8_t row, uint32_t* x, uint32_t* y);
void term_showcursor();
void term_hidecursor();
void term_toggle_cursor();
void term_scroll();
uint8_t term_getchar();
void term_cursorblink_handler(unsigned hTimer, void *pParam, void *pContext);

}

#endif


