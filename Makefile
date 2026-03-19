
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

GCC_FLAGS = -nostdlib -mcpu=cortex-m4 -mthumb -fno-exceptions -fno-rtti -O1
C_FLAGS = -nostdlib -mcpu=cortex-m4 -mthumb -O1
DEBUG_FLAGS = -nostdlib -mcpu=cortex-m4 -mthumb -fno-exceptions -fno-rtti -O0 -g


SRC = bootloader/main.cpp
STUB = include/stub.c
STARTUP = include/startup.s
LINK_SRC = include/main.ld

all: main.bin

main.o: $(SRC)
	$(CC) -c $(GCC_FLAGS) $(SRC) -o main.o

stub.o: $(STUB)
	$(CC) -c $(C_FLAGS) $(STUB) -o stub.o

startup.o: $(STARTUP)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb $(STARTUP) -o startup.o

main.elf: $(LINK_SRC) main.o stub.o startup.o
	$(LD) -T $(LINK_SRC) -o main.elf

main.bin: main.elf
	$(OBJCOPY) -O binary main.elf main.bin

flash: main.bin
	st-flash write main.bin 0x08000000

debug:
	$(CC) -c $(DEBUG_FLAGS) $(SRC) -o main.o
	$(CC) -c $(DEBUG_FLAGS) $(STUB) -o stub.o
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb $(STARTUP) -o startup.o
	$(LD) -T $(LINK_SRC) -o main.elf
	$(OBJCOPY) -O binary main.elf main.bin

clean:
	rm -f *.o *.elf *.bin
