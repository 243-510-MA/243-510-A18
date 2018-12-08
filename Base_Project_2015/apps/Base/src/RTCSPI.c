/*
 * Project name: Serre 2018
 * Controle de la real time clock
 * Chip : MCP79510
 * Labrosse / Daunais / Mailloux-Bourassa / Caron
 */
#include "system_config/miwikit_pic18f46j50_24j40/spi.h"
#include "system.h"
#include "system_config.h"
#include "miwi/miwi_api.h"
#include "RTCSPI.h"

// RTC Commands
#define EEREAD  0x03
#define EEWRITE 0x02
#define EEWRDI  0x04
#define EEWREN  0x06
#define SSREAD  0x05
#define SRWRITE 0x01
#define READ    0x13
#define WRITE   0x12
#define UNLOCK  0x14
#define IDWRITE 0x32
#define IDREAD  0x33
#define CLRRAM  0x54
 
//time and configuration registers
#define MILISECONDS_REGISTERS      0x00
#define SECONDS_REGISTER           0x01
#define MINUTES_REGISTER           0x02
#define HOURS_REGISTER             0x03
#define DAY_REGISTER               0x04
#define DATE_REGISTER              0x05
#define MONTH_REGISTER             0x06
#define YEAR_REGISTER              0x07
#define STATUS_REGISTER            0x08
#define CALIBRATION                0x09


//// RTC click module connections
#define Chip_Select LATAbits.LATA2
#define Chip_Select_Direction TRISAbits.TRISA2
//// RTC click module connections

//clear RTCC and SRAM memory
void rtcClear(void)
{
    TRISAbits.TRISA2 = 0;
    for(int i=0;i<0x20;i++)
    {
        Chip_Select = 0;
        SPIPut2(WRITE);//Clear RTCC memory
        SPIPut2(i);
        SPIPut2(0x00);
        Chip_Select = 1; 
        delay_us(1);
    }
    Chip_Select = 0;
    SPIPut2(CLRRAM); //Clear SRAM memory
    Chip_Select = 1;
    delay_us(1);
}
// Set RTC time
void rtcSetTime(uint8_t seconds, uint8_t minutes, uint8_t hours,uint8_t days, uint8_t months, uint8_t years) 
{
    TRISAbits.TRISA2 = 0;    
    // Write seconds into RTC
    Chip_Select = 0;
    SPIPut2(WRITE);
    SPIPut2(SECONDS_REGISTER);
    SPIPut2(uint2bcd(seconds));
    Chip_Select = 1;
    delay_us(1);

    // Write minutes into RTC
    Chip_Select = 0;
    SPIPut2(WRITE);
    SPIPut2(MINUTES_REGISTER);
    SPIPut2(uint2bcd(minutes));
    Chip_Select = 1;
    delay_us(1);

    // Write hours into RTC
    Chip_Select = 0;
    SPIPut2(WRITE);
    SPIPut2(HOURS_REGISTER);
    SPIPut2(uint2bcd(hours));
    Chip_Select = 1;
    delay_us(1);

    // Write days into RTC
    Chip_Select = 0;
    SPIPut2(WRITE);
    SPIPut2(DATE_REGISTER);
    SPIPut2(uint2bcd(days));
    Chip_Select = 1;
    delay_us(1);

    // Write months into RTC
    Chip_Select = 0;
    SPIPut2(WRITE);
    SPIPut2(MONTH_REGISTER);
    SPIPut2(uint2bcd(months));
    Chip_Select = 1;
    delay_us(1);

    // Write years into RTC
    Chip_Select = 0;
    SPIPut2(WRITE);
    SPIPut2(YEAR_REGISTER);
    SPIPut2(uint2bcd(years));
    Chip_Select = 1;
    delay_us(1);
    
    Chip_Select = 0;
	SPIPut2(WRITE);
	SPIPut2(0x01);
	SPIPut2(0x80);
	Chip_Select = 1;
    delay_us(1);
    
    char cnt = 0;
	while(cnt < 100)
	{
		Chip_Select = 0;
		SPIPut2(READ);
		SPIPut2(0x04);
		if((SPIGet2() & 0b00100000) != 0)
		{
			break;		
		}
		cnt++;		
	}		
	Chip_Select = 1;
    delay_us(1);
    
    Chip_Select = 0;
	SPIPut2(WRITE);
	SPIPut2(DAY_REGISTER);
	SPIPut2(0x08);
	Chip_Select = 1;

}

