//PAN

#include "pan.h"
#include "soft_uart.h"
#include "system.h"
#include "codes library.h"
#include "system_config.h"
#include "miwi/miwi_api.h"
#include "EEPROM.h"
#include "RTCSPI.h"
#include "math.h"

int saveCounter=0;
uint32_t startPoint=0;

void Pan(void){
    CS_DIR=0;
    
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 = 1;
	
	dataCapteur tempData;
	uint16_t compteurBoucle=0;
	uint16_t compteurSetTime=0;
	uint16_t compteurDumpData=0;
	
	int time[5];
	int lastTime[5];
	uint8_t flagResume=0;
       
    __delay_ms(1000);
    rtcStart();
    __delay_ms(1000);
   
    startPoint=EEPROMinit();
	
    while(true){
		//Reglage de la date et l'heure
		if(BoutonSetTime==0){
			compteurSetTime++;
			if(compteurSetTime==2000){
				setTimeMenu();
			}
		}
        
		else{
            compteurSetTime=0;
        }
		
		//EEPROM Dumping over UART
		if(BoutonDumpData_plusOk==0){
			compteurDumpData++;
			if(compteurDumpData==3000){
				LCD_Erase();
				sprintf((char *)&LCDText[0], (char*)"Dumping EEPROM");
				LCD_Update();
				sprintf((char *)&LCDText[16], (char*)"IN Progress");
				LCD_Update();
				dumpData();
				__delay_ms(1000);
				LCD_Erase();
				sprintf((char *)&LCDText[0], (char*)"Bt2 = mem clear");
				LCD_Update();
				sprintf((char *)&LCDText[16], (char*)"Bt3=no mem clear");
				LCD_Update();
				flagResume=0;
				while(flagResume==0){					
					if(BoutonDumpData_plusOk==0){
						__delay_ms(10);
						while(BoutonDumpData_plusOk==0);
						__delay_ms(10);
						flagResume=1;
						eraseData();
					}
					else if(BoutonMoinsNo==0){
						__delay_ms(10);
						while(BoutonMoinsNo==0);
						__delay_ms(10);
						flagResume=1;
						saveCounter=0;
						startPoint=EEPROMinit();
					}
				}
                LCD_Erase();
			}
		}
		else compteurDumpData=0;
		
		//Verification des messages entrants
		if(MiApp_MessageAvailable()){
			if(rxMessage.Payload[0]==0x02){//trame de reception
				switch(rxMessage.Payload[1]){//switch sur les ID de carte capteur
					case CARTE1_ADDRESS:
						tempData.frame.temperatureAir1=rxMessage.Payload[2];
						tempData.frame.temperatureSol1=rxMessage.Payload[3];
						tempData.frame.humidite1=rxMessage.Payload[4];
						break;
					case CARTE2_ADDRESS:
						tempData.frame.temperatureAir2=rxMessage.Payload[2];
						tempData.frame.temperatureSol2=rxMessage.Payload[3];
						tempData.frame.humidite2=rxMessage.Payload[4];					
						break;
					case CARTE3_ADDRESS:
						tempData.frame.temperatureAir3=rxMessage.Payload[2];
						tempData.frame.temperatureSol3=rxMessage.Payload[3];
						tempData.frame.humidite3=rxMessage.Payload[4];	
						break;
				}
			}
        }
        MiApp_DiscardMessage();
		__delay_ms(1);
		compteurBoucle++;
		
		//as toutes les 10 secondes demande les donnees au modules capteurs et verifie l'heure
		if (compteurBoucle==1000){
			compteurBoucle=0;
			//transmit prompt for sensor boards
			MiApp_FlushTx();
			MiApp_WriteData(0x01);
			MiApp_WriteData(0xDE);
			MiApp_WriteData(0xAD);
			MiApp_BroadcastPacket(false);
						
			lastTime[1]=time[1];//conserve la derniere valeur des Heures
			
			time[0]=rtcReadMinutes();
			time[1]=rtcReadHour();
			time[2]=rtcReadDay();
			time[3]=rtcReadMonth();
			time[4]=rtcReadYear();
			
			//print time on lcd
			sprintf((char *)&LCDText[0], (char*)"M%cJ %2d%c%2d %2d:%2d ",'/',time[3],'/',time[2],time[1],time[0]);
            
			LCD_Update();
            			
			if (time[1]!=lastTime[1]){//determine si l'heure a change
				for (int i=0; i<5; i++){
					tempData.array[i]=time[i];
				}
				eepromWriteDataToSave(startPoint+saveCounter*14,tempData);  
                saveCounter++;				
				eepromWriteLastAddress(startPoint+saveCounter*14);               
				//************************************************************//
                //METTRE UN PRINT "tempData" VERS LE UART OU LE SPI2          //
                //(protocole choisi) POUR L'ENVOIE DES DONNES AU MODULE       //
                //RESPONSABLE DE LA COMMUNICATION PAR WIFI                    //
                //************************************************************//
				if(saveCounter==17){
					startPoint=startPoint+saveCounter*14+18;
					saveCounter=0;
				}
			}		
		}
    }
}

