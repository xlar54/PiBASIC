#include "cmd.h"

#define MAXBANKS	7
#define MAXRAM		65535

extern "C"
{

// 512k system
uint8_t mem[MAXBANKS+1][MAXRAM+1] = {0};


void cmd_cursorblink_handler(unsigned hTimer, void *pParam, void *pContext)
{
	term_toggle_cursor();
	uint32_t cursorTimer = TimerStartKernelTimer(30, cmd_cursorblink_handler, 0, (void *)cursorTimer);
}

void cmd_handleinput(Keyboard* keyboard)
{
	uint32_t cursorTimer = TimerStartKernelTimer(30, cmd_cursorblink_handler, 0, (void *)cursorTimer);
	
	term_printf("\n                 The Raspberry Pi BASIC Development System");
	term_printf("\n                       Operating System Version 1.0");
	term_printf("\n                               %dK System", (MAXBANKS * MAXRAM / 1024));
	term_printf("\n                    Screen resolution: %d x %d - %dbpp", vga_width, vga_height, vga_bpp);
	term_printf("\n\nReady.");
	
	
	vga_cursor_on();
	term_printf("\n");
	
	char inbuffer[160];
	int inbufferctr = 0;
	uint32_t blinkctr = 0;
	
	while (1)
	{	
		if(keyboard->CheckChanged())
		{
			char c = keyboard->GetChar();
			
			if(c != 0)
			{
				term_putc(c);
				
				if(c == '\n')
				{
					inbuffer[inbufferctr++] = 0;
					inbufferctr = 0;
					
					if (inbuffer[0] >= '0' && inbuffer[0] <= '9')
					{
						//cmd_stashline(inbuffer);
					}
					
					if(strcmp(inbuffer,"help") == 0)
					{
						cmd_help();
					}
					if(strcmp(inbuffer,"dir") == 0)
					{
						cmd_displaydir();
					}
					if(strcmp(inbuffer,"run") == 0)
					{
					}
					if(strcmp(inbuffer,"key") == 0)
					{
						while(1)
						{
							if(keyboard->CheckChanged())
							{
								char c = keyboard->GetChar();
								if(c != 0)
								{
									term_printf("KEY:%c - %d\n",c,c);
									
									if(c==10)
										break;
								}
							}
						}
					}
					if(strncmp(inbuffer, "cd ", 3) == 0)
					{
						int t = 0;
						while(inbuffer[t] != 0)
						{
							inbuffer[0+t] = inbuffer[3+t];
							t++;
						}
						inbuffer[t] = 0;
						
						cmd_changedir(inbuffer);
					}
					if(strncmp(inbuffer, "mkdir ", 6) == 0)
					{
						int t = 0;
						while(inbuffer[t] != 0)
						{
							inbuffer[0+t] = inbuffer[6+t];
							t++;
						}
						inbuffer[t] = 0;
						
						cmd_makedir(inbuffer);
					}
					if(strncmp(inbuffer, "more ", 5) == 0)
					{
						int t = 0;
						while(inbuffer[t] != 0)
						{
							inbuffer[0+t] = inbuffer[6+t];
							t++;
						}
						inbuffer[t] = 0;
						
						cmd_more(inbuffer);
					}
					if(strncmp(inbuffer, "peek ", 5) == 0)
					{
						int t = 0;
						while(inbuffer[t] != 0)
						{
							inbuffer[0+t] = inbuffer[5+t];
							t++;
						}
						inbuffer[t] = 0;
						
						uint8_t *c;
						uint32_t col = atoi(inbuffer);
						cmd_peek(col, 1, c);
						term_printf("\ncols=%s, col=%d, char='%c' - %d\n\n", inbuffer, col, *c, *c);
					}
					if(strncmp(inbuffer, "poke ", 5) == 0)
					{
						int t = 0;
						while(inbuffer[t] != 0)
						{
							inbuffer[0+t] = inbuffer[5+t];
							t++;
						}
						inbuffer[t] = 0;
						
						uint8_t *c;
						uint32_t col = atoi(inbuffer);
						cmd_poke(col, 1, 65);
					}
					if(strcmp(inbuffer,"hires") == 0)
					{
						vga_release();
						vga_init(1024, 768, 16);
						vga_clear();
					}
					if(strcmp(inbuffer,"medres") == 0)
					{
						vga_release();
						vga_init(640, 400, 16);
						vga_clear();
					}
					if(strcmp(inbuffer,"lores") == 0)
					{
						vga_release();
						vga_init(320, 200, 16);
						vga_clear();
					}
					if(strcmp(inbuffer,"circle") == 0)
					{
						vga_setforecolor(COLOUR_GREEN);
						vga_drawcircle(100,100,20);
						vga_setforecolor(COLOUR_CYAN);
					}
					
					term_printf("Ready.\n");
				}
				else if(c == 8)
				{
					if(inbuffer > 0)
					{
						inbufferctr--;
						inbuffer[inbufferctr] = 0;
					}
				}
				else
				{
					inbuffer[inbufferctr++] = c;
				}
			}
			
		}
	}
}

void cmd_help()
{
	term_printf("Help:\n");
	term_printf(" help            - this info\n");
	term_printf(" dir             - list the files on the current path\n");
	term_printf(" cd directory    - change directory\n");
	term_printf(" mkdir directory - make directory\n");
	term_printf(" rmdir directory - delete directory\n");
	term_printf(" more filename   - display file contents\n");
	term_printf("\n");
}

void cmd_peek(uint8_t col, uint8_t row, uint8_t* ch)
{
	term_getcharat(col, row, ch);
}

void cmd_poke(uint8_t col, uint8_t row, uint8_t ch)
{
	term_putcharat(col, row, ch);
}

void cmd_displaydir()
{
	term_printf("\nFiles:\n");
	
	DIR dir;
	FILINFO filImage;
	FRESULT res;
	char* ext;
	
	res = f_opendir(&dir, ".");
	if (res == FR_OK)
	{
		do 
		{
			res = f_readdir(&dir, &filImage);
			ext = strrchr(filImage.fname, '.');
			if (res == FR_OK && filImage.fname[0] != 0 && !(ext && strcasecmp(ext, ".png") == 0))
			{
				term_printf("%s\n",filImage.fname);
			}
		}
		while (res == FR_OK && filImage.fname[0] != 0);
		
		f_closedir(&dir);
	}
	else
		term_printf("\n?Disk read error.");

	term_printf("\n");
}

void cmd_changedir(char *directory)
{
	FRESULT res;

	res = f_chdir(directory);
	
	if(res == FR_NO_PATH)
		term_printf("\n?Invalid Path Error.");
	
	term_printf("\n");

}

void cmd_makedir(char *directory)
{
	FRESULT res;

	res = f_mkdir(directory);
	
	if(res != FR_OK)
		term_printf("\n?Directory Creation Error #%d", res);
	
	term_printf("\n");
}

void cmd_rmdir(char *directory)
{
	FRESULT res;

	res = f_unlink(directory);
	
	if(res != FR_OK)
		term_printf("\n?Directory Deletion Error #%d", res);
	
	term_printf("\n");
}

void cmd_more(char *filename)
{
	FIL fp;
	FRESULT res;
	UINT bytesRead;
	BYTE b[2];
	
	res = f_open(&fp, filename, FA_READ);
	
	if (res == FR_OK)
	{
		term_printf("File contents:\n");
		
		do 
		{
			res = f_read(&fp, b, 1, &bytesRead);
			
			term_printf("%c", b[0]);
		}
		while (res == FR_OK && bytesRead == 1);
		
		f_close(&fp);
	}
	else
		term_printf("\n?File Open Error #%d", res);
	
	term_printf("\n");
}

}

