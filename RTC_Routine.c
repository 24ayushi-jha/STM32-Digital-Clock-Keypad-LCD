/*
 * RTC_Routine.c
 *
 *  Created on: Nov 25, 2025
 *      Author: AYUSHIJHA
 */
#include"RTC_Routine.h"
#include "LCD_Routine.h"
#include "keypad_routine.h"
#include "bcd_routine.h"
#include"main.h"

extern I2C_HandleTypeDef hi2c1;

extern uint8_t sec,min,hour,wkday,date,month,year;
uint8_t led_on_h, led_on_m;
uint8_t led_off_h, led_off_m;

extern void Set_LED_ON_H(uint8_t on_h_dec);
extern void Set_LED_ON_M(uint8_t on_m_dec);
extern void Set_LED_OFF_H(uint8_t off_h_dec);
extern void Set_LED_OFF_M(uint8_t off_m_dec);
void Read_LED_Schedule(uint8_t on_h, uint8_t on_m, uint8_t off_h, uint8_t off_m);

extern void Read_LED_ON_H(uint8_t on_h);
extern void Read_LED_ON_M(uint8_t on_m);
extern void Read_LED_OFF_H(uint8_t off_h);
extern void Read_LED_OFF_M(uint8_t off_m);

extern void Set_LED_ON_H(uint8_t on_h_dec);
extern void Set_LED_ON_M(uint8_t on_m_dec);
extern void Set_LED_OFF_H(uint8_t off_h_dec);
extern void Set_LED_OFF_M(uint8_t off_m_dec);
extern void Write_LED_Schedule(uint8_t on_h_dec, uint8_t on_m_dec, uint8_t off_h_dec, uint8_t off_m_dec);

uint8_t RTC_Buffer[22];
uint8_t user_input[13];

uint8_t BCDToDec(uint8_t bcd);
void check_RTC(void);
void check_weekday_rtc(void);
void Read_Seconds(uint8_t sec);
void Read_minutes(uint8_t min);
void Read_hour(uint8_t hour );
void Read_weekday(uint8_t wkday);
void Read_date(uint8_t date);
void Read_month(uint8_t month);
void Read_year(uint8_t year);

void check_RTC(void)//rtc_enabled
{
	LCD_Send_Command(0x80);
	Print_LCD_String((uint8_t *)"Date:");

	uint8_t seconds;
	HAL_I2C_Mem_Read(&hi2c1,MCP7940M_ADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,&seconds,1,100);
	if((seconds & 0x80 )==0x80)
	{
		return ;  //clk is running ,exiting the function
	}
	else
	{
		seconds=(seconds|0x80);
		HAL_I2C_Mem_Write(&hi2c1,MCP7940M_ADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,&seconds,1,100);
		HAL_Delay(100);
		LCD_Send_Command(0x80);
		Print_LCD_String((uint8_t *)"clock started");
		HAL_Delay(1000);
	}
}

void check_weekday_rtc(void)
{
	uint8_t weekdays;
	HAL_I2C_Mem_Read(&hi2c1,MCP7940M_ADDRESS,0x03,I2C_MEMADD_SIZE_8BIT,&weekdays,1,100);
	if((weekdays & 0x20)==0x20)// check bit 5 (0x20): OSCRUN status bit (oscillator run status)
	{
		LCD_Send_Command(0x80);
	    Print_LCD_String((uint8_t *)"weekdays ok");
	}
}
void rtc_disabled(void)
{
	uint8_t seconds;
	HAL_I2C_Mem_Read(&hi2c1,MCP7940M_ADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,&seconds,1,100);

	if((seconds&0x80)==0x80)
	{
		seconds=seconds&0x7f;
		HAL_I2C_Mem_Write(&hi2c1,MCP7940M_ADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,&seconds,1,100);
	}
}
void Read_Seconds(uint8_t sec)
{
	uint8_t seconds_2;
	HAL_I2C_Mem_Read(&hi2c1,MCP7940M_ADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,&seconds_2,1,100);
	seconds_2=seconds_2 & 0x7f;// mask with 0x7F to clear the start oscillator (ST) bit (Bit 7).
	sec=BCDToDec(seconds_2);
	LCD_Send_Command(0x80|0x46);
	print_lcd_dec(sec);
}

