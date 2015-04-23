# Target definitions
TARGET1 = task1
TARGET2 = task2

# Object definitions
FILTERS = ARM_biquad.o
COEFS = lowpass1_coef.o
BOARD = real_time.o
BOARD2 = real_time_stepped.o
OBJS = $(FILTERS) $(COEFS) $(BOARD) mixer.o
OBJS2 = mixer.o $(BOARD2) 
#rejectDC.o dcblock.o

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

LIBS= -lc -lnosys -lece486_$(ARCH) -l$(ARCH) -lcmsis_dsp_$(ARCH) -lm

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
all: $(TARGET1) $(TARGET1).bin $(TARGET2) $(TARGET2).bin

debug : CFLAGS += -DDEBUG -g -Og
debug : LDFLAGS += -Wl,-Map,$(TARGET1).map

debug : all
# Target1
$(TARGET1): $(OBJS)
	$(CC) -o $(TARGET1) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS)

$(TARGET1).bin: $(TARGET1)
	$(OBJCOPY) -Obinary $(TARGET1) $(TARGET1).bin

# Target2
$(TARGET2): $(OBJS2)
	$(CC) -o $(TARGET2) $(CFLAGS) $(LDFLAGS) $(OBJS2) $(LIBS)

$(TARGET2).bin: $(TARGET2)
	$(OBJCOPY) -Obinary $(TARGET2) $(TARGET2).bin

# Dependencies
ARM_biquad.o: ece486_biquad.c ece486_biquad.h
	$(CC) -c ece486_biquad.c -o ARM_biquad.o $(CFLAGS)

ARM_fir.o: ece486_fir.c ece486_fir.h
	$(CC) -c ece486_fir.c -o ARM_fir.o $(CFLAGS)

lowpass1_coef.o: lowpass1_coef.c lowpass1_coef.h
	$(CC) -c lowpass1_coef.c -o lowpass1_coef.o $(CFLAGS)

mixer.o: ece486_mixer.c ece486_mixer.h
	$(CC) -c ece486_mixer.c -o mixer.o $(CFLAGS)

flash1: $(TARGET1).bin
	st-flash write $(TARGET1).bin 0x08000000

flash2: $(TARGET2).bin
	st-flash write $(TARGET2).bin 0x08000000

clean:
	rm -f $(OBJS) $(TARGET1) $(TARGET1).bin $(TARGET1).map $(OBJS2) $(TARGET2) $(TARGET2).bin $(TARGET2).map;