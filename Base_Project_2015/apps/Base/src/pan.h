/********************************************************************
 Software License Agreement:

 The software supplied herewith by Microchip Technology Incorporated
 (the "Company") for its PIC(R) Microcontroller is intended and
 supplied to you, the Company's customer, for use solely and
 exclusively on Microchip PIC Microcontroller products. The
 software is owned by the Company and/or its supplier, and is
 protected under applicable copyright laws. All rights are reserved.
 Any use in violation of the foregoing restrictions may subject the
 user to criminal sanctions under applicable laws, as well as to
 civil liability for the breach of the terms and conditions of this
 license.

 THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION. NO WARRANTIES,
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *******************************************************************/
#include "system.h"
#include "codes library.h"
#include "system_config.h"
#include "miwi/miwi_api.h"
#include "EEPROM.h"
#include "RTCSPI.h"
#include "math.h"

#ifndef _PAN_H
    #define _PAN_H
	
//#define PAN_ADDRESS 0x00
#define CARTE1_ADDRESS 0x01
#define CARTE2_ADDRESS 0x02
#define CARTE3_ADDRESS 0x03
//#define CARTE4_ADDRESS 0x04
//#define CARTE5_ADDRESS 0x05
//#define CARTE6_ADDRESS 0x06

#define BoutonSetTime PORTBbits.RB1
#define BoutonDumpData_plusOk PORTBbits.RB2
#define BoutonMoinsNo PORTBbits.RB3
   
void getTime (void);

void Pan(void);
void dumpData (void);
void eraseData (void);
void setTimeMenue (void);
uint32_t EEPROMinit (void);

#endif
