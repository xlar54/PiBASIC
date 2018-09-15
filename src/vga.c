#include "vga.h"


void vga_init(u32 widthDesired, u32 heightDesired, u32 colourDepth)
{
	/*if (widthDesired < 320)
		widthDesired = 320;
	if (heightDesired < 240)
		heightDesired = 240;
	if (widthDesired > 1024)
		widthDesired = 1024;
	if (heightDesired > 720)
		heightDesired = 720;*/

	vga_framebuffer = 0;
	rpi_mailbox_property_t* mp;

	vga_scaleX = (float)widthDesired / 1024.0f;
	vga_scaleY = (float)heightDesired / 768.0f;

	do
	{
		RPI_PropertyInit();
		RPI_PropertyAddTag(TAG_ALLOCATE_BUFFER);
		RPI_PropertyAddTag(TAG_SET_PHYSICAL_SIZE, widthDesired, heightDesired);
		RPI_PropertyAddTag(TAG_SET_VIRTUAL_SIZE, widthDesired, heightDesired);
		RPI_PropertyAddTag(TAG_SET_DEPTH, colourDepth);
		RPI_PropertyAddTag(TAG_GET_PITCH);
		RPI_PropertyAddTag(TAG_GET_PHYSICAL_SIZE);
		RPI_PropertyAddTag(TAG_GET_DEPTH);
		RPI_PropertyProcess();

		if ((mp = RPI_PropertyGet(TAG_GET_PHYSICAL_SIZE)))
		{
			vga_width = mp->data.buffer_32[0];
			vga_height = mp->data.buffer_32[1];
		}

		if ((mp = RPI_PropertyGet(TAG_GET_DEPTH)))
			vga_bpp = mp->data.buffer_32[0];

		if ((mp = RPI_PropertyGet(TAG_GET_PITCH)))
			vga_pitch = mp->data.buffer_32[0];

		if ((mp = RPI_PropertyGet(TAG_ALLOCATE_BUFFER)))
			vga_framebuffer = (unsigned char*)(mp->data.buffer_32[0] & 0x3FFFFFFF);
	}
	while (vga_framebuffer == 0);

	//RPI_PropertyInit();
	//RPI_PropertyAddTag(TAG_SET_PALETTE, palette);
	//RPI_PropertyProcess();

	switch (vga_bpp)
	{
		case 32:
			vga_plotpixelFn = vga_plotpixel32;
		break;
		case 24:
			vga_plotpixelFn = vga_plotpixel24;
		break;
		default:
		case 16:
			vga_plotpixelFn = vga_plotpixel16;
		break;
		case 8:
			vga_plotpixelFn = vga_plotpixel8;
		break;
	}
}

void vga_release()
{
	vga_framebuffer = 0;

	RPI_PropertyInit();
	RPI_PropertyAddTag(TAG_RELEASE_BUFFER, vga_framebuffer);
	RPI_PropertyProcessNoCheck();
}


void vga_setforecolor(RGBA color)
{
	vga_current_fg_color = color;
}

void vga_setbackcolor(RGBA color)
{
	vga_current_bg_color = color;
}

void vga_cursor_on()
{
	vga_cursor_mode = 1;
}

void vga_cursor_off()
{
	vga_cursor_mode = 0;
}

void vga_plotpixel32(u32 pixel_offset)
{
	*((volatile RGBA*)&vga_framebuffer[pixel_offset]) = vga_current_fg_color;
}

void vga_plotpixel24(u32 pixel_offset)
{
	vga_framebuffer[pixel_offset++] = BLUE(vga_current_fg_color);
	vga_framebuffer[pixel_offset++] = GREEN(vga_current_fg_color);
	vga_framebuffer[pixel_offset++] = RED(vga_current_fg_color);
}

void vga_plotpixel16(u32 pixel_offset)
{
	*(unsigned short*)&vga_framebuffer[pixel_offset] = ((RED(vga_current_fg_color) >> 3) << 11) | ((GREEN(vga_current_fg_color) >> 2) << 5) | (BLUE(vga_current_fg_color) >> 3);
}

void vga_plotpixel8(u32 pixel_offset)
{
	vga_framebuffer[pixel_offset++] = RED(vga_current_fg_color);
}

void vga_plotpixel(u32 x, u32 y)
{
	if (x < 0 || y < 0 || x >= vga_width || y >= vga_height)
		return;
	int pixel_offset = (x * (vga_bpp >> 3)) + (y * vga_pitch);
	(*vga_plotpixelFn)(pixel_offset);
}

void vga_clear()
{
	vga_cleararea(0, 0, vga_width, vga_height);
}

void vga_swapcolors()
{
	u32 tmp_color = vga_current_fg_color;
	vga_current_fg_color = vga_current_bg_color;
	vga_current_bg_color = tmp_color;
}

void vga_cleararea(u32 x1, u32 y1, u32 x2, u32 y2)
{
	u32 tmp_color = vga_current_fg_color;
	vga_current_fg_color = vga_current_bg_color;
	
	vga_cliprect(x1, y1, x2, y2);

	for (u32 y = y1; y < y2; y++)
	{
		u32 line = y * vga_pitch;
		for (u32 x = x1; x < x2; x++)
		{
			u32 pixel_offset = (x * (vga_bpp >> 3)) + line;
			(*vga_plotpixelFn)(pixel_offset);
		}
	}
	
	vga_current_fg_color = tmp_color;
}

