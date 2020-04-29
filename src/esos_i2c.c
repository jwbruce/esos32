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
* \brief Hardware independent support for ESOS32 I<sup>2</sup>C services
* 
* Must have supporing hw-specific files underneath to work.
*/


/*** I N C L U D E S *************************************************/
#include "esos.h"
#include "esos_i2c.h"

/*** G L O B A L S *************************************************/
struct stTask   	__stChildTaskI2C;
uint8_t           	__esos_i2c_dataBytes[2];

/*** T H E   C O D E *************************************************/

/*********************************************************
 * Public functions intended to be called by other files *
 *********************************************************/

/**
Configure and enable the ESOS I2C service for operation at \em u32_i2cbbps
bits per second clock speed.
\param u32_i2cbps specifies clock speed in bits per second
\sa ESOS_TASK_WAIT_ON_WRITENI2C
\sa ESOS_TASK_WAIT_ON_READNI2C
\hideinitializer
 */
void __esos_i2c_config(uint32_t u32_i2cbps) {
	// setup any ESOS structures needed for I2C here
	__esos_SetSystemFlag(__ESOS_SYS_I2C_IS_BUSY); 
	
	// call the hardware provided function to setup the HW itself
	__esos_i2c_hw_config( u32_i2cbps);
	
	// do any last-minute I2C configuration for ESOS
	__esos_ClearSystemFlag(__ESOS_SYS_I2C_IS_BUSY); 
}

/**@}*/
