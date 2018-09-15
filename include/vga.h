#ifndef VGA_H
#define VGA_H

#include "types.h"
#include <stdlib.h>
#include <string.h>
#include "debug.h"
//#include "stb_image_config.h"
#include "rpi-mailbox-interface.h"
#include "font_data.h"

typedef u32 RGBA;

#define RED(colour)   ( (u8)(((u32)colour) & 0xFF) )
#define GREEN(colour) ( (u8)(((u32)colour >> 8) & 0xFF) )
#define BLUE(colour)  ( (u8)(((u32)colour >> 16) & 0xFF) )
#define ALPHA(colour) ( (u8)(((u32)colour >> 24) & 0xFF) )

#define RGBA(r, g, b, a)  ( ((u32)((u8)(r))) | ((u32)((u8)(g)) << 8) | ((u32)((u8)(b)) << 16) | ((u32)((u8)(a)) << 24) )

#define COLOUR_BLACK 	RGBA(0x00, 0x00, 0x00, 0xff)
#define COLOUR_WHITE 	RGBA(0xff, 0xff, 0xff, 0xff)
#define COLOUR_RED 		RGBA(0xff, 0x00, 0x00, 0xff)
#define COLOUR_GREEN 	RGBA(0x00, 0xff, 0x00, 0xff)
#define COLOUR_BLUE 	RGBA(0x00, 0x00, 0xff, 0xff)
#define COLOUR_CYAN 	RGBA(0x00, 0xff, 0xff, 0xff)
#define COLOUR_YELLOW 	RGBA(0xff, 0xff, 0x00, 0xff)
#define COLOUR_PURPLE 	RGBA(0xff, 0x00, 0x00, 0xff)
#define COLOUR_GRAY 	RGBA(0x80, 0x80, 0x80, 0xff)

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

static const int vga_font_height = 8;
static const int vga_font_width = 8;
static const uint8_t* vga_current_font = font8x8_basic;
static u32 vga_current_fg_color = COLOUR_CYAN;
static u32 vga_current_bg_color = COLOUR_BLUE;
static u8 vga_cursor_mode = 1;

typedef void (*funcptr)(u32 pixel_offset);
extern float vga_scaleX;
extern float vga_scaleY;
extern u32 vga_width;
extern u32 vga_height;
extern u32 vga_bpp;
extern u32 vga_pitch;
extern u8* vga_framebuffer;
extern funcptr vga_plotpixelFn;

extern void RPI_PropertyInit( void );
extern void RPI_PropertyAddTag( rpi_mailbox_tag_t tag, ... );
extern int RPI_PropertyProcess( void );
extern void RPI_PropertyProcessNoCheck( void );
extern rpi_mailbox_property_t* RPI_PropertyGet( rpi_mailbox_tag_t tag );

void vga_init(u32 widthDesired, u32 heightDesired, u32 colourDepth);
void vga_release();
void vga_clear();
void vga_cleararea(u32 x1, u32 y1, u32 x2, u32 y2);

void vga_setforecolor(RGBA color);
void vga_setbackcolor(RGBA color);
void vga_swapcolors();

void vga_cursor_on();
void vga_cursor_off();

void vga_plotpixel32(u32 pixel_offset);
void vga_plotpixel24(u32 pixel_offset);
void vga_plotpixel16(u32 pixel_offset);
void vga_plotpixel8(u32 pixel_offset);
void vga_plotpixel(u32 x, u32 y);

void vga_drawline(u32 x1, u32 y1, u32 x2, u32 y2);
void vga_drawlinev(u32 x, u32 y1, u32 y2);
void vga_drawrect(u32 x0, u32 y0, u32 w, u32 h);
void vga_drawcircle(u32 x0, u32 y0, u32 r);
//void vga_plotimage(u32* image, int x, int y, int w, int h);

void vga_cliprect(u32 x1, u32 y1, u32 x2, u32 y2);
void vga_drawchar(u32 x, u32 y, unsigned char c);
u32 vga_drawtext(u32 x, u32 y, char *ptr);
void vga_drawcursor(u32 x, u32 y);
void vga_erasecursor(u32 x, u32 y);

void vga_scroll();

#endif