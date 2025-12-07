/*
 * bcd_routine.c
 *
 *  Created on: Nov 25, 2025
 *      Author: AYUSHIJHA
 */
#include"bcd_routine.h"
#include"main.h"
#include"LCD_Routine.h"

uint8_t decToBCD(uint8_t dec);
uint8_t BCDToDec(uint8_t bcd);
void print_lcd_dec(uint8_t dec);
void print_lcd_dec(uint8_t dec);

uint8_t decToBCD(uint8_t dec)
{
	uint8_t high,low,bcd;
	low=dec%10;
	dec=dec/10;
	high=dec%10;
	bcd=high;
	bcd=bcd<<4;
	bcd=bcd|low;
	return bcd;
//	return ((val/10)<<4) + (val%10);
}

uint8_t BCDToDec(uint8_t bcd)
{
	uint8_t high,low,dec;
	low=bcd & 0x0f;
	bcd=bcd>>4;
	high=bcd;
	dec=(high*10)+low;
	return dec;
//	return ((bcd>>4)*10)+(bcd & 0x0f);
}

void print_lcd_bcd(uint8_t bcd)
{
	LCD_Print_Data((bcd>>4)+0x30);
	LCD_Print_Data((bcd & 0x0f)+0x30);
}

void print_lcd_dec(uint8_t dec)
{
	uint8_t ones,tens;

	ones=dec%10;//a=123,ones=3
	dec=dec/10;//a=12
	tens=dec%10;//a=12,tens=2
	dec=dec/10;//a=1;

	LCD_Print_Data(tens+0x30);
	LCD_Print_Data(ones+0x30);
}

