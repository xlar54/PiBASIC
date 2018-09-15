#ifndef CMD_H
#define CMD_H

#include "Keyboard.h"

extern "C"
{
#include "types.h"
#include "emmc.h"
#include "diskio.h"
#include "ff.h"
#include "vga.h"
#include "terminal.h"
#include <string.h>
#include <strings.h>
#include "rpi-gpio.h"
#include "rpi-hardware.h"
#include "timer.h"


void cmd_handleinput(Keyboard *keyboard);
void cmd_help();
void cmd_peek(uint8_t row, uint8_t col, uint8_t* ch);
void cmd_poke(uint8_t row, uint8_t col, uint8_t ch);
void cmd_displaydir();
void cmd_changedir(char *directory);
void cmd_makedir(char *directory);
void cmd_rmdir(char *directory);
void cmd_more(char *filename);

}

#endif