//Lecture de la position ou recommencer l'ecritre dans la eeprom
uint32_t EEPROMinit (void){
	uint32_t lastAddress=0;
	float tempAddress=0;
	uint32_t pageFirstAddress=0;
	uint32_t pageNumber=0;
	
	if(eepromRead(0x08)!=0x42){
        //__delay_ms(10);
        eepromWriteLastAddress(256);
        
        eepromWrite(0x08,0x42);        
    }
	lastAddress=eepromReadLastAddress();
	
	tempAddress=lastAddress/256.0;
	pageNumber=(int)floor(tempAddress);
    
	pageFirstAddress=pageNumber*256;
    saveCounter=(lastAddress-pageFirstAddress)/14;
    return pageFirstAddress;
}

void dumpData (void){
	uint32_t lastAddress=0;
	uint32_t start=256;
	uint32_t readCounter=0;
	dataCapteur readData;
	lastAddress=eepromReadLastAddress();
	
	printf("Annee; Mois; Jour; Heure; Minute; humiditeCarte1; temperatureAirCarte1; temperatureSolCarte1; humiditeCarte2; temperatureAirCarte2; temperatureSolCarte2; humiditeCarte3; temperatureAirCarte3; temperatureSolCarte3\n\r");
	
	while((start+readCounter*14)<lastAddress){
		
		readData=eepromReadDataCapteur((start+readCounter*14));  
        __delay_ms(1);
        printf("%d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d; %d\n\r",readData.frame.annee,readData.frame.mois, readData.frame.jour,readData.frame.heure,readData.frame.minute,
                                                                            readData.frame.humidite1,readData.frame.temperatureAir1,readData.frame.temperatureSol1,readData.frame.humidite2,
                                                                            readData.frame.temperatureAir2,readData.frame.temperatureSol2,readData.frame.humidite3,readData.frame.temperatureAir3,
                                                                            readData.frame.temperatureSol3);
        readCounter++;
        if(readCounter==17){
            start=start+readCounter*14+18;
            readCounter=0;
        }
	}
}
void eraseData (void){
	saveCounter=0;
	eepromWriteLastAddress(256);
    startPoint=256;
}

