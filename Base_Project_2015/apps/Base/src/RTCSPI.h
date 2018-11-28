/*
 * Project name: Serre 2018
 * Controle de la real time clock
 * Chip : MCP79510
 * Labrosse / Daunais / Mailloux-Bourassa / David Caron
 */  
#include "system_config/miwikit_pic18f46j50_24j40/spi.h"

#ifndef RTC_H
#define	RTC_H

    //clear RTCC and SRAM memory
    void rtcClear(void);

    // Set RTC time and start counting
    void rtcSetTime(uint8_t seconds, uint8_t minutes, uint8_t hours,uint8_t days, uint8_t months, uint8_t years); 

    //Starts the RTC (without initializing it)
    void rtcStart(void);
    
    // Read time from the RTC
    void rtcReadTime(unsigned char* seconds, unsigned char* minutes, unsigned char* hour, unsigned char* date, unsigned char* month, unsigned char *year);
    
    //Reads the register referring to the seconds and returns it in uint8_t format
    uint8_t rtcReadSeconds(void);
    
    //Reads the register referring to the minutes and returns it in uint8_t format
    uint8_t rtcReadMinutes(void);
    
    //Reads the register referring to the hours and returns it in uint8_t format
    uint8_t rtcReadHour(void);
    
    //Reads the register referring to the date and returns it in uint8_t format
    uint8_t rtcReadDay(void);
    
    //Reads the register referring to the months and returns it in uint8_t format
    uint8_t rtcReadMonth(void);
    
    //Reads the register referring to the years and returns it in uint8_t format
    uint8_t rtcReadYear(void);
    
    //Print the current date/time on the LCD
    void rtcPrintTime(void);
    
    //Convert an int variable to a BCD int
    static unsigned int uint2bcd(unsigned int nombre);


#endif