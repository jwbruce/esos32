/*
 * "Copyright (c) 2019 J. W. Bruce ("AUTHOR(S)")"
 * All rights reserved.
 * (J. W. Bruce, jwbruce_AT_tntech.edu, Tennessee Tech University)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the authors appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS"
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Please maintain this header in its entirety when copying/modifying
 * these files.
 *
 *
 */

/**
 * \addtogroup ESOS_Tick
 * @{
 */


// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/**
* \file
* \brief STM32L4xx hardware support to generate the ESOS system tick functionality.
* 
* \note Currently the code supports the STM32L452RE processor..... on the Nucleo-L452RE board.
*/

// Include any HW-specific header files to pick up the HW register
//  definitions, macros, etc.
#include    "esos_stm32l4.h"

// local prototypes
void Error_Handler(void);

// create a "global" variable for the esos_tick count
//  It either contains the actual tick (in the case of a using the
//  software variable) or the system tick of the last time the tick
//  was accessed (in the case of a hardware timer register
//  containing the real clock tick)
volatile  uint32_t        esos_tick_count;

/****************************************************/
/*
* \brief Initializes the ESOS system tick.
*
* \pre None assumed
*
* \post Sets up any hardware required to generate the 1.0ms tick
* required by ESOS.
*
* The (platform-independent) ESOS initialization code will
* call this function to setup and init the hardware (HWXXX
* MCU, in this case) to create the required timers,IRQs,etc
* to generate the 1.0ms ESOS system tick.
*
* \note We can either generate an IRQ every 1.0ms or longer period,
* we just need to make sure that ISR that increments the tick
* count is consistent.
********************************************************/
void    __esos_hw_InitSystemTick(void) {
  // This implementation relies on the HAL for the TICK and
  // many other things.  So before, we get the tick going, we
  // need to initialize the HAL

  /* Reset of all peripherals, Initializes the Flash interface and the SysTick. */
  HAL_Init();

  // FOR NOW, we will rely on the HAL initialization code to setup
  // its tick timer correctly.  Copied from Cube-created code
  // function  SystemClock_Config(void)

  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Initializes the CPU, AHB and APB busses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_RNG;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.RngClockSelection = RCC_RNGCLKSOURCE_PLL;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
    Error_Handler();
  }
  /**Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
    Error_Handler();
  }

  // call STM32 HAL routine the sets up our clock and hardware tick timer
  // SystemClock_Config();

  // Finish initializing the hardware by resetting tick to
  // the beginning of time.... zero.
  esos_tick_count = 0;

} // end __esos_hw_InitSystemTick()

/****************************************************/

/* *****************************************************
* \brief Returns the ESOS system tick count.
*
* \pre ESOS system tick is running/working.
*
* \return A 32-bit value of the number of ESOS system ticks
* since the system has booted.
*
********************************************************/
uint32_t   __esos_hw_GetSystemTickCount(void) {
  // if a hardware timer register contains the free-running tick
  // value, be sure to save the register value to esos_tick_count
  // before returning.  This is done to ensure that any rogue app
  // that tries to read esos_tick_count directly without calling
  // the ESOS_GET_TICKS() macro (which ultimately maps to this
  // function) will get a tick value that is
  // reasonable..... maybe....  sort of..... not really...
  //
  esos_tick_count = HAL_GetTick();

  return  esos_tick_count;
}  // end __esos_hw_GetSystemTickCount()


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}
