src = $(wildcard *.c)
obj = $(src:.c=.o)
bin = 3dglassbox
hex = $(bin).hex
eep = $(bin).eep

mcu_gcc = attiny13a
mcu_dude = t13a

CC = avr-gcc
OBJCOPY = avr-objcopy

warn = -pedantic -Wall

CFLAGS = -Os $(warn) -mmcu=$(mcu_gcc) -DF_CPU=1000000
LDFLAGS = -Wl,-Map,$(bin).map -mmcu=$(mcu_gcc)

.PHONY: all
all: $(hex) $(eep)

$(bin): $(obj)
	$(CC) -o $@ $(obj) $(LDFLAGS)

$(hex): $(bin)
	$(OBJCOPY) -j .text -j .data -O ihex -R .eeprom $< $@

$(bin).bin: $(bin)
	$(OBJCOPY) -j .text -j .data -O binary -R .eeprom $< $@

$(eep): $(bin)
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

.PHONY: fuses
fuses:
	avrdude -c usbtiny -p $(mcu_dude) -U lfuse:w:0x6a:m -U hfuse:w:0xff:m

.PHONY: program
program: $(hex)
	avrdude -c usbtiny -p $(mcu_dude) -e -U flash:w:$(hex)

.PHONY: clean
clean:
	rm -f $(bin) $(obj) $(hex) $(eep) $(bin).map
