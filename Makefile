
CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

GCC_FLAGS = -nostdlib -mcpu=cortex-m4 -mthumb -fno-exceptions -fno-rtti

SRC = main.cpp
STUB = stub.c
STARTUP = startup.s
LINK_SRC = main.ld

all: main.bin

main.o: $(SRC)
	$(CC) -c $(GCC_FLAGS) $(SRC) -o main.o

stub.o: $(STUB)
	$(CC) -c $(GCC_FLAGS) $(STUB) -o stub.o

startup.o: $(STARTUP)
	$(CC) -c $(GCC_FLAGS) $(STARTUP) -o startup.o

main.elf: $(LINK_SRC) main.o stub.o startup.o
	$(LD) -T $(LINK_SRC) -o main.elf

main.bin: main.elf
	$(OBJCOPY) -O binary main.elf main.bin

flash: main.bin
	st-flash write main.bin 0x08000000

clean:
	rm -f *.o *.elf *.bin
