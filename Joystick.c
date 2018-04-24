#include <p24Fxxxx.h>
#include <xc.h>

// PIC24FJ64GA002 Configuration Bit Settings
// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled. 
					// Primary Oscillator refers to an external osc connected to the OSC1 and OSC2 pins)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // OSC2/CLKO/RC15 functions as port I/O (RC15)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))


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


void setup(void)
{
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0xFFFC; //set up AN0, AN1; Other pins digA
    
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
}
int main(void) {
    setup();
    
    //lcd test stuff
    TRISA = 0b11;
    TRISBbits.TRISB6 = 0;
    
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
    lcdString("2");
    
    while(1)
    {

        AD1CON1bits.ASAM = 1; //start sample
        while(!IFS0bits.AD1IF){}; //wait until conversion time
        AD1CON1bits.ASAM = 0; //shut off sampling to start conversion
 
        int i;
        for(i = 200; i > 0; i--)
        {
            while(!IFS0bits.T1IF){}
            _T1IF = 0;
        }
        
        
        if(xBuf > 520) //x-left
            lcdString("x-right");
        else if(xBuf < 500) //x-right
            lcdString("x-left");
        else if(yBuf > 550) //y-down
            lcdString("y-down");
        else if(yBuf < 400) //y-up
            lcdString("y-up");
        else 
            lcdString("center");
        
        for(i = 200; i > 0; i--)
        {
            while(!IFS0bits.T1IF){}
            _T1IF = 0;
        }
        lcd_cmd(1); //clear display
        
        
    }
    return 0;
}
