#include "bitBangHead.h"
#include "ws2812lib.h"
#include <stdlib.h>

int maze[8][8];

void mazeSetup(void)
{
    int i, j;
    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
            maze[i][j] = 1;
        }
    }
    
    for(i = 0; i < 11; i++)
    {  
        maze[rand() % 8][rand() % 8] = 0; 
    }
}

void turnOnMaze(void)
{
    int i, j;
    for(i = 0; i < 8; i++)
    {
        for(j = 0; j < 8; j++)
        {
            if(maze[i][j] == 1)
                writeColor(0,0,0);
            else
                writeColor(0, 44, 0);
        }
    }
}