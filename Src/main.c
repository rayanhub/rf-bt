/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "rng.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "si4463.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "delay_dwt.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
si4463_t si4463;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/*-------------- si4463 ---------------------*/
uint8_t incomingBuffer[RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH];
uint16_t i = 0;
int8_t rssi_value = 0;
uint8_t check_module_flag = 0;

/*-------------- UART ---------------------*/
char str[10];
uint8_t Rx_indx = 0;
char Rx_buffer[RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH];
uint8_t Rx_data[RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH] = {0,}, Transfer_cplt;
/*-----------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
uint8_t SI4463_IsCTS(void);
void SI4463_SetShutdown(void);
void SI4463_ClearShutdown(void);
void SI4463_Select(void);
void SI4463_Deselect(void);
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
  DWT_Init();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  MX_RNG_Init();
  /* USER CODE BEGIN 2 */

  //HAL_UART_Transmit(&huart2, incomingBuffer,RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH, 100);
  HAL_UART_Receive_IT (&huart2,  (uint8_t *)Rx_data, 1);

  /* Assign functions */
  si4463.IsClearToSend = SI4463_IsCTS;
  si4463.WriteRead = SI4463_WriteRead;
  si4463.SPIRead = SI4463_SPIRead;
  si4463.SPITransmit = SI4463_SPITransmit;
  si4463.Select = SI4463_Select;
  si4463.Deselect = SI4463_Deselect;
  si4463.SetShutdown = SI4463_SetShutdown;
  si4463.ClearShutdown = SI4463_ClearShutdown;
  si4463.DelayMs = DWT_Delay_ms;
  si4463.DelayUs = DWT_Delay_us;

  /* Disable interrupt pin for init Si4463 */
  HAL_NVIC_DisableIRQ(EXTI1_IRQn);

  SystemCheck();

  SI4463_Init(&si4463);
  SI4463_SetTxPower(&si4463,PaPwr100mW);
  /* Clear RX FIFO before starting RX packets */
  SI4463_ClearRxFifo(&si4463);

  /* Start RX mode.
   * SI4463_StartRx() put a chip in non-armed mode in cases:
   * - successfully receive a packet;
   * - invoked RX_TIMEOUT;
   * - invalid receive.
   * For receiveing next packet you have to invoke SI4463_StartRx() again!*/

  //SI4463_Rx_Hop	(&si4463, 0x42, 0x0e, 0x27, 0x63, 0x08, 0x77); // 440,500 MHz
  SI4463_StartRx(&si4463, 0, false, false, false);

  /* Enable interrupt pin and */
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  /* Clear interrupts after enabling interrupt pin.
   * Without it may be situation when interrupt is asserted but pin not cleared.*/
  SI4463_ClearInterrupts(&si4463);
  /* USER CODE END 2 */

  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 120;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