void Read_minutes(uint8_t min)
{
	uint8_t min_seg=0;
	HAL_I2C_Mem_Read(&hi2c1,MCP7940M_ADDRESS,0x01,I2C_MEMADD_SIZE_8BIT,&min_seg,1,100);
	min_seg=min_seg & 0x7f;// mask with 0x7F to clear the reserved bit 7, ensuring only BCD data remains.
	min=BCDToDec(min_seg);
	LCD_Send_Command(0x80|0x43);
	print_lcd_dec(min);
	LCD_Print_Data(':');
}

void Read_hour(uint8_t hour)
{
	uint8_t hour_seg=0;
	HAL_I2C_Mem_Read(&hi2c1,MCP7940M_ADDRESS,0x02,I2C_MEMADD_SIZE_8BIT,&hour_seg,1,100);
	hour_seg=hour_seg & 0x3f;// mask with 0x3F to clear reserved bits (Bits 6 and 7) for 24-hour mode.
	hour=BCDToDec(hour_seg);
	LCD_Send_Command(0x80|0x40);
	print_lcd_dec(hour);
	LCD_Print_Data(':');
}

void Read_weekday(uint8_t wkday)
{
	uint8_t week_seg=0;
	HAL_I2C_Mem_Read(&hi2c1,MCP7940M_ADDRESS,0x03,I2C_MEMADD_SIZE_8BIT,&week_seg,1,100);
	week_seg=week_seg & 0x07;//isolates only weekdays bit 0-2
	if(week_seg==0x01)
	{
		LCD_Send_Command(0x8e);
		Print_LCD_String((uint8_t *)"MON");
	}
	else if(week_seg==0x02)
	{
		LCD_Send_Command(0x8e);
		Print_LCD_String((uint8_t *)"TUE");
	}
	else if(week_seg==0x03)
	{
		LCD_Send_Command(0x8e);
		Print_LCD_String((uint8_t *)"WED");
	}
	else if(week_seg==0x04)
	{
		LCD_Send_Command(0x8e);
		Print_LCD_String((uint8_t *)"THU");
	}
	else if(week_seg==0x05)
	{
		LCD_Send_Command(0x8e);
		Print_LCD_String((uint8_t *)"FRI");
	}
	else if(week_seg==0x06)
	{
		LCD_Send_Command(0x8e);
		Print_LCD_String((uint8_t *)"SAT");
	}
	else if(week_seg==0x07)
	{
		LCD_Send_Command(0x8e);
		Print_LCD_String((uint8_t *)"SUN");
	}
	wkday=BCDToDec(week_seg);
	print_lcd_dec(wkday);
}

void Read_date(uint8_t date)
{
	uint8_t date_seg=0;
	HAL_I2C_Mem_Read(&hi2c1,MCP7940M_ADDRESS,0x04,I2C_MEMADD_SIZE_8BIT,&date_seg,1,100);
	date_seg=date_seg & 0x3f;//mask with 0x3F to clear reserved bits (Bits 6 and 7)
	date=BCDToDec(date_seg);
	LCD_Send_Command(0x80|0x05);
	print_lcd_dec(date);
	LCD_Print_Data(':');
}

void Read_month(uint8_t month)
{
	uint8_t month_seg=0;

	HAL_I2C_Mem_Read(&hi2c1,MCP7940M_ADDRESS,0x05,I2C_MEMADD_SIZE_8BIT,&month_seg,1,100);
	if(month_seg & 0x20)//bit5<-leap year<-0             checking if ith bit set or not
	{
		LCD_Send_Command(0x80|0x49);
		Print_LCD_String((uint8_t *)"le");
	}
	else //bit5<-not a leap year<-1
	{
		return;
//		LCD_Send_Command(0x80|0x49);
//		Print_LCD_String((uint8_t *)"no");
	}
	month_seg= month_seg & 0x1f;//removing bit5=1<-leap year   mask with 0x1F to clear Bit 5 (Leap Year) and reserved bits (Bits 6 and 7)
	month=BCDToDec(month_seg);
	LCD_Send_Command(0x80|0x08);
	print_lcd_dec(month);
	LCD_Print_Data(':');
}

void Read_year(uint8_t year)
{
	uint8_t year_seg=0;
	HAL_I2C_Mem_Read(&hi2c1,MCP7940M_ADDRESS,0x06,I2C_MEMADD_SIZE_8BIT,&year_seg,1,100);
	year_seg=year_seg & 0xff;//mask with 0xff (no change) as the year register uses all 8 bits for BCD value.
	year=BCDToDec(year_seg);
	LCD_Send_Command(0x80|0x0b);
	print_lcd_dec(year);
}