void rtcStart(void)
{
    TRISAbits.TRISA2 = 0;
    
    uint8_t sec = rtcReadSecond();
    uint8_t min = rtcReadMinutes();
    uint8_t hour = rtcReadHour();
    uint8_t date = rtcReadDay();
    uint8_t month = rtcReadMonth();
    uint8_t year = rtcReadYear();
     
    Chip_Select = 0;
	SPIPut2(WRITE);
	SPIPut2(SECONDS_REGISTER);
	SPIPut2(0x00);
	Chip_Select = 1;
           
    // Write minutes into RTC    
    Chip_Select = 0;
    SPIPut2(WRITE);
    SPIPut2(MINUTES_REGISTER);
    SPIPut2(uint2bcd(min));
    Chip_Select = 1;
    delay_us(1);

    // Write hours into RTC
    Chip_Select = 0;
    SPIPut2(WRITE);
    SPIPut2(HOURS_REGISTER);
    SPIPut2(uint2bcd(hour));
    Chip_Select = 1;
    delay_us(1);

    // Write days into RTC
    Chip_Select = 0;
    SPIPut2(WRITE);
    SPIPut2(DATE_REGISTER);
    SPIPut2(uint2bcd(date));
    Chip_Select = 1;
    delay_us(1);

    // Write months into RTC
    Chip_Select = 0;
    SPIPut2(WRITE);
    SPIPut2(MONTH_REGISTER);
    SPIPut2(uint2bcd(month));
    Chip_Select = 1;
    delay_us(1);

    // Write years into RTC
    Chip_Select = 0;
    SPIPut2(WRITE);
    SPIPut2(YEAR_REGISTER);
    SPIPut2(uint2bcd(year));
    Chip_Select = 1;
    delay_us(1);
    
    Chip_Select = 0;
	SPIPut2(WRITE);
	SPIPut2(SECONDS_REGISTER);
	SPIPut2(0x80|sec); // Write seconds into RTC and activate the ST bit
	Chip_Select = 1;

    char cnt = 0;
	while(cnt < 100)
	{
		Chip_Select = 0;
		SPIPut2(READ);
		SPIPut2(0x04);
		if((SPIGet2() & 0b00100000) != 0)
		{
			break;		
		}
		cnt++;		
	}		
	Chip_Select = 1;
    delay_us(1);
    
    Chip_Select = 0;
	SPIPut2(WRITE);
	SPIPut2(DAY_REGISTER);
	SPIPut2(0x08);
	Chip_Select = 1;
}



uint8_t rtcReadMinutes(void) 
{ 
    uint8_t min10,min1,minutes;
    
    Chip_Select = 0;
    
    SPIPut2(READ);
    SPIPut2(MINUTES_REGISTER);
    
    min1   = SPIGet2() & 0x7F;
    
    min10 = (min1 & 0x70) >> 4;
	min1 = min1 & 0x0F;
	minutes = min1 + min10 * 10;
    
    Chip_Select = 1;  
    return minutes;
}
uint8_t rtcReadSecond(void){  
    uint8_t sec10,sec1,seconds;
    
    Chip_Select = 0;
    
    SPIPut2(READ);
    SPIPut2(SECONDS_REGISTER);
    
    sec1   = SPIGet2() & 0x7F;
    
    sec10 = (sec1 & 0x70) >> 4;
	sec1 = sec1 & 0x0F;
	seconds = sec1 + sec10 * 10;
    
    Chip_Select = 1;
    return seconds;
}

