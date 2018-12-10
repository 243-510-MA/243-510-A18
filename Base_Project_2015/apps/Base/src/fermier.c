/* module FERMIER

 Permet l'affichage des capteurs et des données sur l'afficheur, 
 ainsi que la navigation.

 Sur la premiere ligne est affichée le numero du capteur, sur la deuxieme ligne 
 sont affichés, les unes apres les autres, les données du capteur selectionné.

 On peut naviguer entre les differents capteurs grace aux boutons SW1 (capteur précédent)
 et SW2 (capteur suivant) */


//----------------------- defines et includes ----------------------------------
#include "fermier.h"
#include "system.h"
#include "codes library.h"
#include "system_config.h"
#include "miwi/miwi_api.h"
#include "EEPROM.h"

#define switch1   1
#define switch2   2

//----------------------- declaration des fonctions ----------------------------
void affichage (void);
void asking_data(void);
void receiving_data(void);

//----------------------- declaration des variables ----------------------------
int etat = 1;
uint8_t flag=1;
int8_t temperature1;
int8_t temperature2;
int8_t temperature3;
int8_t temperature4;
int8_t temperature5;
int8_t temperature6;
int8_t humidite1;
int8_t humidite2;
int8_t humidite3;

// fonction fermier, a chaque appel de la fonction, celle-ci envoit une trame
// pour demander aux capteurs d'envoyer leurs datas, puis elle stocke les données
// recues dans les variables static.
void fermier(void)
{   
    sprintf((char *)&LCDText, (char*)" Module fermier ");
    LCD_Update();
    delay_ms(2000);
    LCD_Erase();
    
            
    while(1)
    {
        asking_data();
        
        if(MiApp_MessageAvailable())
        {  
            
            receiving_data();
        }
        MiApp_DiscardMessage();
        affichage ();
        
    }
}

// fonction asking_data, a chaque appel de la fonction, celle-ci envoit une trame 
// avec l'ID 0x01, permettant aux capteurs de savoir qu'ils doivent envoyer leurs datas
void asking_data(void)
{
   MiApp_FlushTx();
   MiApp_WriteData(0x01);
   MiApp_WriteData(0xDE);
   MiApp_WriteData(0xAD);
   MiApp_BroadcastPacket(true);
}


// fonction receiving_data, a chaque appel de la fonction celle-ci prend les trames 
// contenues dans le buffer de reception, et insere les données dans les variables 
// correspondantes dependamment de quel capteur l'a envoyé.
void receiving_data(void)
{
    if (rxMessage.Payload[0]== 0x02)
    {
        if (rxMessage.Payload[1]== 0x01)
        {
            temperature1 = rxMessage.Payload[2];
            temperature2 = rxMessage.Payload[3];
            humidite1 = rxMessage.Payload[4];        
        }
        else if (rxMessage.Payload[1]== 0x02)
        {
            temperature3 = rxMessage.Payload[2];
            temperature4 = rxMessage.Payload[3];
            humidite2 = rxMessage.Payload[4];
        }
        else if (rxMessage.Payload[1]== 0x03)
        {
            temperature5 = rxMessage.Payload[2];
            temperature6 = rxMessage.Payload[3];
            humidite3 = rxMessage.Payload[4];       
        }
        MiApp_DiscardMessage();
    }
}


