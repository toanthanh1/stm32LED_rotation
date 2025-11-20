/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx.h"

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

/* USER CODE BEGIN PV */
  uint32_t lastToggle[4] = {0, 0, 0, 0}; // Stores last toggle time for each LED
  uint32_t intervals[4] = {1000, 750, 500, 250}; // Toggle intervals in ms
  GPIO_PinState ledStates[4] = {GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_RESET};
	
	GPIO_PinState lastButtonState = GPIO_PIN_RESET;
  uint8_t led_index = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*
 * Turn ON only one LED according to the given index, turn OFF all others.
 *        LED order: 0 - orange (PD13), 1 - red (PD14), 2 - blue (PD15), 3 - green (PD12)
 * Index of the LED to turn ON (0 to 3)
 */
static void set_led_by_index(uint8_t idx)
{
  /* turn all off first */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
  // Turn ON the selected LED based on index
  switch (idx % 4)
  {
    case 0: HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET); break; // orange
    case 1: HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET); break; // red
    case 2: HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET); break; // blue
    case 3: HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET); break; // green
  }
}
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
  /* USER CODE BEGIN 2 */
    int simulated_proximity = 100;
  /* initialize button state and show first LED (cam) */
  lastButtonState = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
  led_index = 0;
  set_led_by_index(led_index);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  while (1)
  {
    uint32_t now = HAL_GetTick();

    // Toggle LED1 (PD13) every 1 second using direct register access
    if (now - lastToggle[0] >= intervals[0]) {
      ledStates[0] = (ledStates[0] == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET;
      if (ledStates[0] == GPIO_PIN_SET)
        GPIOD->ODR |= GPIO_PIN_13;   // Turn ON LED1
      else
        GPIOD->ODR &= ~GPIO_PIN_13;  // Turn OFF LED1
      lastToggle[0] = now;
    }

    // Toggle LED2 (PD14) every 750 ms
    if (now - lastToggle[1] >= intervals[1]) {
      ledStates[1] = (ledStates[1] == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET;
      if (ledStates[1] == GPIO_PIN_SET)
        GPIOD->ODR |= GPIO_PIN_14;   // Turn ON LED2
      else
        GPIOD->ODR &= ~GPIO_PIN_14;  // Turn OFF LED2
      lastToggle[1] = now;
    }

    // Toggle LED3 (PD15) every 500 ms
    if (now - lastToggle[2] >= intervals[2]) {
      ledStates[2] = (ledStates[2] == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET;
      if (ledStates[2] == GPIO_PIN_SET)
        GPIOD->ODR |= GPIO_PIN_15;   // Turn ON LED3
      else
        GPIOD->ODR &= ~GPIO_PIN_15;  // Turn OFF LED3
      lastToggle[2] = now;
    }

    // Toggle LED4 (PD12) every 250 ms
    if (now - lastToggle[3] >= intervals[3]) {
      ledStates[3] = (ledStates[3] == GPIO_PIN_SET) ? GPIO_PIN_RESET : GPIO_PIN_SET;
      if (ledStates[3] == GPIO_PIN_SET)
        GPIOD->ODR |= GPIO_PIN_12;   // Turn ON LED4
      else
        GPIOD->ODR &= ~GPIO_PIN_12;  // Turn OFF LED4
      lastToggle[3] = now;
    }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  /* Ensure all LEDs are OFF at startup */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);


  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  /* Use internal pull-down so unpressed = RESET, pressed = SET.
     If your button is active-low (pressed reads RESET), change to GPIO_PULLUP and invert logic. */
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

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
