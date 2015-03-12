# Target definitions
ARM_FIR_TARGET = arm_fir_test
ECE486_FIR_TARGET = ece486_fir_test
ARM_BIQUAD_TARGET = arm_biquad_test
ECE486_BIQUAD_TARGET = ece486_biquad_test
TASK4_TARGET = ece486_biquad_task4

# Object definitions
ARM_FIR_OBJS = arm_fir_test.o
ECE486_FIR_OBJS = ARM_fir.o ece486_fir_test.o
ARM_BIQUAD_OBJS = arm_biquad_test.o
ECE486_BIQUAD_OBJS = ARM_biquad.o ece486_biquad_test.o
TASK4_OBJS = ARM_biquad.o ece486_biquad_task4.o biquad_coef.o

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
all: $(ARM_FIR_TARGET) $(ARM_FIR_TARGET).bin $(ECE486_FIR_TARGET) $(ECE486_FIR_TARGET).bin $(ARM_BIQUAD_TARGET) $(ARM_BIQUAD_TARGET).bin $(ECE486_BIQUAD_TARGET) $(ECE486_BIQUAD_TARGET).bin $(TASK4_TARGET) $(TASK4_TARGET).bin

debug : CFLAGS += -DDEBUG -g -Og
debug : LDFLAGS += -Wl,-Map,$(TARGET).map

debug : all
# Optimized STM FIR routine test program
$(ARM_FIR_TARGET): $(ARM_FIR_OBJS)
	$(CC) -o $(ARM_FIR_TARGET) $(CFLAGS) $(LDFLAGS) $(ARM_FIR_OBJS) $(LIBS)

$(ARM_FIR_TARGET).bin: $(ARM_FIR_TARGET)
	$(OBJCOPY) -Obinary $(ARM_FIR_TARGET) $(ARM_FIR_TARGET).bin

# Developed FIR routine test program
$(ECE486_FIR_TARGET): $(ECE486_FIR_OBJS)
	$(CC) -o $(ECE486_FIR_TARGET) $(CFLAGS) $(LDFLAGS) $(ECE486_FIR_OBJS) $(LIBS)

$(ECE486_FIR_TARGET).bin: $(ECE486_FIR_TARGET)
	$(OBJCOPY) -Obinary $(ECE486_FIR_TARGET) $(ECE486_FIR_TARGET).bin

# Optimized STM IIR routine test program
$(ARM_BIQUAD_TARGET): $(ARM_BIQUAD_OBJS)
	$(CC) -o $(ARM_BIQUAD_TARGET) $(CFLAGS) $(LDFLAGS) $(ARM_BIQUAD_OBJS) $(LIBS)

$(ARM_BIQUAD_TARGET).bin: $(ARM_BIQUAD_TARGET)
	$(OBJCOPY) -Obinary $(ARM_BIQUAD_TARGET) $(ARM_BIQUAD_TARGET).bin

# Developed IIR/biquad routine test program
$(ECE486_BIQUAD_TARGET): $(ECE486_BIQUAD_OBJS)
	$(CC) -o $(ECE486_BIQUAD_TARGET) $(CFLAGS) $(LDFLAGS) $(ECE486_BIQUAD_OBJS) $(LIBS)

$(ECE486_BIQUAD_TARGET).bin: $(ECE486_BIQUAD_TARGET)
	$(OBJCOPY) -Obinary $(ECE486_BIQUAD_TARGET) $(ECE486_BIQUAD_TARGET).bin

# Task 4 test program
$(TASK4_TARGET): $(TASK4_OBJS)
	$(CC) -o $(TASK4_TARGET) $(CFLAGS) $(LDFLAGS) $(TASK4_OBJS) $(LIBS)

$(TASK4_TARGET).bin: $(TASK4_TARGET)
	$(OBJCOPY) -Obinary $(TASK4_TARGET) $(TASK4_TARGET).bin

# Dependencies
ARM_biquad.o: ece486_biquad.c ece486_biquad.h
	$(CC) -c ece486_biquad.c -o ARM_biquad.o $(CFLAGS)

ARM_fir.o: ece486_fir.c ece486_fir.h
	$(CC) -c ece486_fir.c -o ARM_fir.o $(CFLAGS)

biquad_coef.o: biquad_coef.c biquad_coef.h
	$(CC) -c biquad_coef.c -o biquad_coef.o $(CFLAGS)


flash_arm_fir: $(ARM_FIR_TARGET).bin
	st-flash write $(ARM_FIR_TARGET).bin 0x08000000

flash_ece486_fir: $(ECE486_FIR_TARGET).bin
	st-flash write $(ECE486_FIR_TARGET).bin 0x08000000

flash_arm_biquad: $(ARM_BIQUAD_TARGET).bin
	st-flash write $(ARM_BIQUAD_TARGET).bin 0x08000000

flash_ece486_biquad: $(ECE486_BIQUAD_TARGET).bin
	st-flash write $(ECE486_BIQUAD_TARGET).bin 0x08000000

flash_task4: $(TASK4_TARGET).bin
	st-flash write $(TASK4_TARGET).bin 0x08000000

clean:
	rm -f $(ARM_FIR_OBJS) $(ARM_FIR_TARGET) $(ARM_FIR_TARGET).bin $(ARM_FIR_TARGET).map;
	rm -f $(ECE486_FIR_OBJS) $(ECE486_FIR_TARGET) $(ECE486_FIR_TARGET).bin $(ECE486_FIR_TARGET).map;
	rm -f $(ARM_BIQUAD_OBJS) $(ARM_BIQUAD_TARGET) $(ARM_BIQUAD_TARGET).bin $(ARM_BIQUAD_TARGET).map;
	rm -f $(ECE486_BIQUAD_OBJS) $(ECE486_BIQUAD_TARGET) $(ECE486_BIQUAD_TARGET).bin $(ECE486_BIQUAD_TARGET).map;
	rm -f $(TASK4_OBJS) $(TASK4_TARGET) $(TASK4_TARGET).bin $(TASK4_TARGET).map