#ifndef BOARD_CAPTEUR_H
#define	BOARD_CAPTEUR_H

#include <stdint.h>
#include "system.h"
#include "system_config.h"
#include "miwi/miwi_api.h"
#include "CapteurI2C.h"

uint16_t Read_Voltage_Sensor(void);
int8_t ReadTemperatureSensor(uint16_t Result);

#endif	/* BOARD_CAPTEUR_H */

