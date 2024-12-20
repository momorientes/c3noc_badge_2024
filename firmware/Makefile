# Define the source and target files
SRC = blink.c
TARGET = firmware
PROGRAMMER = /dev/cu.usbserial-210

# Default target
all: compile flash

# Compile target
compile:
	avr-gcc -mmcu=attiny404 -DF_CPU=16000000UL -D__AVR_DEV_LIB_NAME__=tn204 -Os -o $(TARGET).elf $(SRC)
	avr-objcopy -O ihex $(TARGET).elf $(TARGET).hex

# Flash target
flash:
	@echo "Flashing $(TARGET) to the device..."
	avrdude -c serialupdi -P $(PROGRAMMER) -pt404 -U flash:w:$(TARGET).hex

# Clean target
clean:
	rm -f $(TARGET).hex $(TARGET).elf

.PHONY: all compile flash clean
