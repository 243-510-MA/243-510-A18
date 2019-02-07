/* 
 * File:   newPAN.h
 * Author: Benoit
 *
 * Created on February 7, 2019, 3:28 PM
 */

#ifndef NEWPAN_H
#define	NEWPAN_H

#include <string.h>
#include "system.h"
#include "codes library.h"
#include "system_config.h"
#include "miwi/miwi_api.h"
#include "soft_uart.h"


#define TIMEOUT_COUNT 1000 //each increment of TIMER_COUNT represents 100us of time left for the sensors to answer, after that, you're too late for the party
//1000 = 100 ms = 0.1 s
//10000 = 1 s


void newPan();



#endif