#include "Arduino.h"
#include "SClock.h"

void SClock::mhz() {
    DDRB = 0xFF;
    while (1)
    {
       PORTB = ~PINB;
        
        __asm__("nop\n\t");   
        __asm__("nop\n\t");

     }
}

