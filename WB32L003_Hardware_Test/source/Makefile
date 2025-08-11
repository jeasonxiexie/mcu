# Makefile for WB32L003 simple test

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CFLAGS = -mcpu=cortex-m0plus -mthumb -O2 -g -Wall -nostdlib -ffreestanding
LDFLAGS = -Tlinker.ld -nostartfiles

# Build all versions
all: pc0_pc1 pb0_pb1 pa0_pa1

# PC0/PC1 version (default)
pc0_pc1:
	$(CC) $(CFLAGS) -DLED1_GPIO=\'C\' -DLED1_PIN=0 -DLED2_GPIO=\'C\' -DLED2_PIN=1 \
		-c simple_test.c -o simple_test_pc.o
	$(CC) $(CFLAGS) -c startup_wb32l003.s -o startup.o
	$(CC) $(CFLAGS) $(LDFLAGS) startup.o simple_test_pc.o -o simple_test_pc0_pc1.elf
	$(OBJCOPY) -O ihex simple_test_pc0_pc1.elf ../prebuilt/simple_test_pc0_pc1.hex
	$(SIZE) simple_test_pc0_pc1.elf

# PB0/PB1 version
pb0_pb1:
	$(CC) $(CFLAGS) -DLED1_GPIO=\'B\' -DLED1_PIN=0 -DLED2_GPIO=\'B\' -DLED2_PIN=1 \
		-c simple_test.c -o simple_test_pb.o
	$(CC) $(CFLAGS) -c startup_wb32l003.s -o startup.o
	$(CC) $(CFLAGS) $(LDFLAGS) startup.o simple_test_pb.o -o simple_test_pb0_pb1.elf
	$(OBJCOPY) -O ihex simple_test_pb0_pb1.elf ../prebuilt/simple_test_pb0_pb1.hex
	$(SIZE) simple_test_pb0_pb1.elf

# PA0/PA1 version
pa0_pa1:
	$(CC) $(CFLAGS) -DLED1_GPIO=\'A\' -DLED1_PIN=0 -DLED2_GPIO=\'A\' -DLED2_PIN=1 \
		-c simple_test.c -o simple_test_pa.o
	$(CC) $(CFLAGS) -c startup_wb32l003.s -o startup.o
	$(CC) $(CFLAGS) $(LDFLAGS) startup.o simple_test_pa.o -o simple_test_pa0_pa1.elf
	$(OBJCOPY) -O ihex simple_test_pa0_pa1.elf ../prebuilt/simple_test_pa0_pa1.hex
	$(SIZE) simple_test_pa0_pa1.elf

clean:
	rm -f *.o *.elf ../prebuilt/*.hex

.PHONY: all pc0_pc1 pb0_pb1 pa0_pa1 clean