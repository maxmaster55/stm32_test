#pragma once

#include <MCAL/GPIO/gpio.h>


// 4 or 8
#define LCD_MODE_4  (4)
#define LCD_MODE_8  (8)
#define LCD_MODE    (LCD_MODE_4)

// will use fisrt 4 pins of A for now
typedef struct
{
    GPIO_RegDef_t* port;
    uint8_t d_pins[LCD_MODE]; // will use 8 for now
    uint8_t rs_pin;
    uint8_t en_pin;
    uint8_t rw_pin;
} lcd_cfg_t;


typedef enum{
    LCD_RET_OK,
    LCD_RET_NOK
} lcd_ret_t;

#define LCD_RS_COMMAND  (0)
#define LCD_RS_DATA     (1)

#define LCD_RW_WRITE    (0)
#define LCD_RW_READ     (1)

// 2). Function set:
// DL = 1 : 8 bit interface operation
// N = 0 : 1 - line display
// F = 0 : 5 x 7 dot character font

#define LCD_CMD_ONE_LINE        (0b00110000)
#define LCD_CMD_TWO_LINES       (0b00111000)
#define LCD_CMD_LINE_FONT_7     (0b00110000)
#define LCD_CMD_LINE_FONT_10    (0b00110100)


// (3). Display ON/OFF Control:
// D = 0 : Display OFF
// C = 0 : Cursor OFF
// B = 0 : Blink OFF
#define LCD_CMD_DISPLAY_ON              (0b00001100)
#define LCD_CMD_DISPLAY_OFF             (0b00001000)
#define LCD_CMD_CURSOR_ON               (0b00001010)
#define LCD_CMD_CURSOR_OFF              (0b00001000)
#define LCD_CMD_CURSOR_BLINK_ON         (0b00001001)
#define LCD_CMD_CURSOR_BLINK_OFF        (0b00001000)

// (4). Entry Mode Set
// I/D = 1 : +1 (Increment Mode)
// S = 0 : No Display Shift operation
#define LCD_CMD_INCREMENT_ON         (0b00000110)
#define LCD_CMD_INCREMENT_OFF        (0b00000100)
#define LCD_CMD_SHIFT_ON             (0b00000101)
#define LCD_CMD_SHIFT_OFF            (0b00000100)



#define LCD_CUSTOM_CHAR_0       (0)
#define LCD_CUSTOM_CHAR_1       (1)
#define LCD_CUSTOM_CHAR_2       (2)
#define LCD_CUSTOM_CHAR_3       (3)
#define LCD_CUSTOM_CHAR_4       (4)
#define LCD_CUSTOM_CHAR_5       (5)
#define LCD_CUSTOM_CHAR_6       (6)
#define LCD_CUSTOM_CHAR_7       (7)



// will need some helpers (send data, send cmd, send data, handle 4 bit, trig en)
lcd_ret_t lcd_init(lcd_cfg_t* lcd_cfg);
lcd_ret_t lcd_write_char(lcd_cfg_t* lcd_cfg, char c);
lcd_ret_t lcd_write_string(lcd_cfg_t* lcd_cfg, char* str);
lcd_ret_t lcd_clear_lcd(lcd_cfg_t* lcd_cfg);
lcd_ret_t lcd_set_cursor_pos(lcd_cfg_t* lcd_cfg, uint8_t row, uint8_t col);
lcd_ret_t lcd_save_custom_char(lcd_cfg_t* lcd_cfg, uint8_t* custom, uint8_t index);


