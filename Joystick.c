#include <p24Fxxxx.h>
#include "xc.h"
#include "lcdlib.h"



//x will be an0, assign ADC1BUF 0
//y will be an1, assign ADC1BUF 1
volatile int xBuf, yBuf;

void __attribute__((__interrupt__,__auto_psv__)) _ADC1Interrupt(void)
{
        _AD1IF = 0;
    
    static int count;
    
    int check = count % 2;
    
    if(!check)
        xBuf = ADC1BUF0;
    else
        yBuf = ADC1BUF1;
    
    count++;
    
    count &= 3;
}

int getJoystickDirection(void)
{
    //left = 1, up = 2, right = 3, down = 4, center = 5
    int direction;
    
    AD1CON1bits.ASAM = 1; //start sample
    while(!IFS0bits.AD1IF){}; //wait until conversion time
    AD1CON1bits.ASAM = 0; //shut off sampling to start conversion

    
    if(xBuf > 520) //x-left
        direction = 1;
    else if(xBuf < 500) //x-right
        direction = 3;
    else if(yBuf > 550) //y-down
        direction = 4;
    else if(yBuf < 400) //y-up
        direction = 2;
    else 
        direction = 5;
    
    return direction;
}


void joystickSetup(void)
{
    TRISA = 0b11;
    
    //Setting up ADC
    AD1CON1 = 0;
    AD1CON2 = 0;
    AD1CON3 = 0;
    
    //Set ANx to scan
    AD1CON2bits.CSCNA = 1; //enable multiplexing
    AD1CON2bits.SMPI = 1;  //take 1 sample before interrupt
   // AD1CON2bits.BUFM = 1; //dual, 8 word segments
    AD1CON2bits.ALTS = 0; //set to use for MUX A
    AD1CON1bits.SSRC = 0b111;
    
    AD1CSSLbits.CSSL0 = 1; //AN0
    AD1CSSLbits.CSSL1 = 1; //AN1
    
    //Setup for TAD
    AD1CON3bits.ADCS = 1; //TAD = 2TCy = 125ns
    AD1CON3bits.SAMC = 17; //sample time = 
    
    //set interrupt flags
    _AD1IE = 1; //enable ad int
    _AD1IF = 0; //clear AD IF int
    
    _ADON = 1;
    
     //i2c setup
    I2C2CONbits.I2CEN = 0; //disable
    I2C2BRG = 0x9D; //100 kHz
    I2C2CONbits.I2CEN = 1; //enable
    _MI2C2IF = 0;
    
    //TIMER SETUP
    T1CON = 0;
    PR1 = 15999;
    TMR1 = 0;
    IFS0bits.T1IF = 0;
    T1CONbits.TON = 1;
    lcdinit();
    lcdSetCursor(0,0);
    lcdString("test");
}
