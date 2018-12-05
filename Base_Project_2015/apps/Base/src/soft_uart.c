#include "soft_uart.h"



void puteChar(char charactere){
    TRIS_TX = 0;
    LAT_TX = 0;
    
    __delay_us(DELAIDEFOU);
    
    for(int i = 0; i < 8 ; i++){
        LAT_TX = (charactere >> i) & 0x01;
        __delay_us(DELAIDEFOU);
    }
    
    LAT_TX = 1;
    __delay_us(DELAIDEFOU);
}

void putch(char txData)
{
    puteChar(txData);
}
