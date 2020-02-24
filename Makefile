# Functions needed by this makefile and the one that builds the source libraries (MakefileSrc)
export ECHO = @echo
export RM = rm
export SED = sed
export MKDIR = mkdir -p
export RMDIR = rmdir
export TR = tr
export BLACKHOLE = /dev/null
export PWD = pwd
export CD = cd
export LS = ls

PROJECT = nrf24le1-sensor

TARGET = _target_sdcc_nrf24le1_32
MEMORYMODEL = --model-large

# Programs to use for creating dependencies, compiling source files, and creating the library file, respectively
DEP = sdcc
CC  = sdcc
LD	= sdld
LIB = sdcclib
HEX2BIN = ../hex2bin/hex2bin

BUILD := build
OBJDIR := $(BUILD)/obj
DEPDIR := $(BUILD)/dep
LIBDIR := $(BUILD)/lib

SRCFILES = crc8.c \
           bme280.c \
           ds18b20.c \
           bh1750.c \
		   radio.c \
           dht.c \
		   main.c

DEPFILES := $(subst .c,.d,$(addprefix $(DEPDIR)/,$(SRCFILES)))
OBJFILES := $(SRCFILES:.c=.rel)
OBJFILES := $(addprefix $(OBJDIR)/,$(OBJFILES))
LIBFILES := $(SRCFILES:.c=.lib)
LIBFILES := $(addprefix $(LIBDIR)/,$(LIBFILES))
LDFILES := $(SRCFILES:.c=)
LDFILES := $(addprefix -l,$(LDFILES))

LDSDCC = -L/usr/local/share/sdcc/lib/large -lmcs51 -llibsdcc
LDNRF = -Lsdk/$(TARGET)/lib -lnrf24le1
LDtinyAES = -Ltiny-AES128/lib -laes

LIBNRF = sdk/$(TARGET)/lib/nrf24le1.lib
LIBtinyAES = tiny-AES128/lib/aes.lib

DFLAGS=-MM -Isdk/include -Isdk/$(TARGET)/include/ -Itiny-AES128/include/
CCFLAGS=$(MEMORYMODEL) --std-c99 --opt-code-size -Isdk/include -Isdk/$(TARGET)/include/ -Itiny-AES128/include/
LDFLAGS=--code-loc 0x0000 --code-size 0x4000 --xram-loc 0x0000 --xram-size 0x400 $(LDSDCC) $(LDNRF) $(LDtinyAES) -L$(LIBDIR) $(LDFILES)
LFLAGS =

PROJECTBIN = $(BUILD)/$(PROJECT).bin
CRC8CALC = tools/crc8calc

all: $(LIBNRF) $(LIBtinyAES) $(PROJECTBIN)

-include $(DEPFILES)

$(DEPDIR)/%.d: %.c %.h
	$(ECHO)
	$(ECHO) "Building dependency file '$@'"
	[ -d $(DEPDIR) ] || $(MKDIR) $(DEPDIR) > $(BLACKHOLE)
	$(CC) $(DFLAGS) $< >> $@.tmp
	$(SED) 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.tmp > $@
	$(RM) -f $@.tmp
	$(ECHO) "Finished building dependency file '$@'"

$(LIBDIR)/%.lib: %.rel $(OBJFILES)
	$(ECHO)
	$(ECHO) "Building library file '$@'"
	[ -d $(LIBDIR) ] || $(MKDIR) $(LIBDIR) > $(BLACKHOLE)
	$(LIB) $(LFLAGS) $@ $(OBJDIR)/$<
	$(ECHO) "Finished building library file '$@'"
	$(ECHO)

$(OBJDIR)/%.rel : %.c $(DEPFILES)
	$(ECHO)
	$(ECHO) "Building object file '$@'"
	[ -d $(OBJDIR) ] || $(MKDIR) $(OBJDIR) > $(BLACKHOLE)
	$(DEP) -c $(CCFLAGS) $< -o $(OBJDIR)/
	$(ECHO) "Finished building object file '$@'"

$(BUILD)/$(PROJECT).ihx : $(LIBFILES)
	$(ECHO)
	$(ECHO) "Building hex file '$@'"
	$(CC) $(MEMORYMODEL) $(OBJDIR)/main.rel -o $@ $(LDFLAGS) 
	$(ECHO) "Finished building hex file '$@'"

$(BUILD)/%.bin: $(BUILD)/%.ihx
	$(ECHO)
	$(ECHO) "Building binary file '$@'"
	$(HEX2BIN) -p 00 $@ $^
	$(ECHO) "Finished building binary file '$@'"
	# copy to home
	cp -vf $(PROJECTBIN) ~/ 

$(LIBNRF):
	make -C sdk all

$(LIBtinyAES):
	make -C tiny-AES128 all

tools: tools/crc8calc

tools/crc8calc : tools/crc8calc.c
	$(ECHO)
	$(ECHO) "Building '$@'"
	gcc $< -o $@
	$(ECHO) "Finished building '$@'"

clean:
	$(RM) -rf  $(BUILD)/* && $(RMDIR) $(BUILD)

cleanall:
	$(RM) -rf  $(BUILD)/* && $(RMDIR) $(BUILD)
	make -C sdk clean
	make -C tiny-AES128 clean
	
.PHONY: all clean cleanall tools
