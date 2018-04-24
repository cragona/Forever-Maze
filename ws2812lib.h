 
#ifndef ws2812_H
#define	ws2812_H "ws2812lib.h"

void writeColor(int r, int g, int b);
void drawFrame(unsigned char frame);
unsigned long int packColor(unsigned char r, unsigned char g, unsigned char b);
unsigned char getR(unsigned long int rgb);
unsigned char getG(unsigned long int rgb);
unsigned char getB(unsigned long int rgb);
void writePacked(unsigned long int packed);
unsigned long int wheel(unsigned char pos);
void delay(int delay);

#endif

