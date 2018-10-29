#include "CapteurI2C.h"
#include "soft_i2c.h"
#include "system.h"


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

  // Command to send to the SHT1x to request humidity
  int _gHumidCmd = 0b00000101;

  // Fetch the value from the sensor
  sendCommandSHT(_gHumidCmd);
  waitForResultSHT();
  _val = getData16SHT();
  skipCrcSHT();

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

  // Command to send to the SHT1x to request Temperature
  int _gTempCmd  = 0b00000011;

  sendCommandSHT(_gTempCmd);
  waitForResultSHT();
  _val = getData16SHT();
  skipCrcSHT();

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
     __delay_ms(5);  // I don't know why I need this, but without it I don't get my 8 lsb of temp
     ret = ret*2 + SDA_PIN;
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

  for(i= 0; i < 100; ++i)
  {
    __delay_ms(5);
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
int getData16SHT()
{
  int val;

  // Get the most significant bits
  i2cHighSda();
          
  val = shiftIn(8);
  val *= 256;

  // Send the required ack
  i2cHighSda();
  i2cLowSda();
  i2cHighScl();
  i2cLowScl();

  // Get the least significant bits
  i2cHighSda();
  val |= shiftIn(8);

  return val;
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