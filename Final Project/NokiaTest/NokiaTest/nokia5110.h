/* Nokia 5110 LCD AVR Library
 *
 * Copyright (C) 2015 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by SkewPL, http://skew.tk
 */
#ifndef __NOKIA_5110_H__
#define __NOKIA_5110_H__

#include <avr/pgmspace.h>
#include <stdint.h>

//LCD port
#define PORT_LCD PORTD
#define DDR_LCD DDRD

//LCD pins
#define LCD_CLK PORTD0
#define LCD_DIN PORTD1
#define LCD_DC PORTD2
#define LCD_SCE PORTD3
#define LCD_RST PORTD4

#define LCD_CONTRAST 0x40

void custom_write_string(const char *str, uint8_t scale);
void custom_scroll(char code, uint8_t scale, uint8_t end);

//Must be called once before any other function, initializes display
void nokia_lcd_init(void);

//Clear Screen 
void nokia_lcd_clear(void);

/**
 * Power of display
 * @lcd: lcd nokia struct
 * @on: 1 - on; 0 - off;
 */
void nokia_lcd_power(uint8_t on);

/* Set single pixel
 * @x: horizontal position
 * @y: vertical position
 * @value: show/hide pixel*/
void nokia_lcd_set_pixel(uint8_t x, uint8_t y, uint8_t value);

/**
 * Draw single char with 1-6 scale
 * @code: char code
 * @scale: size of char
 */
void nokia_lcd_write_char(char code, uint8_t scale);

/**
 * Draw string. Example: writeString("abc",3);
 * @str: sending string
 * @scale: size of text
 */
void nokia_lcd_write_string(const char *str, uint8_t scale);

/**
 * Set cursor position
 * @x: horizontal position
 * @y: vertical position
 */
void nokia_lcd_set_cursor(uint8_t x, uint8_t y);

//Render screen to display
void nokia_lcd_render(void);

#endif
