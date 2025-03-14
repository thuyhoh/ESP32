#include "main.h"
#include "stdio.h"
#include <string.h>
#include <stdint.h>

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);

void chuongtrinhcambien();

//==================================
// KHAI BÁO BI?N

float giatri1 = 0;
float giatri2 = 0;
int giatri3 = 0;
int giatri4 = 0;
int giatri5 = 0;

uint8_t  rx_index, rx_data;

long last = 0;

char rx_buffer[50];

//==================================


#ifdef __GNUC__
     #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
     #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
	//HAL_UART_Transmit == Serial.println();
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,0xFFFF);
	HAL_UART_Transmit(&huart2,(uint8_t *)&ch,1,0xFFFF);
	return ch;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

	if(huart -> Instance == USART2)
	{
		// xoa mang du lieu di
		if(rx_index == 0)
		{
			for(int i = 0 ; i < 50 ; i++)
			{
				rx_buffer[i] = 0;
			}
		}
		//==========================================================
			
		if(rx_data != '\n' )
		{

			rx_buffer[rx_index++] = rx_data;		
			
		}
		else
		{
			rx_buffer[rx_index++] = rx_data;
			
			rx_index = 0;
			// hien va xu lý
			//HAL_UART_Transmit(&huart1,(uint8_t*)rx_buffer,sizeof(rx_buffer),100);
			
			//strstr
			if(strstr(rx_buffer,"LM") != NULL )
			{
				chuongtrinhcambien();
				printf("A%fB%fC%dD%dE%dF\n", giatri1 , giatri2 , giatri3, giatri4 , giatri5);

			}
			/*
			int TimA , TimB , TimC = 0;
				 
			for(int i = 0 ; i < sizeof(rx_buffer) ; i++)
			{
				if(rx_buffer[i] == 'A')
				{
					TimA = i;
				}
				if(rx_buffer[i] == 'B')
				{
					TimB = i;
				}
				if(rx_buffer[i] == 'C')
				{
					TimC = i;
				}
			}
			if(TimA >=  0 && TimB > 0 && TimC > 0)
			{
				uint8_t OK[] = "OK";
				HAL_UART_Transmit(&huart2,OK,sizeof(OK),100);	
				//======================================================
				// cat chuoiA						
				int c = 0;		
				for (int i = TimA ; i < TimB - 1; i++)
				{
					ChuoiA[c] += rx_buffer[i + 1];
					c++;
				}
				//HAL_UART_Transmit(&huart2,ChuoiA,sizeof(ChuoiA),100);
				//======================================================	
				// Cat ChuoiB			
				c = 0;
				
				for (int i = TimB ; i < TimC - 1; i++)
				{
					ChuoiB[c] += rx_buffer[i + 1];
					c++;
				}
				//HAL_UART_Transmit(&huart2,ChuoiB,sizeof(ChuoiB),100);
				//======================================================
			  int a = 0;
				int  PhanChuc, PhanDV = 0;
			
				int dodaichuoiA = 0;
				
				for(int i = 0 ; i < sizeof(ChuoiA) ; i++)
				{
					if(ChuoiA[i] != NULL)
					{
						//a = (ChuoiA[i]) - '0';
						dodaichuoiA++;							
					}
				}
				if(dodaichuoiA == 2)
				{
					 PhanChuc = (ChuoiA[0]) - '0'; char  '1' => int '1' => 1
					 PhanDV = (ChuoiA[1]) - '0';
					 a = PhanChuc*10 + PhanDV;
					if(a == 45)
					{
						//printf("%s \r\n"  , rx_buffer);
						
						printf("%d. Ðúng \r\n", ++line);
					}
				}
				//======================================================
			}
			*/

			
		}	
		
		HAL_UART_Receive_IT(&huart2,&rx_data,1);
	}
	

}



int main(void)
{

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
	
	// khoi tao ngat uart cong giao tiep esp
	HAL_UART_Receive_IT(&huart2,&rx_data,1);
	
	last = HAL_GetTick(); 

 
  while (1)
  {    		
		if(HAL_GetTick() - last >= 1000)
		{
			HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
			chuongtrinhcambien();
			// A cambien1 B cambien2 C cambien.....
			//printf("A%fB%fC%dD%dE%dF\n", giatri1 , giatri2 , giatri3, giatri4 , giatri5);
			last = HAL_GetTick();
		}			
  }
  
}


void chuongtrinhcambien()
{
	giatri1++;
	giatri2 = giatri2 + 2;
	giatri3 = giatri3 + 3;
	giatri4 = giatri4 + 4;
	giatri5 = giatri5 + 5;
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA4 PA5
                           PA6 PA7 PA8 PA11
                           PA12 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB10
                           PB11 PB12 PB13 PB14
                           PB15 PB3 PB4 PB5
                           PB6 PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
