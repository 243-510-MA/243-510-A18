//MAIN

#include "stdio.h"
#include "system.h"
#include "codes library.h"
#include "system_config.h"
#include "miwi/miwi_api.h"
#include "network.h"
#include "pan.h"
#include "student.h"
#include "teacher.h"
#include "door_unlock.h"
#include "projector_screen.h"
#include "CapteurI2C.h"
#include "eusart2.h"

#define PAN 0                       //Controls the projector, red light, buzzer and motion sensor (such many thing, wow)
#define DOOR_UNLOCK 1               //Unlock the door (duh)
#define PROJECTOR_SCREEN 2          //DROP DA PROJECTOR
#define TEACHER 3                   //Mista ze teacha
#define STUDENT 4                   //You little shit

//*************************************************************************

#define DEVICEMODE PAN // Choisir ici

//*************************************************************************

//EUSART2 TX SUR PIN 5 AKA RP24 AKA I07
void main(void)
{ 
    
    INTCONbits.GIE = 0;
    
    EECON2 = 0x55;
    
    EECON2 = 0xAA;
    
    PPSCON = 0;
            
    RPOR24 = 0x05;
    
    EECON2 = 0x55;
    
    EECON2 = 0xAA;
    
    PPSCONbits.IOLOCK = 1;
    
    RCSTA2bits.SPEN = 1;
    
    TRISDbits.TRISD7 = 0;
    
    EUSART2_Initialize();
    
    SYSTEM_Initialize();


    
    

    
    while(1){
        printf("Hello");
        LATAbits.LA6 = !LATAbits.LA6;
        
        /*sprintf((char *)&LCDText, (char*)"Temp : %.2f", readTemperatureC());
        sprintf((char *)&LCDText[16], (char*)"WetNES : %.2f", readHumidity());
        LCD_Update();*/
        __delay_ms(100);
    }
    
    LED0 = LED1 = LED2 = 0;
    
    
/* 
    Network(DEVICEMODE);
 
    if(DEVICEMODE == PAN)
    {
        Pan();     
    }   


    if(DEVICEMODE == DOOR_UNLOCK)
    {
        DoorUnlock();
    }


    if(DEVICEMODE == PROJECTOR_SCREEN)
    {
        ProjectorScreen();
    }


    if(DEVICEMODE == TEACHER)
    {
        Teacher();
    }


    if(DEVICEMODE == STUDENT)
    {
        Student();
    }
 */
}