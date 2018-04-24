 
#ifndef LCD_H
#define	LCD_H "lcdlib.h"

void lcd_cmd(char c);
void lcdinit(void);
void lcdSetCursor(char row, char col);
void lcdPrintChar(char c);
void lcdString(char *s);

#endif

