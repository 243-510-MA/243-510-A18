//PAN

#include "pan.h"
#include "system.h"
#include "codes library.h"
#include "system_config.h"
#include "miwi/miwi_api.h"

void Pan(void)
{
    TRISB = 0x0;
    int status = 0;
    uint8_t rssi = 0;
    GYRO = 0;
    Buzzer = 0;
    
 /*  
    RCONbits.IPEN = 1;
    T4CON = 0b00111111;
    PR4 = 156;
    IPR3bits.TMR4IP = 0;
    INTCONbits.GIEH = 0;
    PIR3bits.TMR4IF = 0;
    
*/
    
    LCD_BKLT = 1;
    while(true)
    {
        
        
        if(MiApp_MessageAvailable())
        {
            MiApp_FlushTx();
            MiApp_WriteData(0x01);
            MiApp_BroadcastPacket(false);
            __delay_ms(100);
            
            LCD_BKLT = !LCD_BKLT;
            
            MiApp_DiscardMessage();
        }
       
    }
}