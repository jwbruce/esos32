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
 * \addtogroup ESOS_I2C_Service
 * @{
 */

// Documentation for this file. If the \file tag isn't present,
// this file won't be documented.
/** \file
* \brief Hardware-specific support for ESOS32 I<sup>2</sup>C services
*        on the STM32L4xx MCUs (tested on STM32L452RE)
*/


/*** I N C L U D E S *************************************************/
#include "esos_stm32l4_i2c.h"

#include	<stdio.h>


/*** D E F I N E S *************************************************/
#define DEBUG(str)			__esos_unsafe_PutString(str)

/*** G L O B A L S *************************************************/
struct stTask   __stChildTaskI2C, __stGrandChildTaskI2C;
uint8_t           __esos_i2c_dataBytes[2];

static char			ac_debug[80];

/*** T H E   C O D E *************************************************/

/*********************************************************
 * Public functions intended to be called by other files *
 *********************************************************/

// Documentation for this file. If the \file tag is not present,
// this file will not be documented.
// Note: place this comment below the #if NUM_I2C_MODS so Doxygen
// will only see it once.
/** \file
 *  I2C support functions. \see pic24_i2c.h for details.
 */


/**
Configure and enable the I2C1 module for operation at \em u16_fKHz KHz clock speed.
\param u16_fKHz specifies clock speed in KHz
\sa ESOS_TASK_WAIT_ON_WRITENI2C1
\sa ESOS_TASK_WAIT_ON_READNI2C1
\hideinitializer
 */
void __esos_i2c_hw_config(uint32_t u32_i2cbps) {
	rcc_periph_clock_enable(RCC_I2C1);
	
	// this clock should have been enabled in stm32l4_tick.c
	//rcc_periph_clock_enable(RCC_GPIOB);
	
	// This line is used in the f3 example, but it does not exist in l4
	//rcc_set_i2c_clock_hsi(I2C1);
	// will need to poke the register manually...
	RCC_CCIPR |= 0x00002000;

	i2c_reset(I2C1);
	/* Setup GPIO pin GPIO_USART2_TX/GPIO9 on GPIO port B for transmit. */
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO8);
	gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_100MHZ, GPIO8);
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO9);
	gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_100MHZ, GPIO9);
	gpio_set_af(GPIOB, GPIO_AF4, GPIO8);
	gpio_set_af(GPIOB, GPIO_AF4, GPIO9);
	i2c_peripheral_disable(I2C1);
	//configure ANFOFF DNF[3:0] in CR1
	i2c_enable_analog_filter(I2C1);
	i2c_set_digital_filter(I2C1, 0);
	/* HSI is at 8Mhz */
	i2c_set_speed(I2C1, u32_i2cbps, 8);
	//configure No-Stretch CR1 (only relevant in slave mode)
	i2c_enable_stretching(I2C1);
	//addressing mode
	i2c_set_7bit_addr_mode(I2C1);
	i2c_set_own_7bit_slave_address(I2C1, 0);
	// enable the TX interrupt
	//i2c_enable_interrupt(I2C1, I2C_CR1_TXIE);
	//i2c_enable_interrupt(I2C1, I2C_CR1_RXIE);
	i2c_peripheral_enable(I2C1);
	//sprintf(ac_debug,"   4: 0x%08lx 0x%08lx 0x%08lx 0x%08lx", I2C1_CR1, I2C1_CR2, I2C1_ISR, I2C1_TXDR);   DEBUG(ac_debug);
}

/**
 * 
 * ESOS_CHILD_TASK( __esos_i2c_hw_writeMReadN, uint8_t u8_addr, uint8_t* pu8_wd, uint8_t u8_wcnt, uint8_t* pu8_rd, uint8_t u8_rcnt)
 * 
 * Transaction: Write \em u8_wcnt bytes stored in buffer \em *pu8_wd to I2C slave,
 * then read \em u8_rcnt bytes into buffer \em *pu8_rd from I2C slave.
 * I2C slave is located at I2C address \em u8_addr.
 * \param u8_addr  Slave I2C address
 * \param pu8_d Pointer to buffer containing bytes to send
 * \param u16_cnt Number of bytes to send
 * 
 * /note Once written, the writeN() and readN() routines can call this
 * routine to do their dirty work
 */

