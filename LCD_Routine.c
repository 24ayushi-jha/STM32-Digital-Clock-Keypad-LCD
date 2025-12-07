/*
 * Lcd_Routine.c
 *
 *  Created on: Nov 24, 2025
 *      Author: AYUSHIJHA
 */
#include "LCD_Routine.h"
#include"main.h"
void LCD_Send_8bit(uint8_t data);
void LCD_Send_Command(uint8_t cmd);
void LCD_Init(void);
void LCD_Print_Data(uint8_t data);
void Print_LCD_String(uint8_t *ptr);

void LCD_Send_8bit(uint8_t data)
{
    // D0
    if ((data & 0x01) == 0x01){HAL_GPIO_WritePin(LCD_D0_7_GPIO_Port, LCD_D0_7_Pin, GPIO_PIN_SET);}
    else {HAL_GPIO_WritePin(LCD_D0_7_GPIO_Port, LCD_D0_7_Pin, GPIO_PIN_RESET);}
    // D1
    if ((data & 0x02) == 0x02){HAL_GPIO_WritePin(LCD_D1_8_GPIO_Port, LCD_D1_8_Pin, GPIO_PIN_SET);}
    else{HAL_GPIO_WritePin(LCD_D1_8_GPIO_Port, LCD_D1_8_Pin, GPIO_PIN_RESET);}
    // D2
    if ((data & 0x04) == 0x04){HAL_GPIO_WritePin(LCD_D2_9_GPIO_Port, LCD_D2_9_Pin, GPIO_PIN_SET);}
    else{HAL_GPIO_WritePin(LCD_D2_9_GPIO_Port, LCD_D2_9_Pin, GPIO_PIN_RESET);}
    // D3
    if ((data & 0x08) == 0x08){HAL_GPIO_WritePin(LCD_D3_10_GPIO_Port, LCD_D3_10_Pin, GPIO_PIN_SET);}
    else{HAL_GPIO_WritePin(LCD_D3_10_GPIO_Port, LCD_D3_10_Pin, GPIO_PIN_RESET);}
    // D4
    if ((data & 0x10) == 0x10){HAL_GPIO_WritePin(LCD_D4_11_GPIO_Port, LCD_D4_11_Pin, GPIO_PIN_SET);}
    else{HAL_GPIO_WritePin(LCD_D4_11_GPIO_Port, LCD_D4_11_Pin, GPIO_PIN_RESET);}
    // D5
    if ((data & 0x20) == 0x20){HAL_GPIO_WritePin(LCD_D5_12_GPIO_Port, LCD_D5_12_Pin, GPIO_PIN_SET);}
    else{HAL_GPIO_WritePin(LCD_D5_12_GPIO_Port, LCD_D5_12_Pin, GPIO_PIN_RESET);}
    // D6
    if ((data & 0x40) == 0x40){HAL_GPIO_WritePin(LCD_D6_13_GPIO_Port, LCD_D6_13_Pin, GPIO_PIN_SET);}
    else{HAL_GPIO_WritePin(LCD_D6_13_GPIO_Port, LCD_D6_13_Pin, GPIO_PIN_RESET);}
    // D7
    if ((data & 0x80) == 0x80){HAL_GPIO_WritePin(LCD_D7_14_GPIO_Port, LCD_D7_14_Pin, GPIO_PIN_SET);}
    else{HAL_GPIO_WritePin(LCD_D7_14_GPIO_Port, LCD_D7_14_Pin, GPIO_PIN_RESET);}
}

void LCD_Send_Command(uint8_t cmd)
{
	 HAL_GPIO_WritePin(LCD_RS_4_GPIO_Port,LCD_RS_4_Pin,GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(LCD_RW_5_GPIO_Port,LCD_RW_5_Pin,GPIO_PIN_RESET);
	 LCD_Send_8bit(cmd);
	 HAL_GPIO_WritePin(LCD_EN_6_GPIO_Port,LCD_EN_6_Pin,GPIO_PIN_SET);
     HAL_Delay(2);
     HAL_GPIO_WritePin(LCD_EN_6_GPIO_Port,LCD_EN_6_Pin,GPIO_PIN_RESET);
}

void LCD_Init(void)
{
	 HAL_Delay(50);
	 LCD_Send_Command(0x38);
	 HAL_Delay(2);
	 LCD_Send_Command(0x38);
	 HAL_Delay(2);
	 LCD_Send_Command(0x0C);
	 HAL_Delay(2);
	 LCD_Send_Command(0x01);
	 HAL_Delay(2);
	 LCD_Send_Command(0x06);
	 HAL_Delay(2);
}

//function for printing any digit
void LCD_Print_Data(uint8_t data)
{
	 HAL_GPIO_WritePin(LCD_RS_4_GPIO_Port,LCD_RS_4_Pin,GPIO_PIN_SET);
	 HAL_GPIO_WritePin(LCD_RW_5_GPIO_Port,LCD_RW_5_Pin,GPIO_PIN_RESET);
	 LCD_Send_8bit(data);
	 HAL_GPIO_WritePin(LCD_EN_6_GPIO_Port,LCD_EN_6_Pin,GPIO_PIN_SET);
	 HAL_Delay(2);
	 HAL_GPIO_WritePin(LCD_EN_6_GPIO_Port,LCD_EN_6_Pin,GPIO_PIN_RESET);
}

//function for printing string
void Print_LCD_String(uint8_t *ptr)
{
	while(*ptr)
	{
		LCD_Print_Data(*ptr);
		ptr++;
    }
}



