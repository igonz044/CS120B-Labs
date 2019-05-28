/* Nokia 5110 LCD AVR Library example
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

#include <avr/io.h>
#include <util/delay.h>

#include "nokia5110.h"

int main(void)
{
	unsigned short i;
    /*nokia_lcd_init();
    nokia_lcd_clear();
    nokia_lcd_write_string("IT'S WORKING!",1);
    nokia_lcd_set_cursor(5, 10);
    nokia_lcd_write_string("Nice!", 1);
	nokia_lcd_set_cursor(84, 10);
	nokia_lcd_write_string("hii", 1);
    nokia_lcd_render();*/
	
	nokia_lcd_init();
	
    for (;;) {
		nokia_lcd_clear();
		nokia_lcd_set_cursor(i, 20);
		custom_write_string("hello world!", 1);
		nokia_lcd_render();
        _delay_ms(200);
		i++;
		if(i > 60)
		{
			i = 0;
		}
	}
}
