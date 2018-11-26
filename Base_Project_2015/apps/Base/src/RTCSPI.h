/*
 * Project name: Serre 2018
 * Controle de la real time clock
 * Chip : MCP79510
 * Labrosse / Daunais / Mailloux-Bourassa / Caron
 */  
#include "system_config/miwikit_pic18f46j50_24j40/spi.h"

#ifndef RTC_H
#define	RTC_H

    //clear RTCC and SRAM memory
    void rtcClear(void);

    // Set RTC time 
    void rtcSetTime(uint8_t seconds, uint8_t minutes, uint8_t hours,uint8_t days, uint8_t months, uint8_t years); 

    //Initialize the RTC 
    void rtcStart(void);
    
    // Read time from the RTC
    void rtcReadTime(unsigned char* seconds, unsigned char* minutes, unsigned char* hour, unsigned char* date, unsigned char* month, unsigned char *year);
    
    //Read the seconds register from the RTC and return a uint8_t byte
    uint8_t rtcReadSeconds(void);
    
    //Read the minutes register from the RTC and return a uint8_t byte
    uint8_t rtcReadMinutes(void);
    
    //Read the hour register from the RTC and return a uint8_t byte
    uint8_t rtcReadHour(void);
    
    //Read the date register from the RTC and return a uint8_t byte
    uint8_t rtcReadDay(void);
    
    //Read the month register from the RTC and return a uint8_t byte
    uint8_t rtcReadMonth(void);
    
    //Read the year register from the RTC and return a uint8_t byte
    uint8_t rtcReadYear(void);
    
    //Print the current date/time on the LCD
    void rtcPrintTime(void);
    
    //Convert an int variable to a BCD int
    static unsigned int uint2bcd(unsigned int nombre);


#endif