// fonction d'affichage, a chaque appel de cette fonction celle-ci affiche a l'ecran
// le contenu des variables static (contenant les données des capteurs).
// Elle permet aussi de gérer la logique de switch (incrémentation de décrémentation
// des numéros de capteurs).
void affichage (void)
{
    int switch_val = BUTTON_Pressed();
    switch (etat)  
    {
        case 1 :    
        
            LCD_Erase();
            sprintf((char *)&LCDText, (char*)"Capteur 1");
            LCD_Update();
            flag = 0;

                if (flag == 0)
                {
                    sprintf((char *)&LCDText[16], (char*)"Temp air : %d", temperature1);
                    LCD_Update();
                    
                    for (int i=0 ; i<1500 ; i++ )
                    {
                        delay_ms(1);
                        switch_val = BUTTON_Pressed();
                        if (switch_val == switch2)
                        {
                            etat = 2;
                            flag = 1;
                            break;
                        }
                        if (switch_val == switch1)
                        {
                            etat = 3;
                            flag = 1;
                            break;                        
                        }
                        if(MiApp_MessageAvailable())
                        {  
                            
            
                            receiving_data();
                        }
                        MiApp_DiscardMessage();
                    }
                }
                if (flag == 0)
                {

                    sprintf((char *)&LCDText[16], (char*)"Temp sol : %d", temperature2); 
                    LCD_Update();

                    for (int i=0 ; i<1500 ; i++ )
                    {
                        delay_ms(1);
                        switch_val = BUTTON_Pressed();
                        if (switch_val == switch2)
                        {
                            etat = 2;
                            flag = 1;
                            break;
                        }
                        if (switch_val == switch1)
                        {
                            etat = 3;
                            flag = 1;
                            break;                        
                        }
                        if(MiApp_MessageAvailable())
                        {  
            
                            receiving_data();
                        }
                        MiApp_DiscardMessage();
                    }
                }
                if (flag == 0)
                {

                    sprintf((char *)&LCDText[16], (char*)"Humidity : %d", humidite1);
                    LCD_Update();
                
                    for (int i=0 ; i<1500 ; i++ )
                    {
                        delay_ms(1);
                        switch_val = BUTTON_Pressed();
                        if (switch_val == switch2)
                        {
                            etat = 2;
                            flag = 1;
                            break;
                        }
                        if (switch_val == switch1)
                        {
                            etat = 3;
                            flag = 1;
                            break;                        
                        }
                        if(MiApp_MessageAvailable())
                        {  
            
                            receiving_data();
                        }
                        MiApp_DiscardMessage();
                    } 
                }
            
            break;
            
        
        
        case 2 : 

            LCD_Erase(); 
            sprintf((char *)&LCDText, (char*)"Capteur 2");
            LCD_Update();
            flag = 0;
                    

                if (flag == 0)
                {

                    sprintf((char *)&LCDText[16], (char*)"Temp air : %d", temperature3);
                    LCD_Update();

                    for (int i=0 ; i<1500 ; i++ )
                    {
                        delay_ms(1);
                        switch_val = BUTTON_Pressed();
                        if (switch_val == switch2)
                        {
                            etat = 3;
                            flag = 1;
                            break;
                        }
                        if (switch_val == switch1)
                        {
                            etat = 1;
                            flag = 1;
                            break;                        
                        }
                        if(MiApp_MessageAvailable())
                        {  
            
                            receiving_data();
                        }
                        MiApp_DiscardMessage();
                    }
                }
                if (flag == 0)
                {
                    sprintf((char *)&LCDText[16], (char*)"Temp sol : %d", temperature4); 
                    LCD_Update();

                    for (int i=0 ; i<1500 ; i++ )
                    {
                        delay_ms(1);
                        switch_val = BUTTON_Pressed();
                        if (switch_val == switch2)
                        {
                            etat = 3;
                            flag = 1;
                            break;
                        }
                        if (switch_val == switch1)
                        {
                            etat = 1;
                            flag = 1;
                            break;                        
                        }
                        if(MiApp_MessageAvailable())
                        {  
                            receiving_data();
                        }
                        MiApp_DiscardMessage();
                    }
                }
                if (flag == 0)
                {
                    sprintf((char *)&LCDText[16], (char*)"Humidity : %d", humidite2);
                    LCD_Update();

                    for (int i=0 ; i<1500 ; i++ )
                    {
                        delay_ms(1);
                        switch_val = BUTTON_Pressed();
                        if (switch_val == switch2)
                        {
                            etat = 3;
                            flag = 1;
                            break;
                        }
                        if (switch_val == switch1)
                        {
                            etat = 1;
                            flag = 1;
                            break;                        
                        }
                        if(MiApp_MessageAvailable())
                        {  
                            receiving_data();
                        }
                        MiApp_DiscardMessage();
                    }
                }
            
            break;
        
        
        case 3 :    

            LCD_Erase();
            sprintf((char *)&LCDText, (char*)"Capteur 3");
            LCD_Update();
            flag = 0;
                    
                if (flag == 0)
                {
                    sprintf((char *)&LCDText[16], (char*)"Temp air : %d", temperature5);
                    LCD_Update();

                    for (int i=0 ; i<1500 ; i++ )
                    {
                        delay_ms(1);
                        switch_val = BUTTON_Pressed();
                        if (switch_val == switch2)
                        {
                            etat = 1;
                            flag = 1;
                            break;
                        }
                        if (switch_val == switch1)
                        {
                            etat = 2;
                            flag = 1;
                            break;                        
                        }
                        if(MiApp_MessageAvailable())
                        {  
            
                            receiving_data();
                        }
                        MiApp_DiscardMessage();
                    }
                }
                if (flag == 0)
                {
                    sprintf((char *)&LCDText[16], (char*)"Temp sol : %d", temperature6);   
                    LCD_Update();

                    for (int i=0 ; i<1500 ; i++ )
                    {
                        delay_ms(1);
                        switch_val = BUTTON_Pressed();
                        if (switch_val == switch2)
                        {
                            etat = 1;
                            flag = 1;
                            break;
                        }
                        if (switch_val == switch1)
                        {
                            etat = 2;
                            flag = 1;
                            break;                        
                        }
                        if(MiApp_MessageAvailable())
                        {  
                            receiving_data();
                        }
                        MiApp_DiscardMessage();
                    }
                }
                if (flag == 0)
                {
                    sprintf((char *)&LCDText[16], (char*)"Humidity : %d", humidite3);
                    LCD_Update();

                    for (int i=0 ; i<1500 ; i++ )
                    {
                        delay_ms(1);
                        switch_val = BUTTON_Pressed();
                        if (switch_val == switch2)
                        {
                            etat = 1;
                            flag = 1;
                            break;
                        }
                        if (switch_val == switch1)
                        {
                            etat = 2;
                            flag = 1;
                            break;                        
                        }
                        if(MiApp_MessageAvailable())
                        {  
                            
            
                            receiving_data();
                        }
                        MiApp_DiscardMessage();
                    }
                }
            
        break;
    }    
}   
           
      