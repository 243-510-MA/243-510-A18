//*********************************************************************************************//
//	Nicolas et Robin																		   //	
//																							   //
//	Fonction d'interfacage avvec la EEPROM ainsi que des fonctions pour enregistrer et lire	   //
//	les donnee des capteur selon un format min/heure/jour/mois/annee/hum1..3/temp1...6)		   //	
//																							   //
//*********************************************************************************************//

#include "EEPROM.c"
#include "spi.h"
#include "system.h"
#include "system_config.h"

//define du chip select
#define cSEeprom 

//structure de formatage des donnees des capteurs
typedef union{	
	struct{
		uint8_t minute;
		uint8_t heure;
		uint8_t	jour;
		uint8_t	mois;
		uint8_t annee;
		uint8_t humidite1;
		uint8_t humidite2;
		uint8_t humidite3;
		uint8_t temperature1;
		uint8_t temperature2;
		uint8_t temperature3;
		uint8_t temperature4;
		uint8_t temperature5;
		uint8_t temperature6;
	}frame;
	uint8_t array[14];
} ;


void eepromWrite(uint32_t writeAddress,uint8_t data);
/*{
	@Ecrit un byte a l'adresse specifie
	
	Input:	writeAddress (adresse ou ecrire)
			data (le byte a enregistrer)
	Output: none			
}*/	


void eepromMultiWrite(uint32_t writeStartAddress,uint8_t length,uint8_t *data);
/*{
	@Ecrit une serie de bytes a partir de l'adresse specifie
	
	Input:	writeStartAddress (adresse ou commencer l'ecriture)
			data (tableau de uint8_t a enregistrer)
	Output: none	
}*/

uint8_t eepromRead(uint32_t readAddress);
/*{
	@Lit un byte a l'adresse specifie
	
	Input:	readAddress (adresse ou lire)
	
	Output: data (le byte lue)	
}*/


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
	


savedData eepromReadSavedData(uint32_t startAddress);
/*{
	@Lit un timeStamp de donnee enregistres a l'adresse specifier
	
	Input:	startAddress (adresse de depart pour la lecture)
	
	Output: data (de type savedData, formater min/heure/jour/mois/annee/hum1..3/temp1...6) 	
	
}*/	

void eepromWriteDataToSave(uint32_t writeStartAddress, savedData data);
/*{
	@Enregistre un timeStamp de donnee enregistres a l'adresse specifier
	
	Input:	writeStartAddress (adresse ou commencer l'ecriture)
			data (de type savedData, formater min/heure/jour/mois/annee/hum1..3/temp1...6)
	
	Output: none 	
			
}*/	