void Set_seconds( uint8_t sec)
{
	uint8_t sec_seg=decToBCD(sec);
	sec_seg=sec_seg & 0x7f;//mask with 0x7F to clear the start oscillator (ST) bit (Bit 7)
	sec_seg=sec_seg|0x80;//ST (Start Oscillator), Bit 7 of the SEC register (0x00).
	HAL_I2C_Mem_Write(&hi2c1,MCP7940M_ADDRESS,0x00,I2C_MEMADD_SIZE_8BIT,&sec_seg,1,100);
}

void Set_min(uint8_t min)
{
	uint8_t min_seg=decToBCD(min);
	min_seg=min_seg & 0x7f;//mask with 0x7F to clear the reserved bit 7, ensuring only BCD data remains
	HAL_I2C_Mem_Write(&hi2c1,MCP7940M_ADDRESS,0x01,I2C_MEMADD_SIZE_8BIT,&min_seg,1,100);
}

void Set_hour( uint8_t hour)
{
	uint8_t hour_seg=decToBCD(hour);
	hour_seg=hour_seg & 0x3f;//mask with 0x3F to ensure 24hr mode (clears bits 6 and 7)
	HAL_I2C_Mem_Write(&hi2c1,MCP7940M_ADDRESS,0x02,I2C_MEMADD_SIZE_8BIT,&hour_seg,1,100);
}

void Set_weekday(uint8_t weekday)
{
	uint8_t week_seg=decToBCD(weekday);
	week_seg=week_seg & 0x07;//mask with 0x07 to keep only the Weekday value (Bits 0-2).
	week_seg=week_seg|0x20;//OSCRUN Status Bit, Bit 5 of the WKDY register (0x03).>-1
	HAL_I2C_Mem_Write(&hi2c1,MCP7940M_ADDRESS,0x03,I2C_MEMADD_SIZE_8BIT,&week_seg,1,100);
}

void Set_date(uint8_t date)
{
    uint8_t date_seg = decToBCD(date);
    date_seg=date_seg & 0x3f;//mask with 0x3F to clear reserved bits (Bits 6 and 7)
    HAL_I2C_Mem_Write(&hi2c1, MCP7940M_ADDRESS, 0x04,I2C_MEMADD_SIZE_8BIT, &date_seg, 1, 100);
}

void Set_month(uint8_t month)
{
    uint8_t month_seg = decToBCD(month);
    month_seg=month_seg&0x1f; //removing bit5=1<-leap year
    HAL_I2C_Mem_Write(&hi2c1, MCP7940M_ADDRESS, 0x05,I2C_MEMADD_SIZE_8BIT, &month_seg, 1, 100);
}

void Set_year( uint8_t year)
{
    uint8_t year_seg = decToBCD(year);
    HAL_I2C_Mem_Write(&hi2c1, MCP7940M_ADDRESS, 0x06,I2C_MEMADD_SIZE_8BIT, &year_seg, 1, 100);
}
void Read_Data(uint8_t sec, uint8_t min, uint8_t hour, uint8_t wkday, uint8_t date,uint8_t  month, uint8_t year)
{
	Read_Seconds(sec);
	Read_minutes(min);
	Read_hour(hour);
	Read_weekday(wkday);
	Read_date(date);
	Read_month(month);
	Read_year(year);
}
void Write_Data(uint8_t sec_ones,uint8_t sec_tens, uint8_t min_ones,uint8_t min_tens, uint8_t hour_ones,uint8_t hour_tens,
		 	 	uint8_t wkday, uint8_t date_ones,uint8_t date_tens,uint8_t  month_ones,uint8_t month_tens,uint8_t year_ones,
				uint8_t year_tens)
{
	uint8_t second = sec_tens*10 + sec_ones;
	uint8_t minute=min_tens*10+min_ones;
	uint8_t hour=hour_tens*10+hour_ones;
	uint8_t date=date_tens*10+date_ones;
	uint8_t month=month_tens*10+month_ones;
	uint8_t year=year_tens*10+year_ones;

	Set_seconds(second);
	Set_min(minute);
	Set_hour(hour);
	Set_weekday(wkday);
	Set_date(date);
	Set_month(month);
	Set_year(year);
}
