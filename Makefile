MEMORYMODEL = --model-large

SDCCBIN = /opt/sdcc/bin
SDCCINC = /opt/sdcc/share/sdcc/include

CC = $(SDCCBIN)/sdcc
LD = $(SDCCBIN)/sdld
CPP = $(SDCCBIN)/sdcpp
AS = $(SDCCBIN)/sdas8051
HEX2BIN = ../hex2bin/hex2bin

SDKSRC = ../SDK/src
SDKINC = ../SDK/include
AESINC = aes/include 

BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj

CFLAGS = -I$(SDCCINC) -I$(SDKINC) -I$(AESINC) --std-c99 $(MEMORYMODEL)
LDFLAGS = -L$(OBJDIR)

PROJECT = nrf24le1-sensor

SRCS = main.c

OBJS := $(SRCS:.c=.rel)
OBJS := $(addprefix $(OBJDIR)/,$(OBJS))

all: $(BUILDDIR)/$(PROJECT).ihx $(BUILDDIR)/$(PROJECT).bin

$(OBJS) : $(OBJDIR)/%.rel : %.c

$(BUILDDIR)/$(PROJECT).ihx : $(OBJDIR)/main.rel $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $(BUILDDIR)/$(PROJECT).ihx

%.bin: %.ihx
	$(HEX2BIN) -p 00 $@ $^

$(OBJDIR)/%.rel : %.c $(OBJDIR)
	@echo "Compiling $<"
	$(CC) -c $(CFLAGS) $< -o $(OBJDIR)/

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(BUILDDIR)
