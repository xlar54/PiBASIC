#ifndef MAIN_H
#define MAIN_H

extern "C"
{
#include "types.h"
#include "rpi-defs.h"
#include "rpi-base.h"
#include "rpi-mailbox-interface.h"
#include "rpi-mailbox.h"
#include "rpi-gpio.h"
#include "rpi-aux.h"
#include "rpi-hardware.h"
#include "interrupt.h"
#include "timer.h"
#include "emmc.h"
#include "diskio.h"
#include "ff.h"
#include "vga.h"

#include <string.h>
#include <strings.h>
}
#include "Keyboard.h"

float vga_scaleX;
float vga_scaleY;
u32 vga_width;
u32 vga_height;
u32 vga_bpp;
u32 vga_pitch;
u8* vga_framebuffer;
funcptr vga_plotpixelFn;

extern "C"
{
	void kernel_main(unsigned int r0, unsigned int r1, unsigned int atags);
	static void start_core(int core, func_ptr func);
	void run_core();
	void core0();
	void core1();
	void core2();
	void core3();
}


#endif