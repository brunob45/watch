
#####         Target Specific Details          #####
#####     Customize these for your project     #####

# Name of target controller
# (e.g. 'at90s8515', see the available avr-gcc mmcu
# options for possible values)
MCU=attiny88

F_CPU = 128000
F_TWI = 100000

# Name of our project
# (use a single word, e.g. 'myproject')
PROJECTNAME=Montre
BINDIR=bin
SRCDIR=src

# Source files
# List C/C++/Assembly source files:
# (list all files to compile, e.g. 'a.c b.cpp as.S'):
# Use .cc, .cpp or .C suffix for C++ files, use .S
# (NOT .s !!!) for assembly source code files.
PRJSRC=$(wildcard $(addprefix $(SRCDIR)/, *.c *.cpp *.cc *.C *.S))
#$(info $$PRJSRC is [${PRJSRC}])
# additional includes (e.g. -I/path/to/mydir)
INC=-I./include

# libraries to link in (e.g. -lmylib)
LIBS=

# Optimization level,
# use s (size opt), 1, 2, 3 or 0 (off)
OPTLEVEL=s


#####      AVR Dude 'writeflash' options       #####
#####  If you are using the avrdude program
#####  (http://www.bsdhome.com/avrdude/) to write
#####  to the MCU, you can set the following config
#####  options and use 'make writeflash' to program
#####  the device.


# programmer id--check the avrdude for complete list
# of available opts.  These should include stk500,
# avr910, avrisp, bsd, pony and more.  Set this to
# one of the valid "-c PROGRAMMER-ID" values
# described in the avrdude info page.
#
AVRDUDE_PROGRAMMERID=avrisp #usbasp

# port--serial or parallel port to which your
# hardware programmer is attached
#
AVRDUDE_PORT=COM3 #/dev/ttyS0


####################################################
#####                Config Done               #####
#####                                          #####
##### You shouldn't need to edit anything      #####
##### below to use the makefile but may wish   #####
##### to override a few of the flags           #####
##### nonetheless                              #####
#####                                          #####
####################################################


##### Flags ####

# HEXFORMAT -- format for .hex file output
HEXFORMAT=ihex

# compiler
CFLAGS=-I. $(INC) -mmcu=$(MCU) -O$(OPTLEVEL) \
	-fpack-struct -fshort-enums             \
	-funsigned-bitfields -funsigned-char    \
	-Wall                                      \
	-Wa,-ahlms=$(firstword                  \
	$(filter %.lst, $(<:$(SRCDIR)/%.c=%.lst))) \
	-DF_CPU=$(F_CPU) \
	-DF_TWI=$(F_TWI) \
#	-g -DF_XTL=$(F_XTL) \
	-DF_DIV=$(F_DIV) \
# c++ specific flags
CPPFLAGS=-fno-exceptions -mint8  \
	-Wa,-ahlms=$(BINDIR)/$(firstword         \
	$(filter %.lst, $(<:$(SRCDIR)/%.cpp=%.lst))\
	$(filter %.lst, $(<:$(SRCDIR)/.cc=.lst)) \
	$(filter %.lst, $(<:$(SRCDIR)/.C=.lst))) \
	# -std=c++11

# assembler
ASMFLAGS =-I. $(INC) -mmcu=$(MCU)        \
	-x assembler-with-cpp            \
	-Wa,-gstabs,-ahlms=$(firstword   \
		$(<:.S=.lst) $(<.s=.lst))


# linker
LDFLAGS=-Wl,-Map,$(TRG).map -mmcu=$(MCU)

##### executables ####
CC=avr-gcc
OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump
SIZE=avr-size
#AVRDUDE=/home/robot/avrdude/opt/avr/bin/avrdude
AVRDUDE=avrdude
REMOVE=rm -f

##### automatic target names ####
TRG=$(BINDIR)/$(PROJECTNAME).out
DUMPTRG=$(PROJECTNAME).s

HEXROMTRG=$(BINDIR)/$(PROJECTNAME).hex
HEXTRG=$(HEXROMTRG) $(BINDIR)/$(PROJECTNAME).ee.hex
GDBINITFILE=gdbinit-$(PROJECTNAME)

# Define all object files.

# Start by splitting source files by type
#  C++
CPPFILES=$(filter %.cpp, $(PRJSRC))
CCFILES=$(filter %.cc, $(PRJSRC))
BIGCFILES=$(filter %.C, $(PRJSRC))
#  C
CFILES=$(filter %.c, $(PRJSRC))
#  Assembly
ASMFILES=$(filter %.S, $(PRJSRC))


