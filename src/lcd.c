/*





*/
#include "config.h"

#include <avr/io.h>
//#include <avr/pgmspace.h>
#include <util/delay.h>
#include "lcd.h"


void lcd_init(void)
{
    _delay_ms(50);
    LCD_CTRL_DIR |= (1<<LCD_CTRL_ENABLE_BIT) | (1<<LCD_CTRL_RW_BIT) | (1<<LCD_CTRL_RS_BIT);
    _delay_ms(30);

    //lcd_write(LCD_RW_WRITE,LCD_RS_COMMAND,LCD_INIT_4BIT);

#ifdef LCD_DATABITS_4
#ifdef LCD_DATABITS_4_LOW
    LCD_DATA_DIR |= 0x0F;
#else
    LCD_DATA_DIR |= 0xF0;
#endif
    lcd_write(LCD_RW_WRITE,LCD_RS_COMMAND,LCD_INIT_4BIT);
    _delay_ms(30);
#else
    lcd_write(LCD_RW_WRITE,LCD_RS_COMMAND,LCD_INIT_8BIT);
#endif
    lcd_write(LCD_RW_WRITE,LCD_RS_COMMAND,LCD_COMMAND_CLEAR_SCREEN); //Clear Screen 0x01 = 00000001

    _delay_ms(2);
//#ifdef LCD_HIDE_CURSOR
    lcd_write(LCD_RW_WRITE,LCD_RS_COMMAND,(LCD_ONOFF_SELECT|LCD_ONOFF_DISPLAY)); //Clear Screen 0x01 = 00000001
//#endif
    //lcd_write(LCD_RW_WRITE,LCD_RS_COMMAND,0b00001110);

    _delay_ms(1);
}

void lcd_write(uint8_t wait, uint8_t rs, uint8_t data)
{
    if (wait)
    {
        lcd_wait();
    }

    // prepare LCD controls for data/command input
    LCD_CTRL_PORT &= ~ ((1<<LCD_CTRL_RW_BIT)|(1<<LCD_CTRL_RS_BIT));
    if (rs)
    {
        LCD_CTRL_PORT |= (1<<LCD_CTRL_RS_BIT);
    }
#ifdef LCD_DATABITS_4
    // 4 data bits
    // first: high 4 bits, masq low 4 bits

#ifdef LCD_DATABITS_4_LOW
    LCD_DATA_PORT &= 0xF0;
    LCD_DATA_PORT |= ((data>>4)&(0x0F));
#else
    LCD_DATA_PORT &= 0x0F;
    LCD_DATA_PORT |= ((data)&(0xF0));
#endif

    lcd_apply();

    // second: low 4 bits
#ifdef LCD_DATABITS_4_LOW
    LCD_DATA_PORT &= 0xF0;
    LCD_DATA_PORT |= ((data)&(0x0F));
#else
    LCD_DATA_PORT &= 0x0F;
    LCD_DATA_PORT |= ((data<<4)&(0xF0));
#endif
    lcd_apply();

#ifdef LCD_DATABITS_4_LOW
    LCD_DATA_PORT &= 0xF0;
#else
    LCD_DATA_PORT &= 0x0F;
#endif
#else
    // 8 data bits
    LCD_DATA_PORT = data;
    lcd_apply();
    LCD_DATA_PORT = 0;
#endif

}

