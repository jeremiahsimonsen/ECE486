#TARGET=group02_lab2
BOARD_TARGET = board
T1_TARGET = task1
T2_TARGET = task2

#OBJS  = group02_lab2.o
BOARD_OBJS = board_biquad.o board_fir.o
T1_OBJS = task1.o ece486_fir.o
T2_OBJS = task2.o ece486_biquad.o

#  Support either ARCH=STM32F429xx or ARCH=STM32F407xx
ARCH = STM32F407xx
#ARCH = STM32F429xx
INSTALLDIR = /usr/local/stmdev/

# Compiler options for STM board
###############################################################
CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy

INCDIRS = -I$(INSTALLDIR)/include -I.
LIBDIRS = -L$(INSTALLDIR)/lib

LIBS= -lc -lnosys -lece486_$(ARCH) -l$(ARCH) -lcmsis_dsp_$(ARCH)

LINKSCRIPT = $(INSTALLDIR)/lib/$(ARCH)_FLASH.ld

CFLAGS = -mcpu=cortex-m4 -mthumb -O3 -Wall  \
         -fomit-frame-pointer -fno-strict-aliasing -fdata-sections \
         -include stm32f4xx_hal_conf.h -DARM_MATH_CM4 -D$(ARCH) \
         -mfpu=fpv4-sp-d16 -mfloat-abi=softfp $(INCDIRS) \
         -fsingle-precision-constant


LDFLAGS = -Wl,-T$(LINKSCRIPT) \
          -Wl,--gc-sections $(LIBDIRS)
               
.PHONY : all flash clean debug

################################################################
# Rules
#all: fir_test $(TARGET) $(TARGET).bin
all: $(T1_TARGET) $(T2_TARGET) $(BOARD_TARGET) $(BOARD_TARGET).bin

ece486_fir.o: ece486_fir.c ece486_fir.h
	gcc -c ece486_fir.c -o ece486_fir.o

board_fir.o: ece486_fir.c ece486_fir.h
	$(CC) -c ece486_fir.c -o board_fir.o $(CFLAGS)

ece486_biquad.o: ece486_biquad.c ece486_biquad.h
	gcc -c ece486_biquad.c -o ece486_biquad.o

board_biquad.o: ece486_biquad.c ece486_biquad.h
	$(CC) -c ece486_biquad.c -o board_biquad.o $(CFLAGS)

$(T1_TARGET): $(T1_OBJS)
	gcc -o $(T1_TARGET) $(T1_OBJS)

task1.o: test_fir.c ece486_fir.o
	gcc -c test_fir.c -o task1.o

$(T2_TARGET): $(T2_OBJS)
	gcc -o $(T2_TARGET) $(T2_OBJS)

task2.o: test_biquad.c ece486_biquad.o
	gcc -c test_biquad.c -o task2.o

debug : CFLAGS += -DDEBUG -g -Og
debug : LDFLAGS += -Wl,-Map,$(TARGET).map

debug : all

$(BOARD_TARGET): $(BOARD_OBJS)
	$(CC) board.c -o $(BOARD_TARGET) $(CFLAGS) $(LDFLAGS) $(BOARD_OBJS) $(LIBS)

$(BOARD_TARGET).bin: $(BOARD_TARGET)
	$(OBJCOPY) -Obinary $(BOARD_TARGET) $(BOARD_TARGET).bin

# board.o: board.c ece486_fir.o ece486_biquad.o
# 	$(CC) -c board.c -o board.o $(LDFLAGS) $(LIBS)

flash: $(BOARD_TARGET).bin
	st-flash write $(BOARD_TARGET).bin 0x08000000

clean:
	rm -f $(BOARD_OBJS) $(BOARD_TARGET) $(BOARD_TARGET).bin $(BOARD_TARGET).map;
	rm -f $(T1_OBJS) $(T1_TARGET);
	rm -f $(T2_OBJS) $(T2_TARGET);