# List all object files we need to create
OBJDEPS=$(addprefix $(BINDIR)/, $(CFILES:$(SRCDIR)/%.c=%.o)    \
	$(CPPFILES:$(SRCDIR)/%.cpp=%.o)\
	$(BIGCFILES:.C=.o) \
	$(CCFILES:.cc=.o)  \
	$(ASMFILES:.S=.o) )

# Define all lst files.
LST=$(filter %.lst, $(OBJDEPS:.o=.lst))
#$(info $$LST is [${LST}])

# All the possible generated assembly
# files (.s files)
GENASMFILES=$(filter %.s, $(OBJDEPS:.o=.s))

.SUFFIXES : .c .cc .cpp .C .o .out .s .S \
	.hex .ee.hex .h .hh .hpp


.PHONY: writeflash clean stats gdbinit stats

# Make targets:
# all, disasm, stats, hex, writeflash/install, clean
all: $(TRG)

disasm: $(DUMPTRG) stats

stats: $(TRG)
	$(OBJDUMP) -h $(TRG)
	$(SIZE) $(TRG)

hex: $(HEXROMTRG)


writeflash: hex
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMERID)   \
	 -p $(MCU) -P $(AVRDUDE_PORT) -e        \
	 -U flash:w:$(HEXROMTRG)

install: writeflash

$(DUMPTRG): $(TRG)
	$(OBJDUMP) -d  $< > $@


$(TRG): $(OBJDEPS)
	$(CC) $(LDFLAGS) -o $(TRG) $(OBJDEPS) \
	-lm $(LIBS)


#### Generating assembly ####
# asm from C
$(BINDIR)/%.s: $(SRCDIR)/%.c
	$(CC) -S $(CFLAGS) $< -o $@

# asm from (hand coded) asm
%.s: %.S
	$(CC) -S $(ASMFLAGS) $< > $@


# asm from C++
#.cpp.s .cc.s .C.s :
$(BINDIR)/%.s : $(SRCDIR)/%.cpp
	$(CC) -S $(CFLAGS) $(CPPFLAGS) $< -o $@



#### Generating object files ####
# object from C
$(BINDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


# object from C++ (.cc, .cpp, .C files)
#.cc.o .cpp.o .C.o :
#$(OBJDEPS) : .cpp
$(BINDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# object from asm
.S.o :
	$(CC) $(ASMFLAGS) -c $< -o $@


#### Generating hex files ####
# hex files from elf
#####  Generating a gdb initialisation file    #####
.out.hex:
	$(OBJCOPY) -j .text                    \
		-j .data                       \
		-O $(HEXFORMAT) $< $@

.out.ee.hex:
	$(OBJCOPY) -j .eeprom                  \
		--change-section-lma .eeprom=0 \
		-O $(HEXFORMAT) $< $@


#####  Generating a gdb initialisation file    #####
##### Use by launching simulavr and avr-gdb:   #####
#####   avr-gdb -x gdbinit-myproject           #####
gdbinit: $(GDBINITFILE)

$(GDBINITFILE): $(TRG)
	@echo "file $(TRG)" > $(GDBINITFILE)

	@echo "target remote localhost:1212" \
		                >> $(GDBINITFILE)

	@echo "load"        >> $(GDBINITFILE)
	@echo "break main"  >> $(GDBINITFILE)
	@echo "continue"    >> $(GDBINITFILE)
	@echo
	@echo "Use 'avr-gdb -x $(GDBINITFILE)'"


#### Cleanup ####
clean:
	$(REMOVE) $(TRG) $(TRG).map $(DUMPTRG)
	$(REMOVE) $(OBJDEPS)
	$(REMOVE) $(LST) $(GDBINITFILE)
	$(REMOVE) $(GENASMFILES)
	$(REMOVE) $(HEXTRG)
	
fuses:
	avrdude -c ${AVRDUDE_PROGRAMMERID} -p $(MCU) -P ${AVRDUDE_PORT} \
	  -U hfuse:w:0xD9:m -U lfuse:w:0xEF:m \
      -U efuse:w:0x07:m 
#programmed e: 1111 1111      h: 1101 1111      l: 1110 1111
#default    e: 1111 1111      h: 1101 1111      l: 0110 1110

# l7 : CKDIV8 (0)
# l6 : CKOUT (1)
# l5 : SUT1 (1)
# l4 : SUT0 (0)
# l1 : CKSEL1 (1)
# l0 : CKSEL0 (0)
#
#

#####                    EOF                   #####