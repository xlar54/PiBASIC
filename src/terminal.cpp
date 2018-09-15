#include "terminal.h"

static Keyboard* keyboard;

extern "C"
{
	
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "vga.h"

#define MAXCOLS vga_width/vga_font_width
#define MAXROWS vga_height/vga_font_height

void term_init(Keyboard *keybrd)
{
	keyboard = keybrd;
	
	vga_clear();
	
	vga_screenmem = (uint8_t*)malloc(MAXCOLS * MAXROWS);

	uint32_t cursorTimer = TimerStartKernelTimer(30, term_cursorblink_handler, 0, (void *)cursorTimer);
	
	for(uint8_t row=0;row<MAXROWS; row++)
		for(uint8_t col=0;col<MAXCOLS; col++)
			term_putcharat(col,row,32);
		
	term_cursor_mode = 1;

}

void term_clear()
{
	vga_clear();
	
	term_row = 0;
	term_col = 0;
}

void term_putchar(uint8_t c)
{
	if(term_cursor_mode == 1)
		term_hidecursor();
	
	if (c > 127)
	{
		switch(c)
		{
			case 128: // key up
			{
				if(term_row !=0)
					term_row--;
				break;
			}
			case 129: // key down
			{
				term_row++;
				break;
			}
			case 130: // key left
			{
				// if at beginning of line and crsr left
				if(term_col == 0)
				{
					if(term_row == 0)
						break;
					else
					{
						term_col = MAXCOLS-1;
						term_row--;
					}
				}
				else
					term_col--;
				break;
			}
			case 131: // key right
			{
				term_col++;
				break;
			}
			case 132: // key home
			{
				term_col = 0;
				term_row = 0;
				break;
			}
		}
	}
	
	if(c == '\n')
	{
		term_col = 0;
		term_row++;
	}
	else if(c == '\b')
	{	
		if(term_col > 0)
		{	
			// simple same-line backspace
			term_col--;
			term_putcharat(term_col, term_row, 32);
		}
		else
		{
			// cursor is at start of a line. Only backspace if not at 0,0
			if(term_col == 0 && term_row != 0)
			{			
				term_col = MAXCOLS-1;
				term_row--;
				term_putcharat(term_col, term_row, 32);
			}
		}
	}
	else
	{
		// ordinary printable character
		if(c>31 && c<127)
		{
			term_putcharat(term_col, term_row, c);
			term_col++;
		}
	}
	
	
	// if at the end of the line, drop to next line
	if(term_col == MAXCOLS)
	{
		term_col = 0;
		term_row++;
	}	
	
	// scroll if we need to
	if(term_row == MAXROWS)
	{
		term_scroll();
		term_row = MAXROWS-1;
	}
	
	if(vga_cursor_mode == 1)
		term_showcursor();
}

void term_puts(char* text)
{
	int ctr = 0;
	while(text[ctr] != 0)
	{
		term_putchar(text[ctr]);
		ctr++;
	};
}

void term_printf(char* text, ...)
{
	// set up variable argument list
	va_list ap;
	va_start(ap, text);

	char res[256];
	char* result = res;
	
	// scan all characters in 'text' and look for format specifiers
	do
	{
		if(*text == '%')
		{
			if(*(text + 1) == 'c') // unsigned char
			{
				*result++ = (char)va_arg(ap, unsigned int);
			}
			else if(*(text + 1) == 'd') // integer (signed)
			{
				char buffer[10];
				snprintf(buffer, sizeof(buffer), "%d", va_arg(ap, int));
				char* intstr = strcpy(result,buffer);
				
				result += strlen(intstr);
			}
			else if(*(text + 1) == 'f') // float
			{
				char buffer[10];
				snprintf(buffer, sizeof(buffer), "%f", va_arg(ap, double));
				char* intstr = strcpy(result,buffer);
				
				result += strlen(intstr);
			}
			else if(*(text + 1) == 'g') // float (trimmed)
			{
				char buffer[10];
				snprintf(buffer, sizeof(buffer), "%g", va_arg(ap, double));
				char* intstr = strcpy(result,buffer);
				
				result += strlen(intstr);
			}
			else if(*(text + 1) == 's') // string
			{
				char* arg = (char*)va_arg(ap, unsigned int);
				
				strcpy(result,arg);
				
				result += strlen(arg);
			}
			
			// make sure we skip the type specifier
			text++;
			
			continue;
		}
		
		// if we got thus far, it's probably just a normal character
		*result++ = *text;
	}while(*text++ != '\0');
	
	result += 0;
	
	term_puts(res);
}

uint8_t term_getchar()
{
	if(keyboard->CheckChanged())
	{
		return keyboard->GetChar();
	}
	
	return 0;
}

void term_getcharat(uint8_t col, uint8_t row, uint8_t* ch)
{
	*ch = *(vga_screenmem + (row * MAXCOLS) + col);
}

void term_putcharat(uint8_t col, uint8_t row, uint8_t ch)
{
	*(vga_screenmem + (row * MAXCOLS) + col) = ch;
	
	vga_drawchar(col * vga_font_width, row * vga_font_height, ch);
}

void term_rc2xy(uint8_t col, uint8_t row, uint32_t* x, uint32_t* y)
{
	*x = col * vga_font_width;
	*y = row * vga_font_height;
}

void term_scroll()
{
	vga_scroll();
	
	memcpy(vga_screenmem, vga_screenmem + (1 * MAXCOLS), MAXCOLS*(MAXROWS-1));
	
	for(uint8_t c=0;c<MAXCOLS;c++)
		*(vga_screenmem + ((MAXROWS-1) * MAXCOLS) + c) = 32;
}

void term_showcursor()
{
	uint8_t c;
	term_getcharat(term_col, term_row, &c);
	
	vga_swapcolors();	
	term_putcharat(term_col, term_row, c);
	vga_swapcolors();
	
	term_cursor_on = 1;
}

void term_hidecursor()
{
	uint8_t c;
	term_getcharat(term_col, term_row, &c);
	term_putcharat(term_col, term_row, c);
	
	term_cursor_on = 0;
}

void term_toggle_cursor()
{
	if(term_cursor_on)
		term_hidecursor();
	else
		term_showcursor();
}

void term_cursorblink_handler(unsigned hTimer, void *pParam, void *pContext)
{
	term_toggle_cursor();
	uint32_t cursorTimer = TimerStartKernelTimer(30, term_cursorblink_handler, 0, (void *)cursorTimer);
}

}