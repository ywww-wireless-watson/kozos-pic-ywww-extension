#PREFIX  = 
ARCH = mips-elf
#BINDIR  = $(PREFIX)/bin
ADDNAME = $(ARCH)-

AR      = $(ADDNAME)ar
AS      = $(ADDNAME)as
CC      = $(ADDNAME)gcc
LD      = $(ADDNAME)ld
NM      = $(ADDNAME)nm
OBJCOPY = $(ADDNAME)objcopy
OBJDUMP = $(ADDNAME)objdump
RANLIB  = $(ADDNAME)ranlib
STRIP   = $(ADDNAME)strip
BIN2HEX = $(ADDNAME)bin2hex

OBJS  = startup.o main.o lib.o serial.o timer.o interrupt.o hardware.o

OBJS += kozos.o syscall.o memory.o consdrv.o tmrdrv.o command.o periodic.o

TARGET = kozos

CFLAGS = -Wall -march=m4k -EL -nostdinc -nostdlib -fno-builtin -DDEBUG
CFLAGS += -I. 
CFLAGS += -G0 #これをしないとエラーが出る
CFLAGS += -Os
CFLAGS += -DKOZOS
CFLAGS += -MMD -MP

LFLAGS = -static -T ld.scr -L.

DEPS = $(OBJS:.o=.d)

.SUFFIXES: .c .o
.SUFFIXES: .S .o
.SUFFIXES: .s .o

all : 
	docker run --rm -it -v $(PWD):$(PWD) -w $(PWD) -u "$(shell id -u):$(shell id -g)" -v /etc/passwd:/etc/passwd:ro -v /etc/group:/etc/group:ro ghcr.io/envzhu/kozos-pic-ongdb/build:latest /bin/bash -c "\
	make build && \
	exit"

build: $(TARGET) $(TARGET).hex

$(TARGET) :	$(OBJS)
		$(CC) $(OBJS) -o $(TARGET) $(CFLAGS) $(LFLAGS)
		cp $(TARGET) $(TARGET).elf
		$(STRIP) $(TARGET)

.c.o :		$<
		$(CC) -c $(CFLAGS) $< -o $@

.S.o :		$<
		$(CC) -c $(CFLAGS) $< -o $@

.s.o :		$<
		$(CC) -c $(CFLAGS) $< -o $@

$(TARGET).hex :	$(TARGET)
		$(OBJCOPY) -O ihex $(TARGET) $(TARGET).hex

image :		$(TARGET).hex

obj_clean :
		rm -f $(OBJS) $(DEPS)

clean :
		rm -f $(OBJS) $(TARGET) $(TARGET).elf $(TARGET).hex $(DEPS)

-include $(DEPS)