uint8_t SI4463_IsCTS(void)
{
	if((CTS_GPIO_Port->IDR & CTS_Pin) != (uint32_t)GPIO_PIN_RESET)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void SI4463_WriteRead(const uint8_t * pTxData, uint8_t * pRxData, const uint16_t sizeTxData)
{
	HAL_SPI_TransmitReceive(&hspi1, (uint8_t *) pTxData, pRxData, sizeTxData,100);
}

void SI4463_SPIRead(const uint8_t * pRxData, const uint16_t sizeRxData)
{
	HAL_SPI_Receive(&hspi1, (uint8_t *) pRxData, sizeRxData, 100);
}

void SI4463_SPITransmit(const uint8_t * pTxData, const uint16_t sizeTxData)
{
	HAL_SPI_Transmit(&hspi1, (uint8_t *) pTxData, sizeTxData, 100);
}

void SI4463_SetShutdown(void)
{
	HAL_GPIO_WritePin(SDN_GPIO_Port, SDN_Pin, GPIO_PIN_SET);
}

void SI4463_ClearShutdown(void)
{
	HAL_GPIO_WritePin(SDN_GPIO_Port, SDN_Pin, GPIO_PIN_RESET);
}

void SI4463_Select(void)
{
	//HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
	SPI1_NSS_GPIO_Port->BSRR = (uint32_t)SPI1_NSS_Pin << 16U;
	DWT_Delay_us(10);
}

void SI4463_Deselect(void)
{
	//HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);
	SPI1_NSS_GPIO_Port->BSRR = SPI1_NSS_Pin;
	DWT_Delay_us(10);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);

  /* Clear incoming buffer */
  memset(incomingBuffer, 0x00, RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH);

  /* Get interrupts and work with it */
  SI4463_GetInterrupts(&si4463);

  /* Handling PH interrupts */
  if (si4463.interrupts.filterMatch)
  {
	  /* Handling this interrupt here */
	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.filterMatch = false;
  }
  if (si4463.interrupts.filterMiss)
  {
	  /* Handling this interrupt here */
	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.filterMiss = false;
  }
  if (si4463.interrupts.packetSent)
  {
	  /* Handling this interrupt here */
	  /* Clear TX FIFO */
	  SI4463_ClearTxFifo(&si4463);

	  /* Re-arm StartRX */
	  //SI4463_Rx_Hop	(&si4463, 0x42, 0x0e, 0x27, 0x63, 0x08, 0x77); // 440,500 MHz
 	  SI4463_StartRx(&si4463, 0, false, false, false);

	  /*Toggle led for indication*/
	  HAL_GPIO_WritePin(LED_TX_GPIO_Port,LED_TX_Pin,GPIO_PIN_SET);
	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.packetSent = false;
  }
  if (si4463.interrupts.packetRx)
  {
	  /* Handling this interrupt here */
	  /* Get FIFO data */
	  SI4463_ReadRxFifo(&si4463, incomingBuffer, RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH);

	  HAL_UART_Transmit(&huart2, incomingBuffer, RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH, 100);
	  /* Clear RX FIFO */
	  SI4463_ClearRxFifo(&si4463);
	  /* Start RX again.
	   * It need because after successful receive a packet the chip change
	   * state to READY.
	   * There is re-armed mode for StartRx but it not correctly working */
	  //SI4463_Rx_Hop	(&si4463, 0x42, 0x0e, 0x27, 0x63, 0x08, 0x77); // 440,500 MHz
	  SI4463_StartRx(&si4463, 0, false, false, false);

	  /*Toggle led for indication*/
	  HAL_GPIO_WritePin(LED_RX_GPIO_Port,LED_RX_Pin,GPIO_PIN_SET);

	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.packetRx = false;
  }
  if (si4463.interrupts.crcError)
  {
	  /* Handling this interrupt here */

	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.crcError = false;
  }
  if (si4463.interrupts.txFifoAlmostEmpty)
  {
	  /* Handling this interrupt here */

	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.txFifoAlmostEmpty = false;
  }
  if (si4463.interrupts.rxFifoAlmostFull)
  {
	  /* Handling this interrupt here */

	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.rxFifoAlmostFull = false;
  }

  /* Handling Modem interrupts */
  if (si4463.interrupts.postambleDetect)
  {
	  /* Handling this interrupt here */
	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.postambleDetect = false;
  }
  if (si4463.interrupts.invalidSync)
  {
	  /* Handling this interrupt here */
	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.invalidSync = false;
  }
  if (si4463.interrupts.rssiJump)
  {
	  /* Handling this interrupt here */
	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.rssiJump = false;
  }
  if (si4463.interrupts.rssi)
  {
	  /* Handling this interrupt here */
	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.rssi = false;
  }
  if (si4463.interrupts.invalidPreamble)
  {
	  /* Handling this interrupt here */
	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.invalidPreamble = false;
  }
  if (si4463.interrupts.preambleDetect)
  {
	  /* Handling this interrupt here */
	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.preambleDetect = false;
  }
  if (si4463.interrupts.syncDetect)
  {
	  /* Handling this interrupt here */
	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.syncDetect = false;
  }

  /* Handling Chip interrupts */
  if (si4463.interrupts.cal)
  {
	  /* Handling this interrupt here */
	  SI4463_GetChipStatus(&si4463);
	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.cal = false;
  }
  if (si4463.interrupts.fifoUnderflowOverflowError)
  {
	  /* Handling this interrupt here */
	  /* Clear RX FIFO */
	  SI4463_ClearRxFifo(&si4463);
	  /* Claer Chip Status errors if exists */
	  SI4463_GetChipStatus(&si4463);
	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.fifoUnderflowOverflowError = false;
  }
  if (si4463.interrupts.stateChange)
  {
	  /* Handling this interrupt here */
	  SI4463_GetChipStatus(&si4463);
	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.stateChange = false;
  }
  if (si4463.interrupts.cmdError)
  {
	  /* Handling this interrupt here */
	  SI4463_GetChipStatus(&si4463);
	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.stateChange = false;
  }
  if (si4463.interrupts.chipReady)
  {
	  /* Handling this interrupt here */
	  SI4463_GetChipStatus(&si4463);
	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.chipReady = false;
  }
  if (si4463.interrupts.lowBatt)
  {
	  /* Handling this interrupt here */
	  SI4463_GetChipStatus(&si4463);
	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.lowBatt = false;
  }
  if (si4463.interrupts.wut)
  {
	  /* Handling this interrupt here */
	  SI4463_GetChipStatus(&si4463);
	  /* Following instruction only for add breakpoints. May be deleted */
	  si4463.interrupts.wut = false;
  }

  /* Clear All interrupts before exit */
  SI4463_ClearAllInterrupts(&si4463);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2)              //is current uart?
	{

		if (Rx_indx == 0)                       //if data is not being received
		{
			for (uint8_t i=0;i<64;i++) 			// 48
			{
				Rx_buffer[i]=0;				    // clear Rx_Buffer before receiving new data
			}
		}


		if (Rx_data[0]!=13)                     //if received data different from ascii 13 (enter)
		{
			Rx_buffer[Rx_indx++]= Rx_data[0];   // store data in buffer
		}

		else 									// if received data = 13
		{
			Rx_indx= 0;
			if(Rx_buffer > 1)
			{
				SI4463_Transmit(&si4463, 0x00, (uint8_t*)&Rx_buffer,0 /*sizeof(Rx_buffer)*/);
			}
			else if (!(strcmp(Rx_buffer, "tempC")))   // Start
			{
				flagRxCmdUART = cmdGetLocalTemp;
			}
			else if (!(strcmp(Rx_buffer, "3v3")))
			{
				flagRxCmdUART = cmdGetLocal3v3;
			}
			else if (!(strcmp(Rx_buffer, "bat")))
			{
				flagRxCmdUART = cmdGetLocalBat;
			}
			else if (!(strcmp(Rx_buffer, "Rbat")))
			{
				flagRxCmdUART = cmdGetRemoteBat;
			}
		}

		HAL_UART_Receive_IT (&huart2,  (uint8_t *)Rx_data, 1);    // activate receive
	}

}
void SystemCheck(void)
{
	BatCheck();
	SI4463_CheckModule();
}