setTimeMenu(void){	
    uint8_t date[5]={0,1,1,0,0};
	LCD_Erase();
    //reglage annee
	sprintf((char *)&LCDText[0], (char*)"Reglage Annee:");
	LCD_Update();
	while(BoutonSetTime==0);
	__delay_ms(10);
	while(BoutonSetTime==1){
		if(BoutonDumpData_plusOk==0){
			__delay_ms(10);
			while(BoutonDumpData_plusOk==0);
			__delay_ms(10);
			date[4]++;
			if(date[4]>99) date[4]=0;
			sprintf((char *)&LCDText[16], (char*)"           ");//vide la ligne 2
			LCD_Update();
            if (date[4]<10){
                sprintf((char *)&LCDText[16], (char*)"Annee: 200%d",date[4]);
            }
            else sprintf((char *)&LCDText[16], (char*)"Annee: 20%d",date[4]);			
			LCD_Update();
		}
		if(BoutonMoinsNo==0){
			__delay_ms(10);
			while(BoutonMoinsNo==0);
			__delay_ms(10);
			date[4]--;
			if(date[4]>200) date[4]=99;
			sprintf((char *)&LCDText[16], (char*)"               ");//vide la ligne 2
			LCD_Update();
			if (date[4]<10){
                sprintf((char *)&LCDText[16], (char*)"Annee: 200%d",date[4]);
            }
            else sprintf((char *)&LCDText[16], (char*)"Annee: 20%d",date[4]);
            //sprintf((char *)&LCDText[16], (char*)"Annee: 20%d",date[4]);
			LCD_Update();
		}		
	}
	__delay_ms(10);
	while(BoutonSetTime==0);
	__delay_ms(10);
	//reglage mois
	LCD_Erase();
	sprintf((char *)&LCDText[0], (char*)"Reglage Mois:");
	LCD_Update();
	while(BoutonSetTime==1){
		if(BoutonDumpData_plusOk==0){
			__delay_ms(10);
			while(BoutonDumpData_plusOk==0);
			__delay_ms(10);
			date[3]++;
			if(date[3]>12) date[3]=1;
			sprintf((char *)&LCDText[16], (char*)"           ");//vide la ligne 2
			LCD_Update();
			sprintf((char *)&LCDText[16], (char*)"Mois: %d",date[3]);
			LCD_Update();
		}
		if(BoutonMoinsNo==0){
			__delay_ms(10);
			while(BoutonMoinsNo==0);
			__delay_ms(10);
			date[3]--;
			if(date[3]==0) date[3]=12;
			sprintf((char *)&LCDText[16], (char*)"               ");//vide la ligne 2
			LCD_Update();
			sprintf((char *)&LCDText[16], (char*)"Mois: %d",date[3]);
			LCD_Update();
		}		
	}	
	__delay_ms(10);
	while(BoutonSetTime==0);
	__delay_ms(10);
	//reglage jour
	LCD_Erase();
	sprintf((char *)&LCDText[0], (char*)"Reglage Jour");
	LCD_Update();
	while(BoutonSetTime==1){
		if(BoutonDumpData_plusOk==0){
			__delay_ms(10);
			while(BoutonDumpData_plusOk==0);
			__delay_ms(10);
			date[2]++;
			if(date[2]>31) date[2]=1;
			sprintf((char *)&LCDText[16], (char*)"           ");//vide la ligne 2
			LCD_Update();
			sprintf((char *)&LCDText[16], (char*)"Jour: %d",date[2]);
			LCD_Update();
		}
		if(BoutonMoinsNo==0){
			__delay_ms(10);
			while(BoutonMoinsNo==0);
			__delay_ms(10);
			date[2]--;
			if(date[2]==0) date[2]=31;
			sprintf((char *)&LCDText[16], (char*)"               ");//vide la ligne 2
			LCD_Update();
			sprintf((char *)&LCDText[16], (char*)"Jour: %d",date[2]);
			LCD_Update();
		}		
	}
	__delay_ms(10);
	while(BoutonSetTime==0);
	__delay_ms(10);
	//reglage heure
	LCD_Erase();
	sprintf((char *)&LCDText[0], (char*)"Reglage Heure");
	LCD_Update();
	while(BoutonSetTime==1){
		if(BoutonDumpData_plusOk==0){
			__delay_ms(10);
			while(BoutonDumpData_plusOk==0);
			__delay_ms(10);
			date[1]++;
			if(date[1]>24) date[1]=0;
			sprintf((char *)&LCDText[16], (char*)"           ");//vide la ligne 2
			LCD_Update();
			sprintf((char *)&LCDText[16], (char*)"Heure: %d",date[1]);
			LCD_Update();
		}
		if(BoutonMoinsNo==0){
			__delay_ms(10);
			while(BoutonMoinsNo==0);
			__delay_ms(10);
			date[1]--;
			if(date[1]>200) date[1]=24;
			sprintf((char *)&LCDText[16], (char*)"               ");//vide la ligne 2
			LCD_Update();
			sprintf((char *)&LCDText[16], (char*)"Heure: %d",date[1]);
			LCD_Update();
		}		
	}
	__delay_ms(10);
	while(BoutonSetTime==0);
	__delay_ms(10);
	//reglage minute
	LCD_Erase();
	sprintf((char *)&LCDText[0], (char*)"Reglage Minute");
	LCD_Update();
	while(BoutonSetTime==1){
		if(BoutonDumpData_plusOk==0){
			__delay_ms(10);
			while(BoutonDumpData_plusOk==0);
			__delay_ms(10);
			date[0]++;
			if(date[0]>59) date[0]=0;
			sprintf((char *)&LCDText[16], (char*)"           ");//vide la ligne 2
			LCD_Update();
			sprintf((char *)&LCDText[16], (char*)"Minute: %d",date[0]);
			LCD_Update();
		}
		if(BoutonMoinsNo==0){
			__delay_ms(10);
			while(BoutonMoinsNo==0);
			__delay_ms(10);
			date[0]--;
			if(date[0]>200) date[0]=59;
			sprintf((char *)&LCDText[16], (char*)"               ");//vide la ligne 2
			LCD_Update();
			sprintf((char *)&LCDText[16], (char*)"Minute: %d",date[0]);
			LCD_Update();
		}		
	}
	__delay_ms(10);
	while(BoutonSetTime==0);
	__delay_ms(10);
	rtcSetTime(0x00, date[0], date[1], date[2], date[3], date[4]);//enregistre l'heure
    LCD_Erase();
}