uint8_t rtcReadHour(void) 
{   
    uint8_t hour10,hour1,hour;
    
    Chip_Select = 0;
    
    SPIPut2(READ);
    SPIPut2(HOURS_REGISTER);
    
    hour1  = SPIGet2() & 0x3F;
    
    hour10 = (hour1 & 0x30) >> 4;
	hour1 = hour1 & 0x0F;
	hour = hour1 + hour10 * 10;
    
    Chip_Select = 1;  
    return hour;
}

uint8_t rtcReadDay(void) 
{  
    uint8_t day10,day1,day;
    
    Chip_Select = 0;
    
    SPIPut2(READ);
    SPIPut2(DATE_REGISTER);
    
    day1  = SPIGet2() & 0x3F;
    
    day10 = (day1 & 0x30) >> 4;
	day1 = day1 & 0x0F;
	day = day1 + day10 * 10;
    
    Chip_Select = 1;  
    return day;
}

uint8_t rtcReadMonth(void) 
{   
    uint8_t month10,month1,month;
    
    Chip_Select = 0;
    
    SPIPut2(READ);
    SPIPut2(MONTH_REGISTER);
    
    month1 = SPIGet2() & 0x1F;
    
    month10 = (month1 & 0x10) >> 4;
	month1 = month1 & 0x0F;
	month = month1 + month10 * 10;
    
    Chip_Select = 1; 
    return month;
}

uint8_t rtcReadYear(void) 
{   
    uint8_t year10,year1,year;
    
    Chip_Select = 0;
    
    SPIPut2(READ);
    SPIPut2(YEAR_REGISTER);
    
    year1  = SPIGet2();
    
    year10 = (year1 & 0xF0) >> 4;
	year1 = year1 & 0x0F;
	year = year1 + year10 * 10;
    
    Chip_Select = 1; 
    return year;
}

void rtcPrintTime(void)
{   
    LCD_Erase();
    sprintf((char *)&LCDText, (char*)"%u/%u/%u        %u:%u:%u",rtcReadDay(),rtcReadMonth(),rtcReadYear(),rtcReadHour(),rtcReadMinutes(),rtcReadSecond());
    LCD_Update();
}

void rtcReadTime(unsigned char* seconds, unsigned char* minutes, unsigned char* hour, unsigned char* date, unsigned char* month, unsigned char *year) 
{    
    unsigned char sec10, min10, hour10, date10, month10, year10, sec1, min1, hour1, date1, month1, year1;
    
    Chip_Select = 0;
    
    SPIPut2(READ);
    SPIPut2(SECONDS_REGISTER);
    
    sec1   = SPIGet2() & 0x7F;
    min1   = SPIGet2() & 0x7F;
    hour1  = SPIGet2() & 0x3F;
    date1  = SPIGet2(); //Day, read and then overwrite with the date. Day not used
    date1  = SPIGet2() & 0x3F; 
    month1 = SPIGet2() & 0x1F;
    year1  = SPIGet2();
    
    Chip_Select = 1;

    sec10 = (sec1 & 0x70) >> 4;
	sec1 = sec1 & 0x0F;
	*seconds = sec1 + sec10 * 10;	

	min10 = (min1 & 0x70) >> 4;
	min1 = min1 & 0x0F;
	*minutes = min1 + min10 * 10;
	
	hour10 = (hour1 & 0x30) >> 4;
	hour1 = hour1 & 0x0F;
	*hour = hour1 + hour10 * 10;

	date10 = (date1 & 0x30) >> 4;
	date1 = date1 & 0x0F;
	*date = date1 + date10 * 10;

	month10 = (month1 & 0x10) >> 4;
	month1 = month1 & 0x0F;
	*month = month1 + month10 * 10;

	year10 = (year1 & 0xF0) >> 4;
	year1 = year1 & 0x0F;
	*year = year1 + year10 * 10;
}

 static unsigned int uint2bcd(unsigned int nombre)
 {
 	return ((nombre / 10) << 4) | (nombre % 10);
 }