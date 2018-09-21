OBJS	= armc-start.o armc-cstartup.o armc-cstubs.o armc-cppstubs.o \
	exception.o main.o rpi-aux.o rpi-i2c.o rpi-mailbox-interface.o rpi-mailbox.o \
	rpi-gpio.o rpi-interrupts.o cache.o ff.o interrupt.o Keyboard.o \
	emmc.o diskio.o vga.o terminal.o timer.o font_data.o basic.o linkedlist.o expr.o

SRCDIR  	= src
TARGETDIR	= target
OBJS    	:= $(addprefix $(SRCDIR)/, $(OBJS))

LIBS     = uspi/libuspi.a
INCLUDE  = -Iinclude/ -Iuspi/include/

TARGET  ?= kernel

.PHONY: all $(LIBS)

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	@echo "  LINK $@"
	$(Q)$(CC) $(CFLAGS) -lm -o $(TARGETDIR)/$(TARGET).elf -Xlinker -Map=$(TARGETDIR)/$(TARGET).map -T linker.ld -nostartfiles $(OBJS) $(LIBS)
	$(Q)$(PREFIX)objdump -d $(TARGETDIR)/$(TARGET).elf | $(PREFIX)c++filt > $(TARGETDIR)/$(TARGET).lst
	$(Q)$(PREFIX)objcopy $(TARGETDIR)/$(TARGET).elf -O binary $(TARGETDIR)/$(TARGET).img
	cp $(OBJS) obj
	$(Q)$(RM) $(OBJS)
	
uspi/libuspi.a:
	$(MAKE) -C uspi

clean:
	$(Q)$(RM) $(OBJS) $(TARGETDIR)/$(TARGET).elf $(TARGETDIR)/$(TARGET).map $(TARGETDIR)/$(TARGET).lst $(TARGETDIR)/$(TARGET).img
	$(Q)$(RM) obj/*.o
	$(MAKE) -C uspi clean

include Makefile.rules