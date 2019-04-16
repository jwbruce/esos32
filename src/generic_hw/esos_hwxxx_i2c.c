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
* \brief Template for creating hardware-specific support for ESOS32
* I<sup>2</sup>C services
*/


/*** I N C L U D E S *************************************************/
#include "esos_hwxxx_i2c.h"


/*** D E F I N E S *************************************************/

/*** G L O B A L S *************************************************/
struct stTask   	__stChildTaskI2C, __stGrandChildTaskI2C;
uint8_t           __esos_i2c_dataBytes[2];

/*** T H E   C O D E *************************************************/

/*********************************************************
 * Public functions intended to be called by other files *
 *********************************************************/

// Documentation for this file. If the \file tag is not present,
// this file will not be documented.
// Note: place this comment below the #if NUM_I2C_MODS so Doxygen
// will only see it once.
/** \file
 *  I2C support functions. \see esos_i2c.h for details.
 */


/**
Configure and enable the I2C1 module for operation at \em u32_i2cbps.
\param u32_i2cbps specifies clock speed in bits per second
\sa ESOS_TASK_WAIT_ON_WRITENI2C1
\sa ESOS_TASK_WAIT_ON_READNI2C1
\hideinitializer
 */
void __esos_i2c_hw_config(uint32_t u32_i2cbps) {
	// Set up the hardware aspects of the STM32L4 interface of the
	// I2C service direction, timings, address sizes, etc beyond
	// what is already done in esos_i2c_config()


}

/**
 * Transaction: Write \em u8_cnt bytes stored in buffer \em *pu8_d to I2C slave,
 * I2C slave is located at I2C address \em u8_addr.
 * \param u8_addr  Slave I2C address
 * \param pu8_d Pointer to buffer containing bytes to send
 * \param u8_cnt Number of bytes to send
 * 
 */
ESOS_CHILD_TASK( __esos_i2c_hw_writeN, uint8_t u8_addr, uint8_t* pu8_d, uint8_t u8_cnt) {
  ESOS_TASK_BEGIN();

	// put your hardware specific-code here
  
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
  ESOS_TASK_BEGIN();

	// put your hardware specific-code here

  ESOS_TASK_END();
} // end __esos_i2c_hw_readN
  
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
 */
ESOS_CHILD_TASK( __esos_i2c_hw_writeNReadM, uint8_t u8_addr, uint8_t* pu8_wd, uint8_t u8_wcnt, uint8_t* pu8_rd, uint8_t u8_rcnt) {
  ESOS_TASK_BEGIN();
  
	// put your hardware specific-code here  
  
  ESOS_TASK_END();
} // end __esos_i2c_hw_writeN