void lcd_wait()
{
/*
 * It is faulty right now, so we rely on a delay() cycle instead of reading busy flag
 */
    _delay_ms(2);

/*
   // set data direction to read
#ifdef LCD_DATABITS_4
#ifdef LCD_DATABITS_4_LOW
    LCD_DATA_DIR &= 0xF0; // low 4 bit set to 0 (read)
#else
    LCD_DATA_DIR &= 0x0F; // high 4 bit set to 0 (read)
#endif
#else
    LCD_DATA_DIR = 0;
#endif

    LCD_CTRL_PORT |= (1<<LCD_CTRL_RW_BIT);
    LCD_CTRL_PORT &= ~(1<<LCD_CTRL_RS_BIT);

#ifdef LCD_DATABITS_4
    // in 4 bit mode, reads are done in 2x4 bit
    uint8_t lcd_cursor_address = 0b10000000;  // highest bit is busy bit. set it to busy to enter while loop

    while (lcd_cursor_address >= 0x80){
	// send command to read high 4 bit. result need to be read while enable is high?
        lcd_enable();
	// need to wait 100ns according to datasheet

#ifdef LCD_DATABITS_4_LOW
	lcd_cursor_address = ((LCD_DATA_PORT<<4)&(0xF0));
#else
	lcd_cursor_address = ((LCD_DATA_PORT)&(0xF0));
#endif
	lcd_disable();
        lcd_enable();
#ifdef LCD_DATABITS_4_LOW
	lcd_cursor_address |= ((LCD_DATA_PORT)&(0x0F));
#else
	lcd_cursor_address |= ((LCD_DATA_PORT>>4)&(0x0F));
#endif
	lcd_disable();
    }
#else
    // 8 bit wait
    while (LCD_DATA_PORT >= 0x80)
    {
        lcd_apply();
    }
#endif

   // restore data direction to write
#ifdef LCD_DATABITS_4
#ifdef LCD_DATABITS_4_LOW
    LCD_DATA_DIR |= 0x0F; //0xFF means 0b11111111
#else
    LCD_DATA_DIR |= 0xF0; //0xFF means 0b11111111
#endif
#else
    LCD_DATA_DIR = 0xFF; //0xFF means 0b11111111
#endif
*/
}


void lcd_enable()
{
    LCD_CTRL_PORT |= (1<<LCD_CTRL_ENABLE_BIT);
    asm volatile ("nop");
    asm volatile ("nop");
}

void lcd_disable()
{
    LCD_CTRL_PORT &= ~(1<<LCD_CTRL_ENABLE_BIT);
    asm volatile ("nop");
    asm volatile ("nop");
}

void lcd_apply()
{
    LCD_CTRL_PORT |= (1<<LCD_CTRL_ENABLE_BIT);
    asm volatile ("nop");
    asm volatile ("nop");
    LCD_CTRL_PORT &= ~(1<<LCD_CTRL_ENABLE_BIT);
}

void lcd_printstring(char *StringOfCharacters)
{
    while(*StringOfCharacters > 0)
    {
        lcd_write(LCD_RW_WRITE,LCD_RS_CHARACTER,*StringOfCharacters++);
    }
}

void lcd_cmd_printstring(uint8_t command, char *StringOfCharacters)
{
    lcd_write(LCD_RW_WRITE,LCD_RS_COMMAND,command);
    while(*StringOfCharacters > 0)
    {
        lcd_write(LCD_RW_WRITE,LCD_RS_CHARACTER,*StringOfCharacters++);
    }
}

void lcd_print_addrstr(addrstr *mystring)
{


    lcd_write(LCD_RW_WRITE,LCD_RS_COMMAND,(LCD_COMMAND_MOVE_CURSOR | mystring->position));

    uint8_t i = 0;
    //uint8_t j = 0;
    //j = pgm_read_byte(&mystring->text[i]);

    while(mystring->text[i])
    {
        //j = pgm_read_byte(&mystring->text[i++]);
        //lcd_write(LCD_RW_WRITE,LCD_RS_CHARACTER,j);
        lcd_write(LCD_RW_WRITE,LCD_RS_CHARACTER,mystring->text[i++]);
    }
}

inline void lcd_print_character(uint8_t printchar)
{
    lcd_write(LCD_RW_WRITE,LCD_RS_CHARACTER,printchar);
}


/*
void CopyStringtoLCD(const uint8_t *FlashLoc, uint8_t x, uint8_t y)
{
        uint8_t i;
        LCDGotoXY(x,y);
        for(i=0;(uint8_t)pgm_read_byte(&FlashLoc[i]);i++)
        {
                LCDsendChar((uint8_t)pgm_read_byte(&FlashLoc[i]));
        }
}
*/
