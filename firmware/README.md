# C3NOC Badge 38C3 / 2024

## Installing toolchain

On macOS, you need avr-gcc:

```
brew tap osx-cross/avr
brew install avr-gcc avrdude
```
Because the ATtiny204/404 is pretty new we also need to download the support pack:

```
wget https://packs.download.microchip.com/Microchip.ATtiny_DFP.3.1.260.atpack
mkdir microchip_out/
mv Microchip.ATtiny_DFP.3.1.260.atpack Microchip.ATtiny_DFP.3.1.260.zip
unzip Microchip.ATtiny_DFP.3.1.260.zip -d microchip_out
cd microchip_out
cp include/avr/iotn* /opt/homebrew/Cellar/avr-gcc@9/9.4.0_1/avr/include/avr/
cp gcc/dev/attiny*/avrxmega3/*.{a,o} /opt/homebrew/Cellar/avr-gcc@9/9.4.0_1/avr/lib/avrxmega3
cp gcc/dev/attiny*/avrxmega3/short-calls/*.{a,o}  /opt/homebrew/Cellar/avr-gcc@9/9.4.0_1/avr/lib/avrxmega3/short-calls
sudo cp gcc/dev/attiny*/device-specs/*  /opt/homebrew/Cellar/avr-gcc@9/9.4.0_1/lib/avr-gcc/9/gcc/avr/9.4.0/device-specs/
```


## Compiling and Flashing

### Makefile
```bash
make compile # compile but don't flash
make flash # flash firmware.hex 
make all # compile and flash
```

### Manual Compiling

```bash
avr-gcc -mmcu=attiny404 -DF_CPU=16000000UL -D__AVR_DEV_LIB_NAME__=tn404  -Os -o blink.elf blink.c
avr-objcopy -O ihex blink.elf blink.hex
```

## Manual Flashing

```bash
avrdude -c serialupdi -P /dev/cu.usbserial-210 -b 57600 -p t404 -U flash:w:blink.hex
```
