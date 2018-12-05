#include "Board_Capteur.h"


#define DEVICE_ID 1

#define NUM_TEMP_SAMPLES 4

int Board_Capteur_Loop(char id_device){
    
    
    
    
    while(1)
    {
        
        if(MiApp_MessageAvailable())
        {
            
            //If ID == Pan, le message est le time triger, donc on transmet nos data
            if(rxMessage.Payload[0] == 0x01)
            {

                int8_t temperatureAir = 0;
                int8_t temperatureSol = 0;
                int8_t humiditeSol = 0;
                
                temperatureAir = ReadTemperatureSensor(Read_Voltage_Sensor());
                
                int err = 0;
                
                temperatureSol = readTemperatureC(&err);
                
                if(err != 0){
                    temperatureSol = -128;
                }
                
                err = 0;
                
                humiditeSol = readHumidity(&err);
                
                if(err != 0){
                    humiditeSol = -128;
                }
                
                MiApp_FlushTx();
                MiApp_WriteData(0x02);
                MiApp_WriteData(id_device);
                MiApp_WriteData(temperatureAir);
                MiApp_WriteData(temperatureSol);
                MiApp_WriteData(humiditeSol);
                MiApp_BroadcastPacket(false);
                
                
            }
            MiApp_DiscardMessage();
        }
        
    }
}

/*********************************************************************
* Function:         uint8_t ReadTempSensor(uint16_t VBGResult)
*
* PreCondition:     Proper reference voltage value has been determined.
*
* Input:            uint16_t VBGResult - Reference voltage for temp calculation.
*
* Output:           uint8_t temp
*
* Side Effects:	    none
*
* Overview:         Following routine reads the on board Tempature Sensor and
*                   calculates the temp value. 
*
* Note:			    
**********************************************************************/
int8_t ReadTemperatureSensor(uint16_t ADC_READING)
{
    int16_t tempValue;
    double temp;
    int8_t tempHere;
    uint8_t i = 0;
    float tempAverage = 0;
    int8_t tempArray[NUM_TEMP_SAMPLES];
    
     
    // Configure the ADC register settings
    ADCON0 = 0x04;
    ADCON1 = 0xBD;
    
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 0;
    
    do
    {
    	ADCON0bits.ADON = 1;
    	DELAY_10us(10);					// Wait Acquisition time
    	
    	ADCON0bits.GO = 1;	
    	while(ADCON0bits.DONE);
        
    	temp = (1200.0/ADC_READING);
    	tempValue = ADRES;
    	temp = (temp * tempValue);				
    	temp = (temp - 500.0)/10.0;
    	
    	tempArray[i] = (int8_t) temp;
    
        ADCON0bits.ADON = 0;
        DELAY_10us(1);
        i++;
	} while(i < NUM_TEMP_SAMPLES);
	

    for(i = 0; i<NUM_TEMP_SAMPLES; i++)
    {
        tempAverage = (tempAverage + tempArray[i]);
    }
    tempAverage = (tempAverage/NUM_TEMP_SAMPLES);
    tempHere = (int8_t) tempAverage;
    tempAverage = (tempAverage - tempHere) * 10;
    
    if(tempAverage >= 5)
        tempHere = tempHere + 1;
        
    return (int8_t)tempHere;


}

    				
/*********************************************************************
* Function:         uint16_t Read_VBGVoltage(void)
*
* PreCondition:     none
*
* Input:            none
*
* Output:           ADRES
*
* Side Effects:	    none
*
* Overview:         Reads the band gap voltage and compares with reference voltage
*                   to arrive at the current voltage level
*
* Note:			    
**********************************************************************/
uint16_t Read_Voltage_Sensor(void)
{
    ADCON0 = 0x3D;				// Configures the channel as VBG
    ADCON1 = 0xBD;				// Program the acquisition time
    ANCON1bits.VBGEN = 1;		// Enable Band gap reference voltage
    
    DELAY_10us(1000);			//Wait for the Band Gap Settling time
    
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 0;			//Disable ADC interrupts
    							//This routine uses the polling based mechanism
    ADCON0bits.GO = 1;		    //Start A/D conversion    
    while(ADCON0bits.DONE);
    
    ADCON0bits.ADON = 0;	    // Turn ADC OFF
    ANCON1bits.VBGEN = 0;	    // Disable Bandgap
    
    return ADRES;
}