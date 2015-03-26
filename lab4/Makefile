# Target definitions
TARGET = lab4

# Object definitions
OBJS = ARM_biquad.o ARM_fir.o filter1_coef.o filter2_coef.o frequency_estimation.o mixer.o rejectDC.o real_time.o 
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
all: $(TARGET) $(TARGET).bin

debug : CFLAGS += -DDEBUG -g -Og
debug : LDFLAGS += -Wl,-Map,$(TARGET).map

debug : all
# Target
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS)

$(TARGET).bin: $(TARGET)
	$(OBJCOPY) -Obinary $(TARGET) $(TARGET).bin

# Dependencies
ARM_biquad.o: ece486_biquad.c ece486_biquad.h
	$(CC) -c ece486_biquad.c -o ARM_biquad.o $(CFLAGS)

ARM_fir.o: ece486_fir.c ece486_fir.h
	$(CC) -c ece486_fir.c -o ARM_fir.o $(CFLAGS)

filter1_coef.o: filter1_coef.c filter1_coef.h
	$(CC) -c filter1_coef.c -o filter1_coef.o $(CFLAGS)

filter2_coef.o: filter2_coef.c filter2_coef.h
	$(CC) -c filter2_coef.c -o filter2_coef.o $(CFLAGS)

frequency_estimation.o: frequency_estimation.c frequency_estimation.h
	$(CC) -c frequency_estimation.c -o frequency_estimation.o $(CFLAGS)

rejectDC.o: rejectDC.c rejectDC.h
	$(CC) -c rejectDC.c -o rejectDC.o $(CFLAGS)

dcblock.o: dcblock.c dcblock.h
	$(CC) -c dcblock.c -o dcblock.o $(CFLAGS)

mixer.o: ece486_mixer.c ece486_mixer.h
	$(CC) -c ece486_mixer.c -o mixer.o $(CFLAGS)

flash: $(TARGET).bin
	st-flash write $(TARGET).bin 0x08000000

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).bin $(TARGET).map;