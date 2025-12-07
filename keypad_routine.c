/*
 * keypad.c
 *
 *  Created on: Nov 24, 2025
 *      Author: AYUSHIJHA
 */
#include "keypad_routine.h"
#include"LCD_Routine.h"
#include"RTC_Routine.h"
#include"main.h"

extern I2C_HandleTypeDef hi2c1;

uint8_t keypressed=0xff;
uint8_t last_keypressed = 0xFF;

void keycheck(void);
void key_action(void);
void check_RTC(void);
void rtc_disabled(void);
void LCD_Print_Data(uint8_t data);
void Print_LCD_String(uint8_t *ptr);

void keycheck(void)   //event generation or received
{
    keypressed = 0xFF;//when key is pressed its high
    // --- Row 1 ---
    HAL_GPIO_WritePin(KEY1_R1_GPIO_Port, KEY1_R1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(KEY2_R2_GPIO_Port, KEY2_R2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(KEY3_R3_GPIO_Port, KEY3_R3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(KEY4_R4_GPIO_Port, KEY4_R4_Pin, GPIO_PIN_SET);

    if (HAL_GPIO_ReadPin(KEY5_C1_GPIO_Port, KEY5_C1_Pin) == GPIO_PIN_RESET){keypressed = '1';}//event posting or dispatched
    else if (HAL_GPIO_ReadPin(KEY6_C2_GPIO_Port, KEY6_C2_Pin) == GPIO_PIN_RESET){ keypressed = '2';}////event posting or dispatched
    else if (HAL_GPIO_ReadPin(KEY7_C3_GPIO_Port, KEY7_C3_Pin) == GPIO_PIN_RESET){keypressed = '3';}
    else if (HAL_GPIO_ReadPin(KEY8_C4_GPIO_Port, KEY8_C4_Pin) == GPIO_PIN_RESET){keypressed = 'A';}
    // --- Row 2 ---
    HAL_GPIO_WritePin(KEY1_R1_GPIO_Port, KEY1_R1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(KEY2_R2_GPIO_Port, KEY2_R2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(KEY3_R3_GPIO_Port, KEY3_R3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(KEY4_R4_GPIO_Port, KEY4_R4_Pin, GPIO_PIN_SET);

    if (HAL_GPIO_ReadPin(KEY5_C1_GPIO_Port, KEY5_C1_Pin) == GPIO_PIN_RESET){keypressed = '4';}
    else if (HAL_GPIO_ReadPin(KEY6_C2_GPIO_Port, KEY6_C2_Pin) == GPIO_PIN_RESET){keypressed = '5';}
    else if (HAL_GPIO_ReadPin(KEY7_C3_GPIO_Port, KEY7_C3_Pin) == GPIO_PIN_RESET){keypressed = '6';}
    else if (HAL_GPIO_ReadPin(KEY8_C4_GPIO_Port, KEY8_C4_Pin) == GPIO_PIN_RESET){keypressed = 'B';}
    // --- Row 3 ---
    HAL_GPIO_WritePin(KEY1_R1_GPIO_Port, KEY1_R1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(KEY2_R2_GPIO_Port, KEY2_R2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(KEY3_R3_GPIO_Port, KEY3_R3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(KEY4_R4_GPIO_Port, KEY4_R4_Pin, GPIO_PIN_SET);

    if (HAL_GPIO_ReadPin(KEY5_C1_GPIO_Port, KEY5_C1_Pin) == GPIO_PIN_RESET){keypressed = '7';}
    else if (HAL_GPIO_ReadPin(KEY6_C2_GPIO_Port, KEY6_C2_Pin) == GPIO_PIN_RESET){keypressed = '8';}
    else if (HAL_GPIO_ReadPin(KEY7_C3_GPIO_Port, KEY7_C3_Pin) == GPIO_PIN_RESET){keypressed = '9';}
    else if (HAL_GPIO_ReadPin(KEY8_C4_GPIO_Port, KEY8_C4_Pin) == GPIO_PIN_RESET){keypressed = 'C';}
    // --- Row 4 ---
    HAL_GPIO_WritePin(KEY1_R1_GPIO_Port, KEY1_R1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(KEY2_R2_GPIO_Port, KEY2_R2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(KEY3_R3_GPIO_Port, KEY3_R3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(KEY4_R4_GPIO_Port, KEY4_R4_Pin, GPIO_PIN_RESET);

    if (HAL_GPIO_ReadPin(KEY5_C1_GPIO_Port, KEY5_C1_Pin) == GPIO_PIN_RESET){keypressed = '*';}
    else if (HAL_GPIO_ReadPin(KEY6_C2_GPIO_Port, KEY6_C2_Pin) == GPIO_PIN_RESET){keypressed = '0';}
    else if (HAL_GPIO_ReadPin(KEY7_C3_GPIO_Port, KEY7_C3_Pin) == GPIO_PIN_RESET){keypressed = '#';}
    else if (HAL_GPIO_ReadPin(KEY8_C4_GPIO_Port, KEY8_C4_Pin) == GPIO_PIN_RESET){keypressed = 'D';}
}

//fixed lookup tables
extern uint8_t user_input[13];
uint8_t Index=0;
uint8_t one_time_enter=1;

extern uint8_t hour;
extern uint8_t min;
extern uint8_t sec;
extern uint8_t wkday;
extern uint8_t date;
extern uint8_t month;
extern uint8_t year;

uint8_t LCD_ADDRESS_MAP[13] =
{
		0xC7,  // Index 0: sec ones 0xcc
		0xC6,  // Index 1: sec tens 0xcb
		0xC4,  // Index 2: min ones 0xc9
		0xC3,  // Index 3: min tens 0xc8
		0xC1,  // Index 4: hr ones 0xc6
		0xC0,  // Index 5: hr tens 0xc5
		0x8e,  // Index 6: wkday 0xcf
		0x8B,  // Index 7: date ones
		0x8C,  // Index 8: date tens
		0x88,  // Index 9: month ones
		0x89,  // Index 10: month tens
		0x85,  // Index 11: year ones
		0x86   // Index 12: year tens
};

//moment during key released
void key_action(void)
{
	if(keypressed != 0xff)
	{
		if(one_time_enter == 1)
		{
			one_time_enter = 0;
			if(keypressed >= '0' && keypressed <= '9')
			{
				LCD_Send_Command(LCD_ADDRESS_MAP[Index]);
				HAL_Delay(10);

				user_input[Index] = keypressed - '0';//eg '5'-'0'=5 ascii to int conversion
				Index++;  //tracks position of user_input
				if(Index>=13){	Index = 0;	}
				LCD_Print_Data(keypressed);
			}
		}
	}
	else
	{
	one_time_enter = 1;
	}
}