void SI4463_CheckModule(void)
{
	uint8_t i;
	if(SI4463_CheckDevice(&si4463) == SI4463_OK)
	{
		for(i=0; i<6; i++)
		{
			HAL_GPIO_TogglePin(LED_TX_GPIO_Port,LED_TX_Pin);
			HAL_GPIO_TogglePin(LED_RX_GPIO_Port,LED_RX_Pin);
			DWT_Delay_ms(100);
		}
		HAL_UART_Transmit(&huart2, "+SI4463_INIT/OK\n", 15, 100);
	}
	else
	{
		for(i=0; i<6; i++)
		{
			HAL_GPIO_TogglePin(LED_RX_GPIO_Port,LED_RX_Pin);
			DWT_Delay_ms(100);
		}
		HAL_UART_Transmit(&huart2, "+SI4463_INIT/NO\n", 15, 100);
	}
}

void BatCheck(void)
{
	if(ADC_Get_Vbat() > 670)
		{
			if(ADC_Get_Vbat() < 690)
			{
				HAL_UART_Transmit(&huart2, "+LOWBAT\n", 7, 100);
			}
			else
			{
				//HAL_UART_Transmit(&huart2, "+BAT/OK\n", 7, 100);
			}
		}
		else
		{
			HAL_UART_Transmit(&huart2, "+BAT/NO\n", 7, 100);
			check_module_flag |= (1<<7);
			for(i=0; i<60; i++)
			{
				if(0 != (check_module_flag & (1<<7)) )
				{
					HAL_GPIO_WritePin(LED_TX_GPIO_Port,LED_TX_Pin,GPIO_PIN_RESET);
					break;
				}
			HAL_GPIO_TogglePin(LED_TX_GPIO_Port,LED_TX_Pin);
			DWT_Delay_ms(500);
			}
			HAL_UART_Transmit(&huart2, "+PWR_OFF\n", 8, 100);
			HAL_GPIO_WritePin(PWR_ON_GPIO_Port,PWR_ON_Pin,GPIO_PIN_RESET);
			//HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
		}
}

