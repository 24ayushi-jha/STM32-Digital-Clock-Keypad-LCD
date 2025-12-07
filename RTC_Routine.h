/*
 * RTC_Routine.h
 *
 *  Created on: Nov 25, 2025
 *      Author: AYUSHIJHA
 */

#ifndef INC_RTC_ROUTINE_H_
#define INC_RTC_ROUTINE_H_
#include"stdint.h"
//#include "main.h"
#define MCP7940M_ADDRESS		(0x6f<<1)
extern uint8_t led_on_h, led_on_m;
extern uint8_t led_off_h, led_off_m;
extern uint8_t LED_Status; // 0 = OFF, 1 = ON

void check_RTC(void);
void check_weekday_rtc(void);
void rtc_disabled(void);

void Read_Seconds(uint8_t sec);
void Read_minutes(uint8_t min);
void Read_hour( uint8_t hour );
void Read_weekday(uint8_t wkday);
void Read_date(uint8_t date);
void Read_month(uint8_t month);
void Read_year(uint8_t year);

void Read_LED_ON_H(uint8_t on_h);
void Read_LED_ON_M(uint8_t on_m);
void Read_LED_OFF_H(uint8_t off_h);
void Read_LED_OFF_M(uint8_t off_m);

void Set_seconds(uint8_t sec);
void Set_min(uint8_t min);
void Set_hour(uint8_t hour);
void Set_weekday(uint8_t weekday);
void Set_date(uint8_t date);
void Set_month(uint8_t month);
void Set_year(uint8_t year);

void Set_LED_ON_H(uint8_t on_h_dec);
void Set_LED_ON_M(uint8_t on_m_dec);
void Write_Data( uint8_t sec_ones,uint8_t sec_tens, uint8_t min_ones,uint8_t min_tens, uint8_t hour_ones,uint8_t hour_tens,
		 	 	 uint8_t wkday, uint8_t date_ones,uint8_t date_tens,uint8_t  month_ones,uint8_t month_tens,
				 uint8_t year_ones,uint8_t year_tens);
void Read_Data(uint8_t sec, uint8_t min,uint8_t  hour,uint8_t  wkday,uint8_t  date, uint8_t month, uint8_t year);
#endif /* INC_RTC_ROUTINE_H_ */
