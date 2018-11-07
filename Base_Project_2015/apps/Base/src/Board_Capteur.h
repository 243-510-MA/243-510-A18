#include "system.h"
#include "system_config.h"
#include "miwi/miwi_api.h"

#ifndef BOARD_CAPTEUR_H
#define	BOARD_CAPTEUR_H

#define NUM_TEMP_SAMPLES 5

/*********************************************************************
* Function:         uint16_t Read_VBGVoltage(void)
*
* PreCondition:     none
*
* Input:            none
*
* Output:           ADRES
*
* Side Effects:	    none
*
* Overview:         Reads the band gap voltage and compares with reference voltage
*                   to arrive at the current voltage level
*
* Note:			    
**********************************************************************/
uint16_t Read_VBGVoltage(void);
void CaronSendTemp(void);



#endif	/* BOARD_CAPTEUR_H */

