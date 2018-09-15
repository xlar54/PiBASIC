
#include "main.h"
#include "terminal.h"
#include "basic.h"

extern "C"
{
#include "rpi-aux.h"
#include "rpi-i2c.h"
#include "rpi-gpio.h"
#include "startup.h"
#include "cache.h"
#include "rpi-mailbox-interface.h"
#include "interrupt.h"
#include <uspi.h>
}

const long int tempBufferSize = 1024;
char tempBuffer[tempBufferSize];

CEMMCDevice	m_EMMC;

unsigned int screenWidth = 640;
unsigned int screenHeight = 400;
unsigned int screenDepth = 16;

// Hooks required for USPi library
extern "C"
{
	void LogWrite(const char *pSource, unsigned Severity, const char *pMessage, ...)
	{
		va_list args;
		va_start(args, pMessage);
		vprintf(pMessage, args);
		va_end(args);
	}

	int GetMACAddress(unsigned char Buffer[6])
	{
		rpi_mailbox_property_t* mp;

		RPI_PropertyInit();
		RPI_PropertyAddTag(TAG_GET_BOARD_MAC_ADDRESS);
		RPI_PropertyProcess();

		if ((mp = RPI_PropertyGet(TAG_GET_BOARD_MAC_ADDRESS)))
		{
			for (int i = 0; i < 6; ++i)
			{
				Buffer[i] = mp->data.buffer_8[i];
			}
			return 1;
		}

		return 0;
	}

	int SetPowerStateOn(unsigned id)
	{
		volatile u32* mailbox;
		u32 result;

		mailbox = (u32*)(PERIPHERAL_BASE | 0xB880);
		while (mailbox[6] & 0x80000000);
		mailbox[8] = 0x80;
		do {
			while (mailbox[6] & 0x40000000);
		} while (((result = mailbox[0]) & 0xf) != 0);
		return result == 0x80;
	}

	void usDelay(unsigned nMicroSeconds)
	{
		unsigned before;
		unsigned after;
		for (u32 count = 0; count < nMicroSeconds; ++count)
		{
			before = read32(ARM_SYSTIMER_CLO);
			do
			{
				after = read32(ARM_SYSTIMER_CLO);
			} while (after == before);
		}
	}

	void MsDelay(unsigned nMilliSeconds)
	{
		usDelay(nMilliSeconds * 1000);
	}

	unsigned StartKernelTimer(unsigned nHzDelay, TKernelTimerHandler* pHandler, void* pParam, void* pContext)
	{
		return TimerStartKernelTimer(nHzDelay, pHandler, pParam, pContext);
	}

	void CancelKernelTimer(unsigned hTimer)
	{
		TimerCancelKernelTimer(hTimer);
	}

	typedef void TInterruptHandler(void* pParam);

	// USPi uses USB IRQ 9
	void ConnectInterrupt(unsigned nIRQ, TInterruptHandler* pHandler, void *pParam)
	{
		InterruptSystemConnectIRQ(nIRQ, pHandler, pParam);
	}
}

// Hooks for FatFs
// If you have hardware RTC return a correct value here. 
// THis can then be reflected in file modification times/dates.
DWORD get_fattime() { return 0; }	
Keyboard* keyboard;

void InitializeHardware()
{
#if defined(RPI3)
	RPI_GpioVirtInit();
	RPI_TouchInit();
#endif

	vga_init(screenWidth, screenHeight, screenDepth);

	RPI_PropertyInit();
	RPI_PropertyAddTag(TAG_GET_MAX_CLOCK_RATE, ARM_CLK_ID);
	RPI_PropertyProcess();

	rpi_mailbox_property_t* mp;
	u32 MaxClk = 0;
	if ((mp = RPI_PropertyGet(TAG_GET_MAX_CLOCK_RATE)))
	{
		MaxClk = mp->data.buffer_32[1];
	}
	RPI_PropertyInit();
	RPI_PropertyAddTag(TAG_SET_CLOCK_RATE, ARM_CLK_ID, MaxClk);
	RPI_PropertyProcess();
}

extern "C"
{
	void kernel_main(unsigned int r0, unsigned int r1, unsigned int atags)
	{
		FATFS fileSystem;

		disk_setEMM(&m_EMMC);
		m_EMMC.Initialize();
		f_mount(&fileSystem, "", 1);

		//RPI_AuxMiniUartInit(115200, 8);

		InitializeHardware();
		enable_MMU_and_IDCaches();
		_enable_unaligned_access();

		write32(ARM_GPIO_GPCLR0, 0xFFFFFFFF);

		InterruptSystemInitialize();
		TimerSystemInitialize();
		USPiInitialize();

		if (!USPiKeyboardAvailable())
		{
			term_printf("\nKeyboard not found.\bSystem Halted.");
			while(1);
		}

		keyboard = Keyboard::Instance();
		term_init(keyboard);
		
#ifdef HAS_MULTICORE
		start_core(3, _spin_core);	// Do nothing with this core
		start_core(2, _spin_core);	// Do nothing with this core
#ifdef USE_MULTICORE
		start_core(1, _init_core);
		core0();					// Start core0 code
#else
		start_core(1, _spin_core);	// If not using multicore, do nothing
#endif
#endif
#ifndef USE_MULTICORE
		core1();					// Only one core, so just call the code
#endif
	}
	
	void core0()
	{
		//cmd_handleinput(keyboard);
		
		term_showcursor();
		
		basic_main();
		
		while(1)
		{
			uint8_t c = term_getchar();
			if(c != 0)
				term_putchar(c);
		}
		
		char* IL = (char *)0;

		term_printf("\n                 The Raspberry Pi BASIC Development System");
		term_printf("\n                       Operating System Version 1.0");
		term_printf("\n                               512K System");
		term_printf("\n                    Screen resolution: %d x %d - %dbpp", vga_width, vga_height, vga_bpp);
		term_printf("\n\nReady.\n");

		//StartTinyBasic(IL);
		
	}

	void core1()
	{
		// future use
		while(1);
	}


#ifdef HAS_MULTICORE
	void run_core() 
	{
		enable_MMU_and_IDCaches();
		_enable_unaligned_access();

		DEBUG_LOG("core %d starting...\r\n", _get_core());
		
		core1();
	}

	static void start_core(int core, func_ptr func)
	{
		write32(0x4000008C + 0x10 * core, (unsigned int)func);
		__asm ("SEV");	// and wake it up.
		
		DEBUG_LOG("core %d starting...\r\n", core);
	}
#endif
}


