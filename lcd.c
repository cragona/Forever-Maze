/*
 * File:   lcd.c
 * Author: crago
 *
 * Created on March 26, 2018, 10:26 AM
 */

#include "xc.h"
#include <p24Fxxxx.h>
#include <stdio.h>
#include <string.h>
#include "lcdlib.h"

void lcd_cmd(char c)
{
  
    I2C2CONbits.SEN = 1; //send start signal

    while(I2C2CONbits.SEN); //wait for start to be processed

    I2C2TRN = 0x7C;  //send the address to
        
    while(I2C2STATbits.TRSTAT); //wait for interrupt/ack
 
    I2C2TRN = 0x0; //send control bit
  
     while(I2C2STATbits.TRSTAT); //wait for interrupt/ack
         
    I2C2TRN = c; //send control bit
 
     while(I2C2STATbits.TRSTAT); //wait for interrupt/ack
    
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN); //wait for stop to be processed
}

void lcdinit(void)
{
    int i;

    //50 ms delay
    for(i = 0; i < 50; i++){
        while (!IFS0bits.T1IF) ;
        IFS0bits.T1IF = 0;
    }
    
    lcd_cmd(0b00111000); //pass
    lcd_cmd(0b00111001); //pass
    lcd_cmd(0b00010100); //passes now
    lcd_cmd(0b01110000); //contrast c3-c0
    lcd_cmd(0b01011110); //c5 - c4 - ion - bon
    lcd_cmd(0b01101100); //control

    //200ms delay
    for(i = 0; i < 200; i++){       
        T1CONbits.TON = 1;
        while (!IFS0bits.T1IF) ;
        IFS0bits.T1IF = 0;
    } 
    
    lcd_cmd(0b00111000);
    lcd_cmd(0b00001100);
    lcd_cmd(0b00000001);
    
    //2ms delay
    for(i = 0; i < 2; i++)
    {    
        T1CONbits.TON = 1;
        while (!IFS0bits.T1IF) ;
        IFS0bits.T1IF = 0;
    }
}

void lcdSetCursor(char row, char col)
{
    int rowValue, calc;
    
    //find our first variable of the formula
    if(row == 0)
        rowValue = 0x00;
    else if(row == 1)
        rowValue = 0x40;
    
    calc = (rowValue * row) + col;
    
    calc |= 0x80; //set MSB to 1
    
    lcd_cmd(calc); //send away to the lcd
}

void lcdPrintChar(char c)
{
    I2C2CONbits.SEN = 1; //send start signal
    
    while(I2C2CONbits.SEN); //wait for start to be processed
    
    I2C2TRN = 0x7C;  //send the address to
    
    while(I2C2STATbits.TRSTAT); //wait for interrupt/ack
    
    I2C2TRN = 0x40; //send control bit
    
    while(I2C2STATbits.TRSTAT); //wait for interrupt/ack
    
    I2C2TRN = c; //send c
    
    while(I2C2STATbits.TRSTAT); //wait for interrupt/ack
    
    I2C2CONbits.PEN = 1;
    
    while(I2C2CONbits.PEN); //wait for stop to be processed
}

void lcdString(char *s)
{
    I2C2CONbits.SEN = 1; //send start signal
    
    while(I2C2CONbits.SEN); //wait for start to be processed
    
    I2C2TRN = 0x7C;  //send the address to
    
    while(I2C2STATbits.TRSTAT); //wait for interrupt/ack
    
    int i;
    
    int stop = strlen(s);
    
    //send every letter but last in
    for(i = 0;  i < stop-1; i++)
    {
        I2C2TRN = 0xC0; //send control bit
  
        while(I2C2STATbits.TRSTAT); //wait for interrupt/ack
        
        I2C2TRN = s[i];
        
        while(I2C2STATbits.TRSTAT); //wait for interrupt/ack
    }
    
    //send last letter in
    I2C2TRN = 0x40; //send control bit
  
    while(I2C2STATbits.TRSTAT); //wait for interrupt/ack
        
    I2C2TRN = s[i];
    
    while(I2C2STATbits.TRSTAT); //wait for interrupt/ack
    
    I2C2CONbits.PEN = 1; //stop
    
    while(I2C2CONbits.PEN); //wait for stop to be processed
}
