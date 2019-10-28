//*********************************************************************************************//
//	Nicolas et Robin																		   //	
//																							   //
//	Fonction d'interfacage avvec la EEPROM ainsi que des fonctions pour enregistrer et lire	   //
//	les donnee des capteur selon un format min/heure/jour/mois/annee/hum1..3/temp1...6)		   //	
//  	
//  Notes: La ligne CS_DIR=0; doit etre ajoute avant 
//

//*********************************************************************************************//

#include "spi.h"
#include "system.h"
#include "system_config.h"
//define du chip select
#define cSEeprom PORTEbits.RE2	
#define CS_DIR TRISEbits.TRISE2

//structure de formatage des donnees des capteurs
typedef union{	
	struct{
		int8_t minute;
		int8_t heure;
		int8_t jour;
		int8_t mois;
		int8_t annee;
		int8_t humidite1;
		int8_t humidite2;
		int8_t humidite3;
		int8_t temperatureAir1;
		int8_t temperatureAir2;
		int8_t temperatureAir3;
		int8_t temperatureSol1;
		int8_t temperatureSol2;
		int8_t temperatureSol3;
	}frame;
	int8_t array[14];
}dataCapteur;


void eepromWrite(uint32_t writeAddress,int8_t data);
/*{
	@Ecrit un byte a l'adresse specifie
	
	Input:	writeAddress (adresse ou ecrire)
			data (le byte a enregistrer)
	Output: none			
}*/	


void eepromMultiWrite(uint32_t writeStartAddress,uint8_t length,int8_t *data);
/*{
	@Ecrit une serie de bytes a partir de l'adresse specifie
	
	Input:	writeStartAddress (adresse ou commencer l'ecriture)
			data (tableau de uint8_t a enregistrer)
	Output: none	
}*/

int8_t eepromRead(uint32_t readAddress);
/*{
	@Lit un byte a l'adresse specifie
	
	Input:	readAddress (adresse ou lire)
	
	Output: data (le byte lue)	
}*/

uint8_t eepromReadUnsigned(uint32_t readAddress);


void eepromWriteLastAddress(uint32_t startAddressValue);
/*{
	@Enregistre la valeur de l'adresse ou l'enregistrement des donnees
	 de capteur est rendu dans la eeprom.
	
	Input:	startAddressValue (adresse ou ecrire)
	
	Output: none
}*/

uint32_t eepromReadLastAddress(void);
/*{
	@Lit la valeur de l'adresse ou l'enregistrement des donnees
	 de capteur est rendu dans la eeprom.
	
	Input:	none
	
	Output: address ou ou l'enregistrement des donnees
			de capteur est rendu dans la eeprom. 
}*/	
	


dataCapteur eepromReadDataCapteur(uint32_t startAddress);
/*{
	@Lit un timeStamp de donnee enregistres a l'adresse specifier
	
	Input:	startAddress (adresse de depart pour la lecture)
	
	Output: data (de type dataCapteur, formater min/heure/jour/mois/annee/hum1..3/temp1...6) 	
	
}*/	

void eepromWriteDataToSave(uint32_t writeStartAddress, dataCapteur data);
/*{
	@Enregistre un timeStamp de donnee enregistres a l'adresse specifier
	
	Input:	writeStartAddress (adresse ou commencer l'ecriture)
			data (de type dataCapteur, formater min/heure/jour/mois/annee/hum1..3/temp1...6)
	
	Output: none 	
			
}*/	