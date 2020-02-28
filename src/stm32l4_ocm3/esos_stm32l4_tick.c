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
int count = 0;

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
	
	// Config the SysTick
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);

	systick_set_reload(80e3);
	systick_counter_enable();
	systick_interrupt_enable();
	
	rcc_osc_on(RCC_HSI16);
	
	flash_prefetch_enable();
	flash_set_ws(4);
	flash_dcache_enable();
	flash_icache_enable();
	/* 16MHz / 4 = > 4 * 40 = 160MHz VCO => 80MHz main pll  */
	rcc_set_main_pll(RCC_PLLCFGR_PLLSRC_HSI16, 4, 40,
			0, 0, RCC_PLLCFGR_PLLR_DIV2);
	rcc_osc_on(RCC_PLL);
	/* either rcc_wait_for_osc_ready() or do other things */

	/* Enable clocks for the ports we need */
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOD);
	rcc_periph_clock_enable(RCC_GPIOE);

	/* Enable clocks for peripherals we need */
	//rcc_periph_clock_enable(RCC_USART2);
	rcc_periph_clock_enable(RCC_SYSCFG);

	rcc_set_sysclk_source(RCC_CFGR_SW_PLL); /* careful with the param here! */
	rcc_wait_for_sysclk_status(RCC_PLL);
	/* FIXME - eventually handled internally */
	rcc_ahb_frequency = 80e6;
	rcc_apb1_frequency = 80e6;
	rcc_apb2_frequency = 80e6;
	
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

  return  esos_tick_count;
}  // end __esos_hw_GetSystemTickCount()

void sys_tick_handler(void)
{
	// ISR for the systick, named by LibOpenCM3
	// Increment the esos tick counter
	esos_tick_count++;
	
	// The timer services callback function for ESOS
	// Must be called every tick
	__esos_tmrSvcsExecute();
}

// temporary home for the Default handler
void Default_Handler()
{
	
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}
