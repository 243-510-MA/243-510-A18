#include "CapteurI2C.h"
#include "soft_i2c.h"
#include "system.h"

char crcTable[] = {0,49,98,83,196,245,166,151,185,136,219,234,125,76,31,46,
67,114,33,16,135,182,229,212,250,203,152,169,62,15,92,109,
134,183,228,213,66,115,32,17,63,14,93,108,251,202,153,168,
197,244,167,150,1,48,99,82,124,77,30,47,184,137,218,235,
61,12,95,110,249,200,155,170,132,181,230,215,64,113,34,19,
126,79,28,45,186,139,216,233,199,246,165,148,3,50,97,80,
187,138,217,232,127,78,29,44,2,51,96,81,198,247,164,149,
248,201,154,171,60,13,94,111,65,112,35,18,133,180,231,214,
122,75,24,41,190,143,220,237,195,242,161,144,7,54,101,84,
57,8,91,106,253,204,159,174,128,177,226,211,68,117,38,23,
252,205,158,175,56,9,90,107,69,116,39,22,129,176,227,210,
191,142,221,236,123,74,25,40,6,55,100,85,194,243,160,145,
71,118,37,20,131,178,225,208,254,207,156,173,58,11,88,105,
4,53,102,87,192,241,162,147,189,140,223,238,121,72,27,42,
193,240,163,146,5,52,103,86,120,73,26,43,188,141,222,239,
130,179,224,209,70,119,36,21,59,10,89,104,255,206,157,172};

/**
 * Reads the current temperature in degrees Celsius
 */
float readTemperatureC()
{
  int _val;                // Raw value returned from sensor
  float _temperature;      // Temperature derived from raw value

  // Conversion coefficients from SHT15 datasheet
  const float D1 = -40.0;  // for 14 Bit @ 5V
  const float D2 =   0.01; // for 14 Bit DEGC

  // Fetch raw value
  _val = readTemperatureRaw();

  // Convert raw value to degrees Celsius
  _temperature = (_val * D2) + D1;

  return (_temperature);
}

/**
 * Reads the current temperature in degrees Fahrenheit
 */
float readTemperatureF()
{
  int _val;                 // Raw value returned from sensor
  float _temperature;       // Temperature derived from raw value

  // Conversion coefficients from SHT15 datasheet
  const float D1 = -40.0;   // for 14 Bit @ 5V
  const float D2 =   0.018; // for 14 Bit DEGF

  // Fetch raw value
  _val = readTemperatureRaw();

  // Convert raw value to degrees Fahrenheit
  _temperature = (_val * D2) + D1;

  return (_temperature);
}

/**
 * Reads current temperature-corrected relative humidity
 */
float readHumidity()
{
  int _val;                    // Raw humidity value returned from sensor
  float _linearHumidity;       // Humidity with linear correction applied
  float _correctedHumidity;    // Temperature-corrected humidity
  float _temperature;          // Raw temperature value

  // Conversion coefficients from SHT15 datasheet
  const float C1 = -4.0;       // for 12 Bit
  const float C2 =  0.0405;    // for 12 Bit
  const float C3 = -0.0000028; // for 12 Bit
  const float T1 =  0.01;      // for 14 Bit @ 5V
  const float T2 =  0.00008;   // for 14 Bit @ 5V
  
 /* const float C1 = -2.0468;     // for 12 Bit
  const float C2 =  0.0367;    // for 12 Bit
  const float C3 = -0.0000015955; // for 12 Bit
  const float T1 =  0.01;      // for 14 Bit @ 5V
  const float T2 =  0.00008;   // for 14 Bit @ 5V*/

  // Command to send to the SHT1x to request humidity
  int _gHumidCmd = 0b00000101;

  char crc=0;
  // Fetch the value from the sensor
  sendCommandSHT(_gHumidCmd);
  waitForResultSHT();
  getData16SHT(&_val, &crc);
  //skipCrcSHT();

  // Apply linear conversion to raw value
  _linearHumidity = C1 + C2 * _val + C3 * _val * _val;

  // Get current temperature for humidity correction
  _temperature = readTemperatureC();

  // Correct humidity value for current temperature
  _correctedHumidity = (_temperature - 25.0 ) * (T1 + T2 * _val) + _linearHumidity;

  return (_correctedHumidity);
}

/**
 * Reads the current raw temperature value
 */
float readTemperatureRaw()
{
  int _val;
  char crc;
  char calcCRC = 0;

  // Command to send to the SHT1x to request Temperature
  int _gTempCmd  = 0b00000011;

  sendCommandSHT(_gTempCmd);
  waitForResultSHT();
  getData16SHT(&_val, &crc);
  
  calcCRC = _gTempCmd;
  calcCRC = crcTable[calcCRC];
  
  calcCRC = calcCRC ^ (_val/256);
  calcCRC = crcTable[calcCRC];
  
  calcCRC = calcCRC ^ (_val&255);
  calcCRC = crcTable[calcCRC];

  //skipCrcSHT();

  return (_val);
}

/**
 */
int shiftIn(int _numBits)
{
  int ret = 0;
  int i;

  for (i=0; i<_numBits; ++i)
  {
     i2cHighScl();
     //__delay_us(5);  // I don't know why I need this, but without it I don't get my 8 lsb of temp
     ret = ret*2 + SDA_PIN; //Le *2 shift le registre vers la gauche et insere l'état de sda
     i2cLowScl();
  }

  return(ret);
}

int shiftInCRC()
{
  int ret = 0;
  int i;

  for (i=7; i>=0; i--)
  {
     i2cHighScl();
     //__delay_us(5);  // I don't know why I need this, but without it I don't get my 8 lsb of temp
     ret = ret/2 + (SDA_PIN*128); //Le *2 shift le registre vers la gauche et insere l'état de sda
     i2cLowScl();
  }

  return(ret);
}

/**
 */
void sendCommandSHT(int _command)
{
  int ack;

  // Transmission Start
  i2cHighSda();
  i2cHighScl();
  i2cLowSda();
  i2cLowScl();
  i2cHighScl();
  i2cHighSda();
  i2cLowScl();

  // The command (3 msb are address and must be 000, and last 5 bits are command)
  i2cWriteByte(_command);

  // Verify we get the correct ack
  i2cHighScl();
  i2cHighSda();
  ack = SDA_PIN;
  if (ack != 0) {
      //Erreur ACK
  }
  i2cLowScl();
  ack = SDA_PIN;
  if (ack != 1) {
      //Erreur ACK
  }
}

/**
 */
void waitForResultSHT(){
  int i;
  int ack;

  i2cHighSda();

  for(i= 0; i < 3200; ++i)
  {
    __delay_us(100);
    ack = SDA_PIN;

    if (ack == 0) {
      break;
    }
  }

  if (ack == 1) {
    //Ack error
  }
}


/**
 */
int getData16SHT(int *val, char *CRC)
{

  // Get the most significant bits
  i2cHighSda();
          
  *val = shiftIn(8);
  *val *= 256;

  // Send the required ack
  i2cAck();

  // Get the least significant bits
  i2cHighSda();
  *val |= shiftIn(8);
  
#ifdef USECRC
  
  i2cAck();
  
  i2cHighSda();
          
  *CRC = shiftInCRC();
  
  i2cAck();
  
#else
  
  i2cNack();
  
#endif
  
  
  i2cStop();

  return 0;
}

/**
 */
void skipCrcSHT()
{
  // Skip acknowledge to end trans (no CRC)

  i2cHighSda();
  i2cHighScl();
  i2cLowScl();
}