#include "bitBangHead.h"
//bit shifts right 1 bit, takes remainder of that which is our binary bit starting
//from the most significant position, checks this bit and writes in the order of
//rbg colors from R --> G --> B
void writeColor(int r, int g, int b)
{
    if(r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255)
    {
        int shift = 7;
        int count = 8;
        while(count > 0)
        {
            if( (r >> shift) % 2 == 1)
                write_1();
            else
                write_0();
            count--;
            shift--;
        }

        shift = 7;
        count = 8;
        while(count > 0)
        {
            if( (g >> shift) % 2 == 1)
                write_1();
            else
                write_0();
            count--;
            shift--;
        }

        shift = 7;
        count = 8;
        while(count > 0)
        {
            if( (b >> shift) % 2 == 1)
                write_1();
            else
                write_0();
            count--;
            shift--;
        }

        //oneMilliSec();
    }
}

void drawFrame(unsigned char frame)
{
    int byteRed = frame;
    int byteBlue = 255 - frame;
    writeColor(byteRed, 0, byteBlue);
}

unsigned long int packColor(unsigned char r, unsigned char g, unsigned char b)
{
    return (((long)r << 16) | ((long)g << 8) | ((long)b));
}

unsigned char getR(unsigned long int rgb)
{
    return ((unsigned char) (rgb >> 16));
}

unsigned char getG(unsigned long int rgb)
{
    return ((unsigned char) (rgb >> 8));
}

unsigned char getB(unsigned long int rgb)
{
    return ((unsigned char) (rgb >> 0));
}

void writePacked(unsigned long int packed)
{
    writeColor(getR(packed), getG(packed), getB(packed));
}

unsigned long int wheel(unsigned char pos)
{
    pos = 255 - pos;
    if(pos < 85)
    {
        return (packColor(255 - pos * 3, 0, pos * 3));
    }
    if(pos < 170)
    {
        pos -=85;
        return (packColor(0, pos * 3, 255 - pos * 3));
    }
    pos -= 170;
    return (packColor(pos * 3, 255 - pos * 3, 0));
}

void delay(int delay)
{
    while(delay > 0)
    {
        oneMilliSec();
        delay--;
    }
}