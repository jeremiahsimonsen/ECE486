# Target definitions
ARM_TARGET = arm

# Object definitions
ARM_OBJS = ARM_biquad.o ARM_fir.o board.o
SRC = ece486_biquad.c ece486_biquad.h ece486_fir.c ece486_fir.h
C_FILES = ece486_biquad.c ece486_fir.c

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
all: $(ARM_TARGET) $(ARM_TARGET).bin

debug : CFLAGS += -DDEBUG -g -Og
debug : LDFLAGS += -Wl,-Map,$(TARGET).map

debug : all

$(ARM_TARGET): $(ARM_OBJS)
	$(CC) -o $(ARM_TARGET) $(CFLAGS) $(LDFLAGS) $(ARM_OBJS) $(LIBS)

$(ARM_TARGET).bin: $(ARM_TARGET)
	$(OBJCOPY) -Obinary $(ARM_TARGET) $(ARM_TARGET).bin

ARM_biquad.o: ece486_biquad.c ece486_biquad.h
	$(CC) -c ece486_biquad.c -o ARM_biquad.o $(CFLAGS)

ARM_fir.o: ece486_fir.c ece486_fir.h
	$(CC) -c ece486_fir.c -o ARM_fir.o $(CFLAGS)


flash: $(ARM_TARGET).bin
	st-flash write $(ARM_TARGET).bin 0x08000000

clean:
	rm -f $(ARM_OBJS) $(ARM_TARGET) $(ARM_TARGET).bin $(ARM_TARGET).map;