/**
 * Transaction: Write \em u16_cnt bytes stored in buffer \em *pu8_d to I2C slave at address \em u8_addr.
 * \param u8_addr  Slave I2C address
 * \param pu8_d Pointer to buffer containing bytes to send
 * \param u16_cnt Number of bytes to send
 * 
 */
ESOS_CHILD_TASK( __esos_i2c_hw_writeN, uint8_t u8_addr, uint8_t* pu8_d, uint8_t u8_cnt) {
  static uint8_t        u8_tempAddr;
  static uint8_t*       pu8_tempPtr;
  static uint8_t       	u8_tempCnt;
  
  

	
  ESOS_TASK_BEGIN();
  //sprintf(ac_debug,"i2c_hw_writeN 1: 0x%08lx 0x%08lx 0x%08lx 0x%08lx\n", I2C1_CR1, I2C1_CR2, I2C1_ISR, I2C1_ICR);   DEBUG(ac_debug);
  ESOS_TASK_WAIT_WHILE( __ESOS_I2C_STM32L4_IS_BUSY() );
  CLEAR_REGISTER_BITS(I2C1_CR2, I2C_CR2_SADD_7BIT_MASK | I2C_CR2_NBYTES_MASK | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_START | I2C_CR2_STOP );


  // __ESOS_I2C_STM32L4_RESET_CR2();
  //sprintf(ac_debug,"   2: 0x%08lx 0x%08lx 0x%08lx 0x%08lx\n", I2C1_CR1, I2C1_CR2, I2C1_ISR, I2C1_ICR);   DEBUG(ac_debug);
  
  u8_tempAddr=u8_addr;
  pu8_tempPtr=pu8_d;
  u8_tempCnt=u8_cnt;
  
  __ESOS_I2C_STM32L4_SET_ADDR7_MODE();
  __ESOS_I2C_STM32L4_SET_ADDR7(MAKE_I2C_WR_ADDR(u8_tempAddr));
  __ESOS_I2C_STM32L4_SET_WRITE_DIR();
  __ESOS_I2C_STM32L4_SET_NUM_BYTES(u8_cnt);  
  __ESOS_I2C_STM32L4_SET_AUTOEND();
  //sprintf(ac_debug,"   3: 0x%08lx 0x%08lx 0x%08lx 0x%08lx\n", I2C1_CR1, I2C1_CR2, I2C1_ISR, I2C1_ICR);   DEBUG(ac_debug);
  
	//i2c_peripheral_enable(I2C1);
	//i2c_send_start(I2C1);
	  I2C1_CR2 |= I2C_CR2_START;


  //sprintf(ac_debug,"   4: 0x%08lx 0x%08lx 0x%08lx 0x%08lx\n", I2C1_CR1, I2C1_CR2, I2C1_ISR, I2C1_ICR);   DEBUG(ac_debug);
  while (u8_tempCnt--) {
			/*ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_SEND_STRING("TX NOT EMPTY \n");
			ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();*/
	  
	  //ESOS_TASK_WAIT_WHILE( __ESOS_I2C_STM32L4_IS_NACK_RECEIVED());
	 // sprintf(ac_debug,"   5: 0x%08lx 0x%08lx 0x%08lx 0x%08lx", I2C1_CR1, I2C1_CR2, I2C1_ISR, I2C1_TXDR);   DEBUG(ac_debug);
	  ESOS_TASK_WAIT_UNTIL( __ESOS_I2C_STM32L4_IS_TX_EMPTY() );
	  
			ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_SEND_STRING("TX  EMPTY\n\r");
			ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
	  
	  //i2c_send_data(I2C1, *pu8_tempPtr++);
	  
	  //sprintf(ac_debug,"   6: 0x%08lx 0x%08lx 0x%08lx 0x%08lx\n\r", I2C1_CR1, I2C1_CR2, I2C1_ISR, I2C1_TXDR);   DEBUG(ac_debug);
	  I2C1_TXDR = *pu8_tempPtr;
	  pu8_tempPtr++;
	  
  	  //I2C1_CR2 |= I2C_CR2_START;

	  //I2C1_ICR &= I2C_ICR_NACKCF;
  } // end-while
  // HAL code in HAL_I2C_Master_Transmit (and family ref manual) says:
  // NO need to check TC flag with AUTOEND mode, the STOP is
  // automatically generated.
  //DEBUG("7\n");
  // clear the STOP flag		(HAL code does this)
  __ESOS_I2C_STM32L4_CLEAR_STOP_FLAG();
  // reset CR2 register		(HAL code does this)
  __ESOS_I2C_STM32L4_RESET_CR2();
  
  ESOS_TASK_END();
} // end __esos_i2c_hw_writeN

