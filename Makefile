# Name: Makefile
# Author: Michael Lieschnegg
# Copyright: <insert your copyright message here>
# License: <insert your license reference here>

# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected.
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

DEVICE     = atmega328p
CLOCK      = 16000000

ifeq ($(OS),Windows_NT)
	PROGRAMMER = -c arduino -P COM10 -b 115200 
else
	PROGRAMMER = -c arduino -P /dev/ttyACM0 -b 115200 
endif

SRCS=$(wildcard *.c *.s)
SRCS_C = $(wildcard *.c)
SRCS_S = $(wildcard *.s)
OBJECTS=$(SRCS_C:.c=.o) $(SRCS_S:.s=.o)
#OBJECTS    = main.o

FUSES      = -U lfuse:w:0x64:m -U hfuse:w:0xdd:m -U efuse:w:0xff:m


######################################################################
######################################################################

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -std=c99 -Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm

# symbolic targets:
all:	main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	@echo $(OBJECTS)
	@echo $(SRCS)
	rm -f main.hex main.elf $(OBJECTS)

# file targets:
main.elf: $(OBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	main.o
	avr-objdump -d main.o

cpp:
	$(COMPILE) -E main.c

hterm:
ifeq ($(OS),Windows_NT)
#	start "" "C:/Users/micha/OneDrive - HTBLA Kaindorf/Arbeit/HTL-Kaindorf/Lern_und_Lehrunterlagen/FIVU/Git-FIVU/Tools/hterm.exe"
	../../tools/hterm.exe ../../tools/hterm_win.cfg
else
	../../tools/./hterm ../../tools/hterm_linux.cfg
endif