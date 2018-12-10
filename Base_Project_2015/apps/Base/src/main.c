//MAIN

#include "stdio.h"
#include "system.h"
#include "codes library.h"
#include "system_config.h"
#include "miwi/miwi_api.h"
#include "network.h"

#include "CapteurI2C.h"
#include "soft_uart.h"

#include "fermier.h"
#include "pan.h"
#include "Board_Capteur.h"

#define BOARD_ID 1

#define PAN 0                      //LE TI MASTER RACE
#define BOARD_CAPTEUR 1            //DIT MOÉ LES DATAS G
#define FERMIER 2                  //DIT MOÉ LES DATAS G

//*************************************************************************

#define DEVICEMODE PAN // Choisir ici

//*************************************************************************

//EUSART2 TX SUR PIN 5 AKA RP24 AKA I07
void main(void)
{
    __delay_ms(750);
    
    SYSTEM_Initialize();
    
    Network(DEVICEMODE);
    
    LCD_Erase();
				sprintf((char *)&LCDText[0], (char*)"Dumping EEPROM");
				LCD_Update();
    
    if(DEVICEMODE == PAN)
    {
        Pan(); 
    }
    
    else if(DEVICEMODE == BOARD_CAPTEUR)
    {
        Board_Capteur_Loop(BOARD_ID);   
    }
    
    else if(DEVICEMODE == FERMIER)
    {
        fermier();
    }
    
    while(1);
    
}

