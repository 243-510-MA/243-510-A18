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

#define PAN 0                       //Controls the projector, red light, buzzer and motion sensor (such many thing, wow)
#define BOARD_CAPTEUR 1               //Unlock the door (duh)
#define PROJECTOR_SCREEN 2          //DROP DA PROJECTOR
#define TEACHER 3                   //Mista ze teacha
#define STUDENT 4                   //You little shit

//*************************************************************************

#define DEVICEMODE PAN // Choisir ici

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
    
    
/*
    Network(DEVICEMODE);
 
    if(DEVICEMODE == PAN)
    {
        Pan();     
    } 
 */
}

