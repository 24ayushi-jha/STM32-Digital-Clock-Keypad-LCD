/* USER CODE BEGIN Header */
/**
  **************************
  * @file           : main.c
  * @brief          : Main program body
  **************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  **************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include"LCD_Routine.h"
#include"keypad_routine.h"
#include"RTC_Routine.h"
#include"bcd_routine.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
uint8_t sec, min, hour;
uint8_t wkday, date, month, year,wkday, date_ones, date_tens, month_ones, month_tens, year_ones, year_tens;
uint8_t key_state=0,LED_Status=0;

void LCD_Send_Command(uint8_t cmd);
void Print_LCD_String(uint8_t *ptr);
void keycheck(void);
void key_action(void);
void rtc_disabled(void);
void check_RTC(void);
void Read_Data(uint8_t sec, uint8_t min, uint8_t hour, uint8_t wkday, uint8_t date,uint8_t  month, uint8_t year);
void Write_Data(uint8_t sec_ones,uint8_t sec_tens, uint8_t min_ones,uint8_t min_tens, uint8_t hour_ones,uint8_t hour_tens,
		 	 	uint8_t wkday, uint8_t date_ones,uint8_t date_tens,uint8_t  month_ones,uint8_t month_tens,uint8_t year_ones,
				uint8_t year_tens);
void LCD_Init(void);

extern uint8_t keypressed,Index; //extern defined in another source file but accessed in current file
extern uint8_t user_input[13];
extern uint8_t led_on_h, led_on_m;
extern uint8_t led_off_h, led_off_m;

void LED_Schedule(void)
{
	if(hour==led_on_h && min==led_on_m)
	{
		LED_Status=1;
	}
	if(hour==led_off_h && min==led_off_m)
	{
		LED_Status=0;
	}
	if(LED_Status==1)
	{
		HAL_GPIO_WritePin(LED_PIN_GPIO_Port,LED_PIN_Pin,GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(LED_PIN_GPIO_Port,LED_PIN_Pin,GPIO_PIN_RESET);
	}
}


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  LCD_Init();
  /* USER CODE BEGIN 2 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  check_RTC();

  while (1)
  {
	  switch(key_state)
	  	  {
	  	  	  case 0:
	  			  HAL_Delay(900);
	  			  LED_Schedule();
	  			  Read_Data(sec, min, hour,wkday, date, month, year);

	  			  LCD_Send_Command(0x80|0x4c);
	  			  Print_LCD_String((uint8_t *)"nor");

	  			  keycheck();
	  			  key_action();
	  			  if(keypressed=='#'){	key_state=1;	keypressed = 0xFF;	} break;
	  			  if(keypressed=='A'){	key_state=3;	}	break;
	  		  case 1:
	  			  rtc_disabled();  //clock stop
	  			  keycheck();      //keypad data
	  			  key_action();

	  			  LCD_Send_Command(0x80|0x4c);
	  			  Print_LCD_String((uint8_t *)"edit");

	  			  if(keypressed=='D')
	  			  {
	  				  key_state=2;
	  			  	  Index=0;
	  			  	  keypressed=0xff;

	  			  	  LCD_Send_Command(0x80|0x4c);
					  Print_LCD_String((uint8_t *)"");
	  			  	  break;
	  			  }
	  			  break;
	  		  case 2:
	  			  Write_Data(user_input[0], user_input[1], user_input[2], user_input[3], user_input[4], user_input[5], user_input[6],
	  					  	 user_input[7], user_input[8], user_input[9], user_input[10], user_input[11], user_input[12] );
	  			  check_RTC();     //start  //rtc_enabled
	  			  key_state = 0;
	  			  break;
	  	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00201D2B;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(KEY4_R4_GPIO_Port, KEY4_R4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_D7_14_Pin|LCD_D6_13_Pin|LCD_D5_12_Pin|LCD_D4_11_Pin
                          |LCD_D3_10_Pin|LCD_D2_9_Pin|LCD_D1_8_Pin|KEY1_R1_Pin
                          |KEY2_R2_Pin|KEY3_R3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_D0_7_Pin|LCD_EN_6_Pin|LCD_RW_5_Pin|LCD_RS_4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_PIN_GPIO_Port, LED_PIN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : KEY4_R4_Pin */
  GPIO_InitStruct.Pin = KEY4_R4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(KEY4_R4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY5_C1_Pin KEY6_C2_Pin */
  GPIO_InitStruct.Pin = KEY5_C1_Pin|KEY6_C2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY7_C3_Pin KEY8_C4_Pin */
  GPIO_InitStruct.Pin = KEY7_C3_Pin|KEY8_C4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_D7_14_Pin LCD_D6_13_Pin LCD_D5_12_Pin LCD_D4_11_Pin
                           LCD_D3_10_Pin LCD_D2_9_Pin LCD_D1_8_Pin KEY1_R1_Pin
                           KEY2_R2_Pin KEY3_R3_Pin */
  GPIO_InitStruct.Pin = LCD_D7_14_Pin|LCD_D6_13_Pin|LCD_D5_12_Pin|LCD_D4_11_Pin
                          |LCD_D3_10_Pin|LCD_D2_9_Pin|LCD_D1_8_Pin|KEY1_R1_Pin
                          |KEY2_R2_Pin|KEY3_R3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_D0_7_Pin LCD_EN_6_Pin */
  GPIO_InitStruct.Pin = LCD_D0_7_Pin|LCD_EN_6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RW_5_Pin LCD_RS_4_Pin */
  GPIO_InitStruct.Pin = LCD_RW_5_Pin|LCD_RS_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_PIN_Pin */
  GPIO_InitStruct.Pin = LED_PIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_PIN_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