/**
 * Transaction: Read \em u8_cnt bytes from I2C slave at address
 * \em u8_addr, save to buffer \em *pu8_d.
 * As per the I2C standard, a NAK is returned for the last byte read
 * from the slave, ACKs are returned for the other bytes.
 * \param u8_addr  Slave I2C address
 * \param pu8_d Pointer to buffer for storing bytes read from slave
 * \param u8_cnt Number of bytes read from slave.
 * 
 */
ESOS_CHILD_TASK( __esos_i2c_hw_readN, uint8_t u8_addr, uint8_t* pu8_d, uint8_t u8_cnt) {
  static uint8_t        u8_tempAddr;
  static uint8_t*       pu8_tempD;
  static uint8_t        u8_tempCnt, u8_i;

  ESOS_TASK_BEGIN();
  ESOS_TASK_WAIT_WHILE( __ESOS_I2C_STM32L4_IS_BUSY() );  
  u8_tempAddr = u8_addr;
  pu8_tempD = pu8_d;
  u8_tempCnt = u8_cnt;
  
  //__ESOS_I2C_STM32L4_SET_ADDR7_MODE();
  __ESOS_I2C_STM32L4_SET_ADDR7(u8_tempAddr);
  __ESOS_I2C_STM32L4_SET_READ_DIR();
  __ESOS_I2C_STM32L4_SET_NUM_BYTES(u8_cnt);  

	I2C1_CR2 |= I2C_CR2_START;
  // enable AUTOEND after the START to get REPEATED START
  __ESOS_I2C_STM32L4_SET_AUTOEND();   
  for (u8_i=0; u8_i < u8_tempCnt; u8_i++) {
	  	//sprintf(ac_debug,"   6: 0x%08lx 0x%08lx 0x%08lx 0x%08lx\n\r", I2C1_CR1, I2C1_CR2, I2C1_ISR, I2C1_RXDR);   DEBUG(ac_debug);
	  
		/*ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("RX EMPTY \n\r ");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();*/
	  
    ESOS_TASK_WAIT_UNTIL( __ESOS_I2C_STM32L4_IS_RX_NOT_EMPTY() );
	  
	 	/*ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("RX NOT EMPTY \n\r ");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();*/
	  
		  	//sprintf(ac_debug,"   7: 0x%08lx 0x%08lx 0x%08lx 0x%08lx\n\r", I2C1_CR1, I2C1_CR2, I2C1_ISR, I2C1_RXDR);   DEBUG(ac_debug);
    *pu8_tempD = I2C1_RXDR;
    pu8_tempD++;
	//I2C1_RXDR = 0;
  } // end-for

  // HAL code in HAL_I2C_Master_Transmit (and family ref manual) says:
  // NO need to check TC flag with AUTOEND mode, the STOP is
  // automatically generated.
  
  // clear the STOP flag		(HAL code does this)
  __ESOS_I2C_STM32L4_CLEAR_STOP_FLAG();
  // reset CR2 register		(HAL code does this)
  __ESOS_I2C_STM32L4_RESET_CR2();
  
  ESOS_TASK_END();
} // end __esos_i2c_hw_readN
  
