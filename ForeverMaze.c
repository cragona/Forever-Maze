#include "xc.h"
#include <p24Fxxxx.h>
#include <stdint.h>
#include <stdlib.h>
#include "lcdlib.h"

#include "joysticklib.h"
#include "ws2812lib.h"


// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1 // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME // Clock Switching and Monitor (Clock switching is enabled,
 

// Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

#define PLAYERSPEED 60 //Period (1/FrameRate) in milliseconds

volatile static int playerPrevRow = 0, playerPrevCol = 0, playerRow = 0, playerCol = 0;
volatile static int maze[8][8];

void setup(void)
{
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0xFFFC; //set up AN0, AN1; Other pins digA 
    TRISB = 0x0011;     //RA0 output
    joystickSetup();
}

void mazeSetup(void)
{
    int i,j;
    int r = rand() % 20; //generates random number b/w 0-19
    
    if(r == 0)
    {
        volatile int premade[8][8] = {
                {0,1,1,1,1,1,1,1},
				{0,1,1,1,1,1,1,1},
				{0,0,0,0,0,0,1,1},
				{1,1,1,1,1,0,0,0},
				{1,1,1,1,1,1,1,0},
				{1,1,1,1,1,1,1,0},
				{1,1,1,1,1,1,1,0},
				{1,1,1,1,1,1,1,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 1)
    {
        volatile int premade[8][8] = {
                {0,0,0,0,0,0,0,0},
				{1,1,0,1,1,1,1,0},
				{1,1,0,1,1,1,1,0},
				{1,1,0,1,1,1,1,0},
				{1,1,0,1,1,1,1,0},
				{1,1,0,1,1,1,1,0},
				{1,1,0,0,0,0,1,0},
				{1,1,1,1,1,1,1,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 2)
    {
        volatile int premade[8][8] = {
                {0,1,1,1,1,1,1,0},
				{0,1,1,1,1,1,1,1},
				{0,1,1,1,1,1,1,1},
				{0,0,0,0,0,0,0,0},
				{1,1,1,0,1,1,1,0},
				{0,0,0,0,1,1,1,0},
				{0,1,0,0,1,1,1,0},
				{0,0,0,0,1,1,1,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 3)
    {
        volatile int premade[8][8] = {
                {0,1,1,1,1,1,1,1},
				{0,0,1,1,1,1,1,1},
				{1,0,1,1,1,1,1,1},
				{1,0,0,0,0,0,0,0},
				{0,1,1,1,1,1,1,0},
				{0,1,1,1,1,1,1,0},
				{0,1,1,1,1,1,1,0},
				{0,1,1,1,1,1,1,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 4)
    {
        volatile int premade[8][8] = {
                {0,0,1,0,1,0,1,0},
				{0,0,1,0,1,0,0,0},
				{0,0,1,0,1,0,1,0},
				{0,0,1,0,1,0,0,0},
				{0,0,0,0,0,0,1,0},
				{1,0,1,1,1,0,0,0},
				{1,0,1,1,1,0,0,0},
				{1,0,1,1,1,0,1,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 5)
    {
        volatile int premade[8][8] = {
                {0,0,0,0,1,1,1,1},
				{0,1,1,1,0,1,1,1},
				{0,0,0,0,0,0,1,1},
				{0,0,0,1,0,1,0,1},
				{0,0,0,0,0,0,1,0},
				{0,1,1,1,0,1,0,1},
				{0,0,0,0,0,0,1,1},
				{1,1,0,0,0,0,0,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 6)
    {
        volatile int premade[8][8] = {
                {0,0,0,0,0,0,1,1},
                {1,1,0,1,1,0,0,0},
                {0,0,0,1,1,0,1,1},
                {0,1,1,1,0,0,0,1},
                {0,0,1,1,0,1,0,0},
                {1,0,0,1,0,1,0,1},
                {1,1,0,1,0,1,0,1},
                {1,1,0,0,0,1,0,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 7)
    {
        volatile int premade[8][8] = {
                {0,1,0,0,0,1,0,1},
                {0,1,0,1,0,0,0,1},
                {0,0,0,1,0,1,0,1},
                {1,1,1,1,0,1,0,0},
                {0,0,0,0,0,1,0,1},
                {0,1,1,1,1,1,1,1},
                {0,1,1,0,0,0,1,1},
                {0,0,0,0,1,0,0,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 8)
    {
        volatile int premade[8][8] = {
                {0,0,1,1,0,0,0,0},
                {1,0,0,1,1,0,1,0},
                {1,0,1,1,0,0,1,0},
                {0,0,1,0,0,1,1,0},
                {0,1,1,0,1,1,0,0},
                {0,1,0,0,1,0,0,1},
                {0,1,1,0,1,1,0,1},
                {0,0,0,0,1,1,0,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 9)
    {
        volatile int premade[8][8] = {
                {0,1,1,0,0,0,0,0},
                {0,1,1,0,1,0,1,0},
                {0,0,0,0,1,0,1,0},
                {1,1,1,1,1,0,1,0},
                {1,0,0,0,0,0,1,1},
                {1,0,1,1,1,1,0,0},
                {0,0,1,0,0,0,0,0},
                {1,0,0,0,1,1,1,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 10)
    {
        volatile int premade[8][8] = {
                {0,1,1,0,0,0,0,0},
                {0,1,1,0,1,0,1,0},
                {0,0,0,0,1,0,1,0},
                {1,1,1,1,1,0,1,0},
                {1,0,0,0,0,0,1,1},
                {1,0,1,1,1,1,0,0},
                {0,0,1,0,0,0,0,0},
                {1,0,0,0,1,1,1,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 11)
    {
        volatile int premade[8][8] = {
                {0,1,1,0,0,0,1,0},
                {0,0,1,0,1,0,1,0},
                {0,1,1,0,1,0,0,0},
                {0,1,0,0,1,1,1,0},
                {0,1,0,1,1,0,0,0},
                {0,1,0,1,0,0,1,1},
                {0,1,0,1,0,1,1,1},
                {0,0,0,1,0,0,0,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 12)
    {
        volatile int premade[8][8] = {
                {0,0,0,0,0,0,1,1},
                {1,0,0,1,0,0,0,0},
                {1,0,0,1,1,1,1,1},
                {0,0,1,1,0,0,0,0},
                {0,0,0,0,1,0,1,0},
                {0,1,0,1,0,1,1,0},
                {1,1,0,1,0,0,0,1},
                {0,1,0,0,0,1,0,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 13)
    {
        volatile int premade[8][8] = {
                {0,1,1,1,1,1,1,1},
                {0,0,0,1,0,0,0,0},
                {1,0,0,1,0,1,1,0},
                {1,0,1,1,0,1,0,0},
                {1,0,1,1,0,1,0,1},
                {1,0,1,0,0,1,0,0},
                {1,0,0,0,0,0,1,0},
                {1,1,1,1,1,1,1,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 14)
    {
        volatile int premade[8][8] = {
                {0,0,0,1,1,0,0,1},
                {1,1,0,0,0,0,1,0},
                {0,1,0,0,1,1,1,1},
                {1,0,1,0,0,0,0,0},
                {1,0,0,0,1,1,0,1},
                {1,0,0,0,0,1,0,0},
                {1,1,1,0,0,0,1,0},
                {0,1,0,0,1,1,0,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 15)
    {
        volatile int premade[8][8] = {
                {0,1,1,0,0,0,0,1},
                {0,0,0,0,1,1,0,1},
                {1,1,0,0,1,0,0,0},
                {0,1,1,0,0,1,1,0},
                {0,0,0,1,0,0,1,0},
                {0,1,0,0,0,1,0,0},
                {0,1,0,1,0,1,0,1},
                {0,0,0,1,1,1,0,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 16)
    {
        volatile int premade[8][8] = {
                {0,0,0,0,1,1,1,0},
                {1,0,1,0,0,1,1,1},
                {0,1,1,1,0,0,0,0},
                {0,0,1,0,0,0,1,0},
                {1,0,1,0,1,0,1,0},
                {0,1,1,0,0,1,0,0},
                {0,1,1,1,0,1,0,1},
                {0,0,0,0,1,1,0,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 17)
    {
        volatile int premade[8][8] = {
                {0,1,1,1,1,1,1,1},
				{0,0,0,0,1,1,1,1},
				{1,1,0,0,0,0,0,1},
				{0,1,0,1,1,1,0,1},
				{0,0,0,1,1,1,0,1},
				{0,0,1,1,1,1,0,1},
				{1,0,1,1,1,1,0,0},
				{1,0,1,1,1,1,1,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 18)
    {
        volatile int premade[8][8] = {
                {0,0,0,0,0,0,0,0},
				{1,1,1,1,1,1,1,0},
				{0,0,0,0,0,0,0,0},
				{0,1,1,1,1,1,1,1},
				{0,0,0,0,0,0,0,0},
				{1,1,1,1,1,1,1,0},
				{1,1,1,1,1,1,1,0},
				{1,1,1,1,1,1,1,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    else if(r == 19)
    {
        volatile int premade[8][8] = {
                {0,1,0,0,0,0,1,1},
				{0,1,0,1,1,0,1,1},
				{0,1,0,1,1,0,1,1},
				{0,1,0,1,1,0,0,0},
				{0,1,0,1,1,1,1,0},
				{0,1,0,0,0,0,1,0},
				{0,1,1,1,1,0,1,0},
				{0,0,0,0,0,0,1,0}
        };
        
        for(i = 0; i < 8; i++)
        {
            for(j = 0; j < 8; j++)
            {
                maze[i][j] = premade[i][j];
            }
        }
    }	
    
    maze[playerRow][playerCol] = 8;
}

void writeMaze(void)
{
    int i, j;
    
    static int red = 0, blue = 0, green = 0, breath = 0;
    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
            if(maze[i][j] == 1)
                writeColor(green, red, blue);
            else if(maze[i][j] == 8)
                writeColor(22,0,0);
            else if(i == 7 && j == 7)      
                writeColor(0,22,22);     
            else
                writeColor(0, 0, 0);
        }
    }
    
    if(breath <= 30) //blue in and out
    {
        if(breath > 15)
            blue--;
        else
            blue++;
    }
    else if(breath <= 60 && breath > 30)
    {
        if(breath > 45)
            red--;
        else
            red++;   
    }
    else if(breath > 60 && breath <= 90)        
    {    
        if(breath > 75)
            green--;
        else
            green++;
    }
    
    if(breath == 90)
        breath = 0;
        
    breath++;
      
    delay(1); //set hold/reset for maze
}

void checkPlayer(int direction)
{
    //left = 3, up = 2, right = 1, down = 4, center = 5
    playerPrevRow = playerRow;
    playerPrevCol = playerCol;
    
    switch(direction)
    {
        case 1://right
            if(playerCol == 7)//out of bounds
                break;
            else if(maze[playerRow][playerCol + 1] == 1) //wall
                break;
            else
                playerCol++; //move right
            break;
            
        case 2://up
        if(playerRow == 0)//out of bounds move
            break;
        else if(maze[playerRow - 1][playerCol] == 1)//wall
            break;
        else
            playerRow--; //move up
        return;
        
        case 3://left
            if(playerCol == 0)//out of bounds move
                break;
            else if(maze[playerRow][playerCol - 1] == 1)//wall
                break;
            else
                playerCol--; //move left
            break;
            
        case 4://down
            if(playerRow == 7)
                break;
            else if(maze[playerRow + 1][playerCol] == 1)
                break;
            else
                playerRow++;
            break;
            
        default: //5 - center
            break;
    }
}

void updateMaze(int direction)
{
    checkPlayer(direction);
    maze[playerPrevRow][playerPrevCol] = 0;
    maze[playerRow][playerCol] = 8;
}

int main(void) {
    setup();
    static int playerDirection;
 
    while(1)
    {
        playerRow = 0; //start row
        playerCol = 0; //start col
        srand(time(NULL)); //change seed every pass
        mazeSetup();
        
        while(maze[7][7] != 8)
        {
            playerDirection = getJoystickDirection();
            updateMaze(playerDirection);
            writeMaze();

            if(playerDirection == 3) //x-left
                lcdString("left");
            else if(playerDirection == 1) //x-right
                lcdString("right");
            else if(playerDirection == 4) //y-down
                lcdString("down");
            else if(playerDirection == 2) //y-up
                lcdString("up");
            else 
                lcdString("no move");

            delay(PLAYERSPEED);

            lcd_cmd(1); //clear display
        }

    }
    return 0;
}
