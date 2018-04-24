 
#ifndef joystick_H
#define	joystick_H "joysticklib.h"

void __attribute__((__interrupt__,__auto_psv__)) _ADC1Interrupt(void);
void joystickSetup(void);
int getJoystickDirection(void);

#endif

