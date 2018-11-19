//MAIN

#include "stdio.h"
#include "system.h"
#include "codes library.h"
#include "system_config.h"
#include "miwi/miwi_api.h"
#include "network.h"

#include "pan.h"

#include "CapteurI2C.h"
#include "eusart2.h"

#include "student.h"

#define PAN 0                      //LE TI MASTER RACE
#define BOARD_CAPTEUR 1            //DIT MOÉ LES DATAS G

//*************************************************************************

#define DEVICEMODE BOARD_CAPTEUR // Choisir ici

//*************************************************************************

//EUSART2 TX SUR PIN 5 AKA RP24 AKA I07
void main(void)
{ 
    //BOUGEZ PAS SA DE LA MERCI BYE 
    if(DEVICEMODE == PAN){
        EUSART2_Initialize();
    }
    
    SYSTEM_Initialize();

    
    LED0 = LED1 = LED2 = 0;
    
    
    Network(DEVICEMODE);
    
    sprintf((char *)&LCDText, (char*)"CACACAC");
    LCD_Update();
    
    LED0 = LED1 = LED2 = 1;
 
    if(DEVICEMODE == PAN)
    {
        Pan();     
    }
    

    else if(DEVICEMODE == BOARD_CAPTEUR)
    {
        Board_Capteur_Loop();   
    }
    
    while(1);
    
}

