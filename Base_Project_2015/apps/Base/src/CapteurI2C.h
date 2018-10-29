/* 
 * File:   CapteurI2C.h
 * Author: Benoit
 *
 * Created on October 28, 2018, 6:30 PM
 */

#ifndef CAPTEURI2C_H
#define	CAPTEURI2C_H

#define USECRC

float readHumidity();
float readTemperatureC();
float readTemperatureF();

float readTemperatureRaw();
int shiftIn(int _numBits);
int shiftInCRC();
void sendCommandSHT(int _command);
void waitForResultSHT();
int getData16SHT(int *val, char *CRC);
void skipCrcSHT();

#endif	/* CAPTEURI2C_H */

