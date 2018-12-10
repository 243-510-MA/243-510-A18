#ifndef _SOFT_UART_H
#define	_SOFT_UART_H

#include <system.h>

#define TRIS_TX TRISAbits.TRISA2
#define LAT_TX PORTAbits.RA2

#define DELAIDEFOU 97

void puteChar(char charactere);


#endif