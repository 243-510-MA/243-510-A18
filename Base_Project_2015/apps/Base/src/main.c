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

#define BOARD_ID 2

#define PAN 0                      //LE TI MASTER RACE
#define BOARD_CAPTEUR 1            //DIT MOÉ LES DATAS G
#define FERMIER 2                  //DIT MOÉ LES DATAS G

//*************************************************************************

#define DEVICEMODE BOARD_CAPTEUR // Choisir ici

//*************************************************************************

//EUSART2 TX SUR PIN 5 AKA RP24 AKA I07
void main(void)
{
 
    
    SYSTEM_Initialize();
    
    Network(DEVICEMODE);
    
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

