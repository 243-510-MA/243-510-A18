#ifndef _SOFT_UART_H
#define	_SOFT_UART_H

#include <system.h>

#define TRIS_TX TRISAbits.TRISA0
#define LAT_TX PORTAbits.RA0

#define DELAIDEFOU 93

void puteChar(char charactere);


#endif