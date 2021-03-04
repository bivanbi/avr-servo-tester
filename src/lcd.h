#ifndef LCD_HEADER_LOADED
#define LCD_HEADER_LOADED

typedef struct
{
    uint8_t position;
    char    *text;
} addrstr;

/*
	LCD DATA PORT
*/
#ifndef LCD_DATA_PORT
#define LCD_DATA_PORT PORTB
#endif

#ifndef LCD_DATA_DIR
#define LCD_DATA_DIR DDRB
#endif

/*
	LCD CONTROL PORT + PINS
*/
#ifndef LCD_CTRL_PORT
#define LCD_CTRL_PORT PORTD
#endif

#ifndef LCD_CTRL_DIR
#define LCD_CTRL_DIR DDRD
#endif

//                          000 0000020
#ifndef LCD_CTRL_ENABLE_BIT
#define LCD_CTRL_ENABLE_BIT     5
#endif

#ifndef LCD_CTRL_RW_BIT
#define LCD_CTRL_RW_BIT         4
#endif

#ifndef LCD_CTRL_RS_BIT
#define LCD_CTRL_RS_BIT         3
#endif


/*
	LCD COMMAND: FUNCTION
*/
#define LCD_FUNCTION_SELECT             0x20
#define LCD_FUNCTION_8BIT_MODE          0x10
#define LCD_FUNCTION_4BIT_MODE          0x00
#define LCD_FUNCTION_2LINE_MODE         0x08
#define LCD_FUNCTION_1LINE_MODE         0x00

/*
	LCD COMMAND: ON/OFF
*/
#define LCD_ONOFF_SELECT                0x08
#define LCD_ONOFF_DISPLAY               0x04
#define LCD_ONOFF_CURSOR                0x02
#define LCD_ONOFF_CURSOR_BLINK          0x01

/*
	LCD COMMAND: MOVE CURSOR
*/
#define LCD_COMMAND_MOVE_CURSOR         0x80
#define LCD_ADDRESS_FIRST_LINE          0x00
#define LCD_ADDRESS_SECOND_LINE         0x40

#define LCD_COMMAND_CLEAR_SCREEN        0x01


#define LCD_RS_COMMAND			0
#define LCD_RS_CHARACTER		1

#define LCD_RW_READ			0
#define LCD_RW_WRITE			1

#define LCD_INIT_8BIT                   (LCD_FUNCTION_SELECT | LCD_FUNCTION_4BIT_MODE | LCD_FUNCTION_2LINE_MODE)
#define LCD_INIT_4BIT                   (LCD_FUNCTION_SELECT | LCD_FUNCTION_4BIT_MODE | LCD_FUNCTION_2LINE_MODE)

void lcd_init(void);
void lcd_wait(void);
void lcd_enable(void);
void lcd_disable(void);
void lcd_apply(void);
void lcd_write(uint8_t wait, uint8_t rs, uint8_t data);
void lcd_printstring(char *StringOfCharacters);
void lcd_flash_led(uint8_t times);
void lcd_cmd_printstring(uint8_t command, char *StringOfCharacters);
void lcd_print_addrstr(addrstr *mystring);
inline void lcd_print_character(uint8_t printchar);


#endif


