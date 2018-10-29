/* 
 * File:   CapteurI2C.h
 * Author: Benoit
 *
 * Created on October 28, 2018, 6:30 PM
 */

#ifndef CAPTEURI2C_H
#define	CAPTEURI2C_H

float readHumidity();
float readTemperatureC();
float readTemperatureF();

float readTemperatureRaw();
int shiftIn(int _numBits);
void sendCommandSHT(int _command);
void waitForResultSHT();
int getData16SHT();
void skipCrcSHT();

#endif	/* CAPTEURI2C_H */

