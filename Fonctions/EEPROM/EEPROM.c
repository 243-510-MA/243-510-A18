//*********************************************************************************************//
//	Nicolas et Robin																		   //	
//																							   //
//	Fonction d'interfacage avvec la EEPROM ainsi que des fonctions pour enregistrer et lire	   //
//	les donnee des capteur selon un format min/heure/jour/mois/annee/hum1..3/temp1...6)		   //	
//																							   //
//*********************************************************************************************//

#include "EEPROM.h"

//Ecriture simple
void eepromWrite(uint32_t writeAddress,uint8_t data){
	
	cSEeprom=0;
	SPIPut2(0x06);//write enable 
	cSEeprom=1;
	//Maybe a delay
	cSEeprom=0;
	SPIPut2(0x02);//write instruction 
	//24 bit Address
	SPIPut2(writeAddress>>16);
	SPIPut2(writeAddress>>8);
	SPIPut2(writeAddress);
	SPIPut2(data);
	cSEeprom=1;
	//Maybe a delay
	cSEeprom=0;
	SPIPut2(0x04);//write disable 
	cSEeprom=1;
	//delay
}	

//Ecriture multiple
void eepromMultiWrite(uint32_t writeStartAddress,uint8_t length,uint8_t *data){
	
	cSEeprom=0;
	SPIPut2(0x06);//write enable 
	cSEeprom=1;
	//Maybe a delay
	cSEeprom=0;
	SPIPut2(0x02);//write instruction 
	//24 bit Address
	SPIPut2(writeStartAddress>>16);
	SPIPut2(writeStartAddress>>8);
	SPIPut2(writeStartAddress);
	for(uint8_t i=0; i<length; i++){
		SPIPut2(data[i]);
	}
	cSEeprom=1;
	//Maybe a delay
	cSEeprom=0;
	SPIPut2(0x04);//write disable 
	cSEeprom=1;
	//delay
}

//Lecture simple
uint8_t eepromRead(uint32_t readAddress){
	uint8_t data=0;
	cSEeprom=0;
	SPIPut2(0x03);//read instruction 
	//24 bit Address
	SPIPut2(readAddress>>16);
	SPIPut2(readAddress>>8);
	SPIPut2(readAddress);
	data=SPIGet2();
	cSEeprom=1;
	return data;	
	//delay
}

//Enregistre la valeur de l'adresse ou l'enregistrement des donnees
//de capteur est rendu dans la eeprom.
void eepromWriteLastAddress(uint32_t startAddressValue){
	uint8_t address[3]={0,0,0};
	address[0]=startAddressValue;
	address[1]=startAddressValue>>8;
	address[2]=startAddressValue>>16;
	eepromMultiWrite(0x00,0x03,address)
}

//Lit la valeur de l'adresse ou l'enregistrement des donnees
//de capteur est rendu dans la eeprom.
uint32_t eepromReadLastAddress(void){
	uint32_t address=0;	
	for(uint8_t i=3; i>0; i--){
		address=address<<8;	
		address=address+eepromRead(0x02-i);
	}		
	return address;
}

//Lit un timeStamp de donnee enregistres a l'adresse specifier
savedData eepromReadSavedData(uint32_t startAddress){
	savedData data;
	cSEeprom=0;
	SPIPut2(0x03);//read instruction 
	//24 bit Address
	SPIPut2(startAddress>>16);
	SPIPut2(startAddress>>8);
	SPIPut2(startAddress);
	
	for(uint8_t i=0; i<14; i++){
		data.array[i]=SPIGet2();
	}
	
	cSEeprom=1;
	return data;	
}	

//Enregistre un timeStamp de donnee enregistres a l'adresse specifier
void eepromWriteDataToSave(uint32_t writeStartAddress, savedData data){
	savedData data;
	cSEeprom=0;
	SPIPut2(0x06);//write enable 
	cSEeprom=1;
	//Maybe a delay
	cSEeprom=0;
	SPIPut2(0x02);//write instruction 
	//24 bit Address
	SPIPut2(writeStartAddress>>16);
	SPIPut2(writeStartAddress>>8);
	SPIPut2(writeStartAddress);
	
	for(uint8_t i=0; i<14; i++){
		data.array[i]=SPIPut2();
	}
	
	cSEeprom=1;
	return data;	
}	