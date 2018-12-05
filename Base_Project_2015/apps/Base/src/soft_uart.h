#ifndef _SOFT_UART_H
#define	_SOFT_UART_H

#include <system.h>

#define TRIS_TX TRISE1
#define LAT_TX LATE1

#define DELAIDEFOU 97

void puteChar(char charactere);


#endif