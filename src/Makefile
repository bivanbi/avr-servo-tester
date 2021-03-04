#avr-gcc -mmcu=attiny2313 -Os lcd.c -o lcd.o
#avr-objcopy -j .text -j .data -O ihex  lcd.o  lcd.hex

PRG = main
SRCS = main.c lcd.c adc.c switch.c pwm_generator.c
INCS = main.h lcd.h config.h adc.h switch.h eeprom_data.h pwm_generator.h

MCU_TARGET = atmega328
PROGRAMMER = jtag3isp
PROGRAMMER_TARGET = m328

OPTIMIZE = -O2
LIBS =
CFLAGS = -std=gnu89 -g -Wall $(OPTIMIZE) -Wl,-Map,$(PRG).map -mmcu=$(MCU_TARGET)
# You should not have to change anything below here.
CC = avr-gcc
# Override is only needed by avr-lib build system.
#override 
#override LDFLAGS = -Wl,-Map,$(PRG).map
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump

all: $(PRG).hex

# save EEPROM to tempfile because flashing will erase eeprom too
flash: $(PRG).hex
	avrdude -c $(PROGRAMMER) -p $(PROGRAMMER_TARGET) -U eeprom:r:$(PRG).eeprom.tmp.hex:i
	avrdude -c $(PROGRAMMER) -p $(PROGRAMMER_TARGET) -U flash:w:$(PRG).hex
	avrdude -c $(PROGRAMMER) -p $(PROGRAMMER_TARGET) -U eeprom:w:$(PRG).eeprom.tmp.hex:i

eeprom_read:
	avrdude -c $(PROGRAMMER) -p $(PROGRAMMER_TARGET) -U eeprom:r:$(PRG).eeprom.tmp.hex:i

eeprom_init: $(PRG).hex
	avrdude -c $(PROGRAMMER) -p $(PROGRAMMER_TARGET) -U eeprom:w:$(PRG).eeprom.hex

$(PRG).o: $(SRCS) $(INCS)
	$(CC) $(CFLAGS) -Os $(SRCS) -o $(PRG).o

$(PRG).hex: $(PRG).o
	avr-objcopy -j .text -j .data -O ihex  $(PRG).o  $(PRG).hex
	avr-objcopy -j .eeprom --change-section-lma .eeprom=0 -O ihex $(PRG).o $(PRG).eeprom.hex

clean:
	rm -rf *.o $(PRG).elf *.eps *.png *.pdf *.bak *.map *.hex *.s
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)

size: $(PRG).o
	avr-size main.o


.PHONY: all flash clean size
