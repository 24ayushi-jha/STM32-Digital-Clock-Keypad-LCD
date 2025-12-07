/*
 * bcd_routine.h
 *
 *  Created on: Nov 25, 2025
 *      Author: AYUSHIJHA
 */

#ifndef INC_BCD_ROUTINE_H_
#define INC_BCD_ROUTINE_H_
#include "main.h"
//#include"stdint.h"
uint8_t decToBCD(uint8_t dec);
uint8_t BCDToDec(uint8_t bcd);
void print_lcd_bcd(uint8_t bcd);
void print_lcd_dec(uint8_t dec);


#endif /* INC_BCD_ROUTINE_H_ */
