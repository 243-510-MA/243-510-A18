#include "newPan.h"

short RA2_lastState = 0;

uint8_t frameCount = 0;

char txBuffer[1000];

void newPan(){
    
    TRISAbits.TRISA2 = 1; //Sets RA2 as input
    
    while(1){
        
        //Always empty the f***k out of the buffer, otherwise shit happens
        if(MiApp_MessageAvailable()){
            MiApp_DiscardMessage();
        }
        
        //Triggers on a rising edge of RA2
        if( (PORTAbits.RA2 == 1) && (RA2_lastState == 0) ){
            
            char tempBuffer[21];
            //Wake up everyone and request some gud gud data to be sent to me
            MiApp_FlushTx();
            MiApp_WriteData(0x01);
            MiApp_WriteData(0xDE);
            MiApp_WriteData(0xAD);  //That's the trigger word, keeps the system alive, because you know it says DEAD in hex................
            MiApp_BroadcastPacket(false);
            
            frameCount = 0;
            
            txBuffer[0] = '\0';
            //memset(txBuffer, '\0', sizeof(txBuffer)); //That's the hardcore way to clear the string
            
            
            //loop for timeout
            for(int i = 0; i < TIMEOUT_COUNT ; i++){
                if(MiApp_MessageAvailable()){
                    //Standard Answer Identifier
                    if(rxMessage.Payload[0]==0x02){
                        frameCount++;
                        sprintf(tempBuffer,"%d;%d;%d;%d;",rxMessage.Payload[1],rxMessage.Payload[2],rxMessage.Payload[3],rxMessage.Payload[4]);
                        strcat(txBuffer, tempBuffer);
                    }
                    MiApp_DiscardMessage();
                }
                
                if(frameCount >= 50){
                    break;
                }
                __delay_us(100);
            }
        }
        RA2_lastState = PORTAbits.RA2;
    }
}