uint8_t Check_UART_Rx_CMD(void)
{


	uint16_t U3v3 = 0;
	uint16_t c_temp = 0;
	uint16_t Ubat = 0;

	uint8_t ret = clearFlag;

	switch (flagRxCmdUART)
	{

		case cmdGetLocalTemp :

			c_temp = ((2194*SI4463_GetTempAdc(&si4463)) / 1000) - 2930;
			sprintf(str, "%d\n", c_temp / 10);
			HAL_UART_Transmit(&huart2, (uint8_t*)(&str), strlen(str), 100);
			ret = flagRxCmdUART;
			flagRxCmdUART = clearFlag;
			break;

		case cmdGetLocal3v3 :
			for(uint8_t i=0; i<10; i++)
			{
				U3v3 += (30*SI4463_GetBatAdc(&si4463))/128;
			}
			U3v3 = U3v3/10;
			sprintf(str, "%d.%.2d\n", U3v3 / 100, U3v3 % 100);
			HAL_UART_Transmit(&huart2, (uint8_t*)(&str), strlen(str), 100);
			ret = flagRxCmdUART;
			flagRxCmdUART = clearFlag;
			break;

		case cmdGetLocalBat :

			Ubat = ADC_Get_Vbat();
			sprintf(str, "%d.%d\n", Ubat / 100, Ubat % 100);
			HAL_UART_Transmit(&huart2, (uint8_t*)(&str), strlen(str), 100);
			ret = flagRxCmdUART;
			flagRxCmdUART = clearFlag;
			break;

		case cmdGetRemote3v3 :
			SI4463_Transmit(&si4463, 0x01, (uint8_t*)(&str), strlen(str));
			HAL_GPIO_WritePin(LED_TX_GPIO_Port,LED_TX_Pin,GPIO_PIN_SET);

			SI4463_GetChipStatus(&si4463);
			SI4463_GetInterrupts(&si4463);
			break;
		case cmdGetRemoteBat :
			outgoingBuffer[TransmitCMD] = cmdGetRemoteBat;
			SI4463_Transmit(&si4463, 0x01, (uint8_t*)(&outgoingBuffer[TransmitCMD]), RADIO_CONFIGURATION_DATA_RADIO_PACKET_LENGTH);
			HAL_GPIO_WritePin(LED_TX_GPIO_Port,LED_TX_Pin,GPIO_PIN_SET);
			flagRxCmdUART = clearFlag;
			//SI4463_GetChipStatus(&si4463);
			//SI4463_GetInterrupts(&si4463);
			break;
		case cmdGetRemoteTelemetry :
			SI4463_Transmit(&si4463, 0x01, (uint8_t*)(&str), strlen(str));
			HAL_GPIO_WritePin(LED_TX_GPIO_Port,LED_TX_Pin,GPIO_PIN_SET);

			SI4463_GetChipStatus(&si4463);
			SI4463_GetInterrupts(&si4463);
			break;
		case cmdTurnRight :
			break;
		case cmdTurnleft :
			break;
		case cmdGOForward :
			break;
		case cmdGoBack :
			break;
		case cmdSetTxPower :
			break;

		default :
			break;
	}
	return ret;
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM3 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM3) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