/**
 * \TODO WRITE THIS CHILD TASK    __esos_i2c_hw_WriteMReadN
 
 * 
 * ESOS_CHILD_TASK( __esos_i2c_hw_writeMReadN, uint8_t u8_addr, uint8_t* pu8_wd, uint8_t u8_wcnt, uint8_t* pu8_rd, uint8_t u8_rcnt)
 * 
 * Transaction: Write \em u8_wcnt bytes stored in buffer \em *pu8_wd to I2C slave,
 * then read \em u8_rcnt bytes into buffer \em *pu8_rd from I2C slave.
 * I2C slave is located at I2C address \em u8_addr.
 * \param u8_addr  Slave I2C address
 * \param pu8_d Pointer to buffer containing bytes to send
 * \param u16_cnt Number of bytes to send
 * 
 * /note Once written, the writeN() and readN() routines can call this
 * routine to do their dirty work
 */

/**
 * Transaction: Write \em u8_wcnt bytes stored in buffer \em *pu8_wd
 * to I2C slave at address \em u8_addr, followed by a read of
 * \em u8_rcnt bytes into the buffer \em *pu8_rd
 * 
 * \param u8_addr  Slave I2C address
 * \param pu8_wd Pointer to buffer containing bytes to send
 * \param u8_wcnt Number of bytes to send
 * \param pu8_rd Pointer to buffer to "catch" bytes read from device
 * \param u8_rcnt Number of bytes to read

 * 
 */
ESOS_CHILD_TASK( __esos_i2c_hw_writeNReadM, uint8_t u8_addr, uint8_t* pu8_wd, uint8_t u8_wcnt, uint8_t* pu8_rd, uint8_t u8_rcnt) {
  static uint8_t        u8_tempAddr;
  static uint8_t*       pu8_tempWPtr;
  static uint8_t       	u8_tempWCnt;
  static uint8_t*       pu8_tempRPtr;
  static uint8_t       	u8_tempRCnt;
  
  // make local copies of the arguments
  u8_tempAddr=u8_addr;
  pu8_tempWPtr=pu8_wd;
  u8_tempWCnt=u8_wcnt;
  pu8_tempRPtr=pu8_rd;
  u8_tempRCnt=u8_rcnt;
  

	
  ESOS_TASK_BEGIN();
  ESOS_TASK_WAIT_WHILE( __ESOS_I2C_STM32L4_IS_BUSY() );
  CLEAR_REGISTER_BITS(I2C1_CR2, I2C_CR2_SADD_7BIT_MASK | I2C_CR2_NBYTES_MASK | I2C_CR2_RELOAD | I2C_CR2_AUTOEND | I2C_CR2_START | I2C_CR2_STOP );
  
  __ESOS_I2C_STM32L4_SET_ADDR7_MODE();
  __ESOS_I2C_STM32L4_SET_ADDR7(MAKE_I2C_WR_ADDR(u8_tempAddr));
  __ESOS_I2C_STM32L4_SET_WRITE_DIR();
  __ESOS_I2C_STM32L4_SET_NUM_BYTES(u8_wcnt);  
  __ESOS_I2C_STM32L4_SET_AUTOEND();

  I2C1_CR2 |= I2C_CR2_START;

  while (u8_tempWCnt--) {
	  ESOS_TASK_WAIT_UNTIL( __ESOS_I2C_STM32L4_IS_TX_EMPTY() );
	  I2C1_TXDR = *pu8_tempWPtr;
	  pu8_tempWPtr++;
  } // end-while

  // HAL code in HAL_I2C_Master_Transmit (and family ref manual) says:
  // NO need to check TC flag with AUTOEND mode, the STOP is
  // automatically generated.
  // clear the STOP flag		(HAL code does this)
  __ESOS_I2C_STM32L4_CLEAR_STOP_FLAG();
  // reset CR2 register		(HAL code does this)
  __ESOS_I2C_STM32L4_RESET_CR2();
  
  ESOS_TASK_END();
} // end __esos_i2c_hw_writeN