void vga_scroll()
{
	u8 *fb = vga_framebuffer;
	int line_byte_width = vga_width * (vga_bpp >> 3);

	for(int line = 0; line < (vga_height - vga_font_height); line++)
		memcpy(&fb[line * vga_pitch], &fb[(line + vga_font_height) * vga_pitch], line_byte_width);
	
	vga_cleararea(0,vga_height-vga_font_height,vga_width,vga_height);
}



void vga_drawchar(u32 x, u32 y, unsigned char c)
{
	u32 fontHeight;
	const unsigned char* fontBitMap;

	fontBitMap = vga_current_font;
	fontHeight = vga_font_height;

	vga_cleararea(x,y,x+vga_font_width,y+vga_font_height);
	
	for (u32 py = 0; py < fontHeight; ++py)
	{
		if (y + py > vga_height)
			return;

		unsigned char b = fontBitMap[c * fontHeight + py];
		int yoffs = (y + py) * vga_pitch;
		for (int px = 0; px < 8; ++px)
		{
			if (x + px >= vga_width)
				continue;

			int pixel_offset = ((px + x) * (vga_bpp >> 3)) + yoffs;
			if ((b & 0x80) == 0x80)
				(*vga_plotpixelFn)(pixel_offset);
			b = b << 1;
		}
	}
}

u32 vga_drawtext(u32 x, u32 y, char *ptr)
{
	int xCursor = x;
	int yCursor = y;
	int len = 0;
	u32 fontHeight;

	fontHeight = vga_font_height;

	while (*ptr != 0)
	{
		char c = *ptr++;
		if ((c != '\r') && (c != '\n'))
		{
			vga_cleararea(xCursor, yCursor, xCursor + vga_font_width, yCursor + fontHeight);
			vga_drawchar(xCursor, yCursor, c);
			
			xCursor += vga_font_width;
		}
		else
		{
			xCursor = x;
			yCursor += fontHeight;
		}
		len++;
	}
	return len;
}

void vga_drawcursor(u32 x, u32 y)
{
	y++;
	for(int y2=0;y2<vga_font_height-1;y2++)
		vga_drawline(x+1, y+y2, x+vga_font_width-1,y+y2);
}

void vga_erasecursor(u32 x, u32 y)
{
	vga_cleararea(x, y, x + vga_font_width, y + vga_font_height);
}

void vga_drawline(u32 x1, u32 y1, u32 x2, u32 y2)
{
	vga_cliprect(x1, y1, x2, y2);

	int dx0, dy0, ox, oy, eulerMax;
	dx0 = (int)(x2 - x1);
	dy0 = (int)(y2 - y1);
	eulerMax = abs(dx0);
	if (abs(dy0) > eulerMax) eulerMax = abs(dy0);
	for (int i = 0; i <= eulerMax; i++)
	{
		ox = ((dx0 * i) / eulerMax) + x1;
		oy = ((dy0 * i) / eulerMax) + y1;
		int pixel_offset = (ox * (vga_bpp >> 3)) + (oy * vga_pitch);
		(*vga_plotpixelFn)(pixel_offset);
	}
}

void vga_drawlinev(u32 x, u32 y1, u32 y2)
{
	vga_cliprect(x, y1, x, y2);
	for (u32 y = y1; y <= y2; ++y)
	{
		int pixel_offset = (x * (vga_bpp >> 3)) + (y * vga_pitch);
		(*vga_plotpixelFn)(pixel_offset);
	}
}

void vga_drawrect(u32 x0, u32 y0, u32 w, u32 h) 
{
    vga_drawline(x0, y0, x0 + w, y0); // top
    vga_drawline(x0, y0, x0, y0 + h); // left
    vga_drawline(x0, y0 + h, x0 + w, y0 + h); // bottom
    vga_drawline(x0 + w, y0, x0 + w, y0 + h); // right
}

void vga_drawcircle(u32 x0, u32 y0, u32 r)
{
    u32 x = r;
    u32 y = 0;
    u32 radiusError = 1 - x;

    while(x >= y)
    {
        // top left
        vga_plotpixel(-y + x0, -x + y0);
        // top right
        vga_plotpixel(y + x0, -x + y0);
        // upper middle left
        vga_plotpixel(-x + x0, -y + y0);
        // upper middle right
        vga_plotpixel(x + x0, -y + y0);
        // lower middle left
        vga_plotpixel(-x + x0, y + y0);
        // lower middle right
        vga_plotpixel(x + x0, y + y0);
        // bottom left
        vga_plotpixel(-y + x0, x + y0);
        // bottom right
        vga_plotpixel(y + x0, x + y0);

        y++;
        if (radiusError < 0)
        {
            radiusError += 2 * y + 1;
        } else {
            x--;
            radiusError+= 2 * (y - x + 1);
        }
    }
}

/*void vga_plotimage(u32* image, int x, int y, int w, int h)
{
	int px;
	int py;
	int i = 0;
	
	RGBA tempcolor = vga_current_fg_color;
	
	for (py = 0; py < h; ++py)
	{
		for (px = 0; px < w; ++px)
		{
			vga_setforecolor(image[i++]);
			vga_plotpixel(x + px, y + py);
		}
	}
	
	vga_current_fg_color = tempcolor;
}*/

void vga_cliprect(u32 x1, u32 y1, u32 x2, u32 y2)
{
	if (x1 > vga_width) x1 = vga_width;
	if (y1 > vga_height) y1 = vga_height;
	if (x2 > vga_width) x2 = vga_width;
	if (y2 > vga_height) y2 = vga_height;
}

