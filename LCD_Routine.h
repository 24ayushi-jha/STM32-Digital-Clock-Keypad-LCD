/*
 * LCD_Routine.h
 *
 *  Created on: Nov 24, 2025
 *      Author: AYUSHIJHA
 */

#ifndef INC_LCD_ROUTINE_H_
#define INC_LCD_ROUTINE_H_
//#include"stdint.h"
#include "main.h"
void LCD_Send_8bit(uint8_t data);
void LCD_Send_Command(uint8_t cmd);
void LCD_Init(void);
void LCD_Print_Data(uint8_t data);
void Print_LCD_String(uint8_t *ptr);
#endif /* INC_LCD_ROUTINE_H_ */
