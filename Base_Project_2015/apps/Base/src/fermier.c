//TEACHER

#include "fermier.h"
#include "system.h"
#include "codes library.h"
#include "system_config.h"
#include "miwi/miwi_api.h"

void fermier(void)
{
    uint8_t switch_val = 0;
    extern uint8_t ConnectionEntry;
    bool Tx_Packet = true;
    uint8_t rssi = 0;
    uint8_t memoire = 0;
    uint8_t Pkt_Loss_Cnt = 0;
    uint8_t questionnaire = 0;
    uint8_t temp = 0;
    uint8_t address = 0;
    uint8_t reponseb = 0;
    uint8_t reponsec = 0;
    uint8_t reponsed = 0;
    LCD_Erase();
    LCD_Display((char *)" FERMIER DEVICE ", 0, true);
   
    while(true)
    {
        
        sprintf((char *)&LCDText, (char*)"SW1: DONNEE1    ");
        LCD_Update();
        
       // sprintf((char *)&LCDText, (char*)"SW1: PROJECTOR  SW2: Suivant  ");
       // LCD_Update();
        
        while(switch_val == 0)
        {
            switch_val = BUTTON_Pressed();
        }
        
       
        
       
            
          
            
            if(switch_val == SW1)
            {
                switch_val = 0;
                sprintf((char *)&LCDText, (char*)"LECTURE DONNEE    ");
                LCD_Update();
            
                while(switch_val == 0)
                {
                    //switch_val = BUTTON_Pressed();
                    questionnaire=1;
                    switch_val = 0;

                            if(questionnaire==1 && switch_val == 0)
                            {
                                switch_val = BUTTON_Pressed();
                                sprintf((char *)&LCDText[21], (char*)"/ %d  %02x", temp,address);
                               // sprintf((char *)&LCDText, (char*)"T: %d C   ",temp);
                                LCD_Update();
                                delay_ms(100);
                                if(MiApp_MessageAvailable())
                                {
                                    if(rxMessage.Payload[0] == CAPTEUR1)
                                    {
                                        temp=rxMessage.Payload[1] ;
                                        address= rxMessage.Payload[2] && rxMessage.Payload[3] ;
                                    }
                                   
                                MiApp_DiscardMessage();
                                }
                                
                            }
                        }

                    }
                }
            }    
      //  }        
   // }
//}
/*     
            
            
            
    while(true)
    {
        
        bool Run_Demo = true;
        bool Tx_Packet = true;
        uint8_t rssi = 0;
        uint8_t Pkt_Loss_Cnt = 0;
        MIWI_TICK tick1, tick2;
        uint8_t switch_val;
        uint8_t pktCmd = 0;

        //*******************************************************************
        // Dispaly Door Unlock Splach Screen
        //*******************************************************************
        LCD_Display((char *)"   Microchip       Door Unlock  ", 0, true);

        //*******************************************************************
        // Read Start tickcount
        //*******************************************************************	
        tick1 = MiWi_TickGet();

        while(Run_Demo)
        {	
            //*******************************************************************
            // Read current tickcount
            //*******************************************************************
            tick2 = MiWi_TickGet();

            // Send a Message
            //((MiWi_TickGetDiff(tick2,tick1) > (ONE_SECOND * TX_PKT_INTERVAL)))
            if((MiWi_TickGetDiff(tick2,tick1) > (ONE_MILLI_SECOND * 1000)))
            {
                //LCD_Erase();

                if(Tx_Packet)
                {	
                        //LCD_Display((char *)"Checking Signal Strength...", 0, true);

                    MiApp_FlushTx();

                    MiApp_WriteData(RANGE_PKT);
                    MiApp_WriteData(0x4D);
                    MiApp_WriteData(0x69);
                    MiApp_WriteData(0x57);
                    MiApp_WriteData(0x69);
                    MiApp_WriteData(0x20);
                    MiApp_WriteData(0x52);
                    MiApp_WriteData(0x6F);
                    MiApp_WriteData(0x63);
                    MiApp_WriteData(0x6B);
                    MiApp_WriteData(0x73);
                    MiApp_WriteData(0x21);

                    if( MiApp_UnicastConnection(ConnectionEntry, false) == false )
                        Pkt_Loss_Cnt++;
                    else
                        Pkt_Loss_Cnt = 0;

                    Tx_Packet = false;
                }

                else
                {	      	
                    if(Pkt_Loss_Cnt < 1)
                    {           		
                        if(rssi < 50) //200)
                        {
               
                        }    
                    }

                        else
                        {
                        }
                    Tx_Packet = true;

                } 	     
                //*******************************************************************
                // Read New Start tickcount
                //*******************************************************************
                tick1 = MiWi_TickGet(); 

            }



            // Check if  Switch Pressed
            switch_val = BUTTON_Pressed();

            if((switch_val == SW1) || (switch_val == SW2))
            {
                /*******************************************************************
                // Send Exit Demo Request Packet and exit Door Unlock
                /*******************************************************************
                MiApp_FlushTx();    
                MiApp_WriteData(EXIT_PKT);
                MiApp_UnicastConnection(ConnectionEntry, false);

                LCD_BacklightON();
                LCD_Display((char *)"   Exiting....     Door Unlock  ", 0, true);
                LCD_BacklightOFF();

                tick1 = MiWi_TickGet();
                // Wait for ACK Packet
                while(Run_Demo)
                {
                    if(MiApp_MessageAvailable())
                    {
                        if(rxMessage.Payload[0] == ACK_PKT)          
                            Run_Demo = false;

                        MiApp_DiscardMessage();
                    }
                    if ((MiWi_TickGetDiff(tick2,tick1) > (ONE_SECOND * EXIT_DEMO_TIMEOUT)))
                        Run_Demo = false;

                    tick2 = MiWi_TickGet();
                }    
            }*//*
        }
    }
}
    	  
*/
    

    	    