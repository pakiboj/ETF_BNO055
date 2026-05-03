/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stm32l4xx_ll_usart.h"
#include "stm32l4xx_ll_i2c.h"
#include "bno055.h"
#include "accel.h"
#include "MQTT.h"
#include "circ_buff.h"

#include <stdlib.h>

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
DMA_HandleTypeDef hdma_i2c1_rx;
DMA_HandleTypeDef hdma_i2c1_tx;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void I2C_scan(void);
void BNO055_SysCheck(void);


#define CIRCULAR_BUFFER_SIZE 20 //defines for circular buffer
#define PEEK_ARRAY_SIZE 5

static accel_sample_t circular_buffer_storage[CIRCULAR_BUFFER_SIZE] = {0};

volatile uint8_t bno055_data_ready = 0;
volatile uint8_t i2cdma_ready = 1;
volatile uint8_t irq_got = 0;
volatile size_t remain = 0;
volatile uint8_t int_reset = 0;
volatile uint8_t readData = 0;



static circular_buf_t cb;
static cbuf_handle_t handle_ = &cb;

static uint8_t imu_dma_buf[IMU_NUMBER_OF_BYTES];


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
	static char tx_buf[64];
	accel_sample_t data_sem;


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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  printf("New session started \n");
  HAL_Delay(2000);

  //  I2C_scan();
  //  printf("Configure finished\n");

  circular_buf_init(handle_, circular_buffer_storage, CIRCULAR_BUFFER_SIZE);
  printf("Circular buffer initialisation finished\n");

  BNO055_Init_I2C(&hi2c1);
  printf("BNO055 initialisation finished\n");

  //BNO055_SysCheck()

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
//  HAL_I2C_Mem_Read_DMA(&hi2c1, BNO055_I2C_ADDR_LO<<1,
//					   BNO055_REG_ACC_DATA_X_LSB,
//					   I2C_MEMADD_SIZE_8BIT,
//					   imu_dma_buf, IMU_NUMBER_OF_BYTES);
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY && bno055_data_ready == 1){ //GPIO interrupt handler
		  bno055_data_ready = 0;
		  HAL_I2C_Mem_Read_DMA(&hi2c1, BNO055_I2C_ADDR_LO<<1,
							   BNO055_REG_ACC_DATA_X_LSB,
							   I2C_MEMADD_SIZE_8BIT,
							   imu_dma_buf, IMU_NUMBER_OF_BYTES);
		  readData = 1;
		  //irq_got = 1; //flags that it got interrupt
		  //printf("interrupt \r\n");

	  }

//	  if(i2cdma_ready == 1 && irq_got == 1){ //I2C finished callback handler
//		  BNO055_ResetInterrupt(&hi2c1);
//		  i2cdma_ready = 0;
//		  readData = 1;
//		  irq_got = 0;
//	  }

	  if(readData == 1){
		  accel_sample_t s = {
		  	  ((int16_t)((imu_dma_buf[1] << 8) | imu_dma_buf[0])) / 100.0f,
			  ((int16_t)((imu_dma_buf[3] << 8) | imu_dma_buf[2])) / 100.0f,
			  ((int16_t)((imu_dma_buf[5] << 8) | imu_dma_buf[4])) / 100.0f
		  	  	  	  	  	  };
		  circular_buf_put(handle_, s);

		  readData = 0;
		  //printf("data read \r\n");
	  }


//	  remain = circular_buf_size(handle_);
//	  printf("remain=%u\r\n", (unsigned)remain);


	  if(!circular_buf_empty(handle_)&& (HAL_UART_GetState(&huart2) == HAL_UART_STATE_READY)) //sending to UART logic
	  {
		  circular_buf_get(handle_, &data_sem);
		  int len = snprintf(tx_buf, sizeof(tx_buf),
		                     "%.2f %.2f %.2f\r\n",
		                     data_sem.x, data_sem.y, data_sem.z);
		  HAL_UART_Transmit_DMA(&huart2,(uint8_t*)tx_buf, len); //send via DMA and forget

	  }



	  //using PuTTy for capturing readings

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
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
  hi2c1.Init.Timing = 0x00F12981;
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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 921600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_8;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);
  /* DMA1_Channel7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);
  /* DMA2_Channel6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Channel6_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel6_IRQn);

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : BNO055_INT_Pin */
  GPIO_InitStruct.Pin = BNO055_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BNO055_INT_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 3, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
int _write(int fd, char* ptr, int len) {
  HAL_StatusTypeDef hstatus;

  if (fd == 1 || fd == 2) {
    hstatus = HAL_UART_Transmit(&huart2, (uint8_t *) ptr, len, HAL_MAX_DELAY);
    if (hstatus == HAL_OK)
      return len;
    else
      return -1;
  }
  return -1;
}

//function used to scan I2C, even tho its written on datasheet, this function is used for checking
//working state of I2C lines
void I2C_scan(void)
{
    for (uint8_t i = 0; i < 128; i++) {
        if (HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(i<<1), 3, 5) == HAL_OK) {
            printf("%2x ", i);
        } else {
            printf("-- ");
        }
        if (i > 0 && (i + 1) % 16 == 0) printf("\n");
    }
    printf("Scan finished\n");
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == BNO055_INT_Pin) {
    	if(HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY){
			BNO055_ResetInterrupt(&hi2c1);
		}
    	bno055_data_ready = 1;
    	//i2cdma_ready = 0;
    }
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c == &hi2c1) {
    	//i2cdma_ready = 1;
    }
}

void BNO055_SysCheck(void){  //function for checking the working state of BNO chip
	char tx[] = "AT\r\n";
	  uint8_t rx[128] = {0};

	  HAL_UART_Transmit(&huart3, (uint8_t*)tx, strlen(tx), 200);
	  HAL_UART_Receive(&huart3, rx, sizeof(rx) - 1, 2000);
	  printf("Response: %s\r\n", rx);

	  uint8_t conf_page1[2] = {BNO055_REG_PAGE_ID, 0x01};
	  HAL_I2C_Master_Transmit(&hi2c1, BNO055_I2C_ADDR_LO<<1, conf_page1, sizeof(conf_page1), 10);
	  HAL_Delay(10);

	  uint8_t reg;
	  HAL_I2C_Mem_Read(&hi2c1, BNO055_I2C_ADDR_LO<<1, BNO055_REG_INT_EN, I2C_MEMADD_SIZE_8BIT, &reg, 1,10);
	  printf("INT_EN REG: 0x%02X\r\n", reg);
	  HAL_I2C_Mem_Read(&hi2c1, BNO055_I2C_ADDR_LO<<1, BNO055_REG_INT_MSK, I2C_MEMADD_SIZE_8BIT, &reg, 1,10);
	  printf("INT_MSK REG: 0x%02X\r\n", reg);


	  uint8_t conf_page0_set[2] = {BNO055_REG_PAGE_ID, 0x00};
	  HAL_I2C_Master_Transmit(&hi2c1, BNO055_I2C_ADDR_LO<<1, conf_page0_set, sizeof(conf_page0_set), 10);
	  HAL_Delay(10);

	  HAL_I2C_Mem_Read(&hi2c1, BNO055_I2C_ADDR_LO<<1, BNO055_REG_OPR_MODE, I2C_MEMADD_SIZE_8BIT, &reg, 1,10);
	  printf("OPR_MODE REG: 0x%02X\r\n", reg);
	  HAL_StatusTypeDef st = HAL_I2C_Mem_Read(&hi2c1, BNO055_I2C_ADDR_LO<<1, BNO055_REG_CHIP_ID, I2C_MEMADD_SIZE_8BIT, &reg, 1, 10);
	  printf("CHIP_ID: 0x%02X (status %d)\r\n", reg, st);
	  HAL_I2C_Mem_Read(&hi2c1, BNO055_I2C_ADDR_LO<<1, BNO055_REG_ACC_ID, I2C_MEMADD_SIZE_8BIT, &reg, 1,10);
	  printf("ACC_ID REG: 0x%02X\r\n", reg);

	  HAL_I2C_Mem_Read(&hi2c1, BNO055_I2C_ADDR_LO<<1, 0x04, I2C_MEMADD_SIZE_8BIT, &reg, 1,10);
	    printf("MSB SW REG: 0x%02X\r\n", reg);
	    HAL_I2C_Mem_Read(&hi2c1, BNO055_I2C_ADDR_LO<<1, 0x05, I2C_MEMADD_SIZE_8BIT, &reg, 1,10);
	    printf("LSB SW REG: 0x%02X\r\n", reg);

